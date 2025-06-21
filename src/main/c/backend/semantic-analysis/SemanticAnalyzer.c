#include "SemanticAnalyzer.h"
#include "../../shared/Logger.h"
#include <stdbool.h>

static Logger *_logger = NULL;

int amountOfColors = 0;

void initializeSemanticAnalyzerModule()
{
    _logger = createLogger("SemanticAnalyzer");
}

void shutdownSemanticAnalyzerModule()
{
    if (_logger != NULL)
    {
        destroyLogger(_logger);
    }
}

static bool validateConfiguration(Configuration *configuration)
{

    // Vemos que la lista de configuraciones no sea NULL
    if (configuration == NULL)
    {
        logError(_logger, "Configuration is NULL.");
        return false;
    }

    // Chequeamos que no se repitan los tipos obligatorios de configuracion
    int obligatoryTypes[5] = {0};
    int i = 0;
    Configuration *current = configuration;

    do
    {
        if (!current->isLast)
        {
            if (!obligatoryTypes[current->option->type])
            {
                obligatoryTypes[current->option->type] = 1;
            }
            else
            {
                logError(_logger, "A configuration is repeated");
                return false;
            }
        }
        else
        {
            if (!obligatoryTypes[current->lastOption->type])
            {
                obligatoryTypes[current->lastOption->type] = 1;
            }
            else
            {
                logError(_logger, "A configuration is repeated");
                return false;
            }
        }
        i++;
    } while (!current->isLast);

    // Vemos que esten todos los tipos obligatorios
    if (i < 5)
    {
        logError(_logger, "Configuration is missing some obligatory options.");
        return false;
    }

    return validateConfigurationRec(configuration);
}

static bool validateConfigurationRec(Configuration *configuration)
{
    bool validConfig = true;
    if (configuration->isLast)
    {
        validConfig = validateConfigurationAux(configuration->lastOption);
    }
    else
    {
        validConfig = validateConfigurationAux(configuration->option);
    }

    if (!validConfig)
    {
        return false;
    }

    if (configuration->isLast)
    {
        logDebugging(_logger, "Configuration is last.");
        return true;
    }
    else
    {

        return validateConfigurationRec(configuration->next);
    }
}

static bool validateConfigurationAux(Option *option)
{
    // Vemos que WIDTH y HEIGHT sean validas
    if (option == WIDTH_OPTION || option == HEIGHT_OPTION)
    {
        if (option->value <= 0)
        {
            logError(_logger, "Configuration option %s must be greater than 0.",
                     option->type == WIDTH_OPTION ? "Width" : "Height");
            return false;
        }
        else
        {
            logDebugging(_logger, "Configuration option %s is valid with value %d.",
                         option->type == WIDTH_OPTION ? "Width" : "Height",
                         option->value);
        }
    }
    else if (option == FRONTIER_OPTION) // Vemos que el FRONTIER_OPTION sea valido
    {
        if (option->frontierType == PERIODIC ||
            option->frontierType == OPEN ||
            option->frontierType == MIRROR)
        {
            logDebugging(_logger, "Configuration option Frontier is valid with type");
        }
        else
        {
            logError(_logger, "Frontier configuration is not valid");
        }
    }
    else if (option == COLORS_OPTION) // Vemos que los COLORS sean validos
    {
        if (option->colors == NULL)
        {
            logError(_logger, "Configuration option COLORS is NULL.");
            return false;
        }
        else
        {
            IntArray *colors = option->colors;
            do
            {
                amountOfColors++;
                if (colors->isLast)
                {
                    if (colors->lastValue < 0)
                    {
                        logError(_logger, "Configuration option COLORS contains a negative value");
                        return false;
                    }
                    else
                    {
                        logDebugging(_logger, "Configuration option COLORS is valid");
                    }
                }
                else
                {
                    if (colors->value < 0)
                    {
                        logError(_logger, "Configuration option COLORS contains a negative value");
                        return false;
                    }
                    else
                    {
                        logDebugging(_logger, "Configuration option COLORS is valid");
                        colors = colors->next;
                    }
                }
            } while (!colors->isLast);
        }
    }
    else if (option == STATES_OPTION) // Vemos que los STATES sean validos
    {
        int amountOfStates = 0;
        StringArray *states = option->states;
        if (states == NULL)
        {
            logError(_logger, "Configuration option STATES is NULL.");
            return false;
        }
        else
        {
            do
            {
                amountOfStates++;
                if (states->isLast)
                {
                    if (states->lastValue == NULL || states->lastValue == " ")
                    {
                        logError(_logger, "A value in the States Array is NULL or empty String");
                        return false;
                    }
                    else
                    {
                        logDebugging(_logger, "States Array is correct");
                    }
                }
                else
                {
                    if (states->value == NULL || states->value == " ")
                    {
                        logError(_logger, "A value in the States Array is NULL or empty String");
                        return false;
                    }
                    else
                    {
                        logDebugging(_logger, "States Array is is correct");
                        states = states->next;
                    }
                }
            } while (!states->isLast);
            if (amountOfStates != amountOfColors)
            {
                logError(_logger, "The amount of colors differ from the amount of states.");
                return false;
            }
        }
    }
    // else if (option == NEIGHBORHOOD_OPTION) // Vemos que el NEIGHBORHOOD_OPTION sea valido
    // {
    //     if (option->neighborhoodEnum == CUSTOM ||
    //         option->neighborhoodEnum == MOORE ||
    //         option->neighborhoodEnum == VON_NEUMANN ||
    //         option->neighborhoodEnum == K_NEIGHBORHOOD)
    //     {
    //         logDebugging(_logger, "Configuration option Frontier is valid with type");
    //     }
    // }
    return true;
}

static bool validateDefaultConfiguration(Configuration *configuration)
{
}
static bool validateTransitionConfiguration(Configuration *configuration)
{
}
static bool validateNeighborhoodConfiguration(Configuration *configuration)
{
}

SemanticAnalysisStatus checkSemantic(Program *program, Logger *logger)
{
    logDebugging(logger, "Checking semantic of the program...");

    if (program == NULL)
    {
        logError(logger, "The program or its expression is NULL.");
        return SEMANTIC_FAILURE;
    }

    switch (program->type)
    {
    case DEFAULT:
        logDebugging(_logger, "The program is DEFAULT program");
        if (validateConfiguration(program->justConfiguration))
        {
            if (validateDefaultConfiguration(program->justConfiguration))
            {
                logError(_logger, "Configuration of Default program is correct");
                return SEMANTIC_SUCCESS;
            }
            else
            {
                logError(_logger, "Configuration of Neighborhood program is incorrect");
                return SEMANTIC_FAILURE;
            }
        }
    case TRANSITION:
        logDebugging(_logger, "The program is a TRANSITION program");
        if (validateConfiguration(program->configuration))
        {
            if (validateTransitionConfiguration(program->justConfiguration))
            {
                logError(_logger, "Configuration of Transition program is correct");
                return SEMANTIC_SUCCESS;
            }
            else
            {
                logError(_logger, "Configuration of Neighborhood program is incorrect");
                return SEMANTIC_FAILURE;
            }
        }
    case NEIGHBORHOOD_PROGRAM:
        logDebugging(_logger, "The program is a NEIGHBORHOOD_PROGRAM");
        if (validateConfiguration(program->configuration))
        {
            if (validateNeighborhoodConfiguration(program->justConfiguration))
            {
                logError(_logger, "Configuration of Neighborhood program is correct");
                return SEMANTIC_SUCCESS;
            }
            else
            {
                logError(_logger, "Configuration of Neighborhood program is incorrect");
                return SEMANTIC_FAILURE;
            }
        }
    default:
        logError(logger, "Unknown program type.");
        return SEMANTIC_FAILURE;
    }
}