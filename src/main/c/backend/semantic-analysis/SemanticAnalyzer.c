#include "SemanticAnalyzer.h"
#include "../../shared/Logger.h"
#include <stdbool.h>

static Logger *_logger = NULL;
int amountOfColors = 0;
int amountOfStates = 0;

void initializeSemanticAnalyzerModule();
void shutdownSemanticAnalyzerModule();
static bool validateConfiguration(Configuration *configuration);
static bool validateConfigurationRec(Configuration *configuration);
static bool validateConfigurationAux(Option *option);
static Option *getOption(Configuration *configuration, OptionType type);
static bool validateDefaultConfiguration(Configuration *configuration);
static bool validateTransitionConfiguration(Configuration *configuration);
static bool validateNeighborhoodConfiguration(Configuration *configuration);
static int getBiggerNumber(IntArray *intArray);
static int getAmountElems(IntArray *option);
SemanticAnalysisStatus checkSemantic(Program *program, Logger *logger);

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

static int getBiggerNumber(IntArray *array)
{
    if (array == NULL)
    {
        return 0;
    }

    int maxValue = array->isLast ? array->lastValue : array->value;

    while (!array->isLast)
    {
        if (array->value > maxValue)
        {
            maxValue = array->value;
        }
        array = array->next;
    }

    if (array->isLast && array->lastValue > maxValue)
    {
        maxValue = array->lastValue;
    }

    return maxValue;
}

static bool validateConfiguration(Configuration *configuration)
{
    if (configuration == NULL)
    {
        logError(_logger, "Semantic Error: The configuration is NULL. A valid configuration is required.");
        return false;
    }

    int obligatoryTypes[5] = {0};
    int i = 0;
    Configuration *current = configuration;

    do
    {
        if (i > 0)
        {
            current = current->next;
        }

        if ((!current->isLast && current->option->type < 5) || current->lastOption->type < 5)
        {
            if (!current->isLast)
            {
                if (!obligatoryTypes[current->option->type])
                {
                    obligatoryTypes[current->option->type] = 1;
                }
                else
                {
                    logError(_logger, "Semantic Error: A configuration option is repeated. Each option must be unique.");
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
                    logError(_logger, "Semantic Error: A configuration option is repeated. Each option must be unique.");
                    return false;
                }
            }
        }
        i++;
    } while (!current->isLast);

    if (i < 5)
    {
        logError(_logger, "Semantic Error: The configuration is missing obligatory options. Ensure all required options are defined.");
        return false;
    }

    bool rta = validateConfigurationRec(configuration);
    if (amountOfStates != amountOfColors)
    {
        logCritical(_logger, "%d,%d", amountOfColors, amountOfStates);
        logError(_logger, "Semantic Error: The number of states does not match the number of colors. Ensure both lists are consistent.");
        return false;
    }
    return rta;
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
        logDebugging(_logger, "Semantic Info: Configuration validation reached the last option.");
        return true;
    }
    else
    {
        return validateConfigurationRec(configuration->next);
    }
}

static bool validateConfigurationAux(Option *option)
{
    if (option->type == WIDTH_OPTION || option->type == HEIGHT_OPTION)
    {
        if (option->value <= 0 || option->value > 300)
        {
            logError(_logger, "Semantic Error: The %s option must have a value greater than 0.",
                     option->type == WIDTH_OPTION ? "Width" : "Height");
            return false;
        }
        else
        {
            logDebugging(_logger, "Semantic Info: The %s option is valid with value %d.",
                         option->type == WIDTH_OPTION ? "Width" : "Height",
                         option->value);
        }
    }
    else if (option->type == FRONTIER_OPTION)
    {
        if (option->frontierType == PERIODIC ||
            option->frontierType == OPEN ||
            option->frontierType == MIRROR)
        {
            logDebugging(_logger, "Semantic Info: The Frontier option is valid with type %d.", option->frontierType);
        }
        else
        {
            logError(_logger, "Semantic Error: The Frontier option has an invalid type. Valid types are PERIODIC, OPEN, or MIRROR.");
            return false;
        }
    }
    else if (option->type == COLORS_OPTION)
    {
        if (option->colors == NULL)
        {
            logError(_logger, "Semantic Error: The COLORS option is NULL. A valid list of colors is required.");
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
                        logError(_logger, "Semantic Error: The COLORS option contains a negative value. All values must be non-negative.");
                        return false;
                    }
                    else
                    {
                        logDebugging(_logger, "Semantic Info: The COLORS option is valid.");
                    }
                }
                else
                {
                    if (colors->value < 0)
                    {
                        logError(_logger, "Semantic Error: The COLORS option contains a negative value. All values must be non-negative.");
                        return false;
                    }
                    else
                    {
                        logDebugging(_logger, "Semantic Info: The COLORS option is valid.");
                        colors = colors->next;
                    }
                }
            } while (!colors->isLast);
        }
    }
    else if (option->type == STATES_OPTION)
    {
        StringArray *states = option->states;
        if (states == NULL)
        {
            logError(_logger, "Semantic Error: The STATES option is NULL. A valid list of states is required.");
            return false;
        }
        else
        {
            do
            {
                amountOfStates++;
                if (states->isLast)
                {
                    if (states->lastValue == NULL || strcmp(states->lastValue, " ") == 0)
                    {
                        logError(_logger, "Semantic Error: A value in the STATES array is NULL or an empty string. All states must be valid.");
                        return false;
                    }
                    else
                    {
                        logDebugging(_logger, "Semantic Info: The STATES array is valid.");
                    }
                }
                else
                {
                    if (states->value == NULL || strcmp(states->value, " ") == 0)
                    {
                        logError(_logger, "Semantic Error: A value in the STATES array is NULL or an empty string. All states must be valid.");
                        return false;
                    }
                    else
                    {
                        logDebugging(_logger, "Semantic Info: The STATES array is valid.");
                        states = states->next;
                    }
                }
            } while (!states->isLast);
        }
    }
    return true;
}

static Option *getOption(Configuration *configuration, OptionType type)
{
    if (configuration == NULL)
    {
        return NULL;
    }

    if (configuration->isLast)
    {
        if (configuration->lastOption->type == type)
        {
            return configuration->lastOption;
        }
        else
        {
            return NULL;
        }
    }
    else
    {
        if (configuration->option->type == type)
        {
            return configuration->option;
        }
        else
        {
            return getOption(configuration->next, type);
        }
    }
}

static bool validateDefaultConfiguration(Configuration *configuration)
{
    Option *neigh = getOption(configuration, NEIGHBORHOOD_OPTION);
    Option *evol = getOption(configuration, EVOLUTION_OPTION);

    if (neigh == NULL || evol == NULL)
    {
        logError(_logger, "Semantic Error: Default configuration is missing NEIGHBORHOOD or EVOLUTION options.");
        return false;
    }

    if (neigh->neighborhoodEnum == CUSTOM)
    {
        logError(_logger, "Semantic Error: Default configuration cannot use a CUSTOM neighborhood.");
        return false;
    }

    if (neigh->neighborhoodEnum == VON_NEUMANN)
    {

        if (getBiggerNumber(evol->evolution->birthArray) > 4 && getBiggerNumber(evol->evolution->surviveArray) > 4)
        {
            logError(_logger, "Semantic Error: Evolution states exceed Neighborhood capacity.");
            return false;
        }
    }
    else if (neigh->neighborhoodEnum == MOORE)
    {
        if (getBiggerNumber(evol->evolution->birthArray) > 8 && getBiggerNumber(evol->evolution->surviveArray) > 8)
        {
            logError(_logger, "Semantic Error: Evolution states exceed Neighborhood capacity.");
            return false;
        }
    }

    if (evol->evolution->isDefault && getAmountElems(getOption(configuration, COLORS_OPTION)->colors) > 2)
    {
        logError(_logger, "Semantic Error: Evolution states exceed Neighborhood capacity.");
        return false;
    }
    return true;
}

static int getAmountElems(IntArray *array)
{
    if (array == NULL)
    {
        return 0;
    }
    IntArray *current = array;
    int i = 0;
    while (!current->isLast)
    {
        i++;
        current = current->next;
    }
    i++;
    return i;
}

static bool validateTransitionConfiguration(Configuration *configuration)
{
    Option *neigh = getOption(configuration, NEIGHBORHOOD_OPTION);
    Option *evol = getOption(configuration, EVOLUTION_OPTION);

    if (neigh != NULL || evol != NULL)
    {
        logError(_logger, "Semantic Error: Transition configuration cannot include NEIGHBORHOOD or EVOLUTION options.");
        return false;
    }
    return true;
}

static bool validateNeighborhoodConfiguration(Configuration *configuration)
{
    Option *neigh = getOption(configuration, NEIGHBORHOOD_OPTION);
    Option *evol = getOption(configuration, EVOLUTION_OPTION);

    if (neigh == NULL || evol == NULL)
    {
        logError(_logger, "Semantic Error: Neighborhood configuration is missing NEIGHBORHOOD or EVOLUTION options.");
        return false;
    }
    else if (neigh->neighborhoodEnum != CUSTOM)
    {
        logError(_logger, "Semantic Error: Neighborhood configuration must use a CUSTOM neighborhood.");
        return false;
    }

    return true;
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
                logDebugging(_logger, "Configuration of Default program is correct");
                return SEMANTIC_SUCCESS;
            }
            else
            {
                logError(_logger, "Configuration of Neighborhood program is incorrect");
                return SEMANTIC_FAILURE;
            }
        }
        break;
    case TRANSITION:
        logDebugging(_logger, "The program is a TRANSITION program");
        if (validateConfiguration(program->configuration))
        {
            if (validateTransitionConfiguration(program->justConfiguration))
            {
                logDebugging(_logger, "Configuration of Transition program is correct");
                return SEMANTIC_SUCCESS;
            }
            else
            {
                logError(_logger, "Configuration of Neighborhood program is incorrect");
                return SEMANTIC_FAILURE;
            }
        }
        break;
    case NEIGHBORHOOD_PROGRAM:
        logDebugging(_logger, "The program is a NEIGHBORHOOD_PROGRAM");
        if (validateConfiguration(program->configuration))
        {
            if (validateNeighborhoodConfiguration(program->justConfiguration))
            {
                logDebugging(_logger, "Configuration of Neighborhood program is correct");
                return SEMANTIC_SUCCESS;
            }
            else
            {
                logError(_logger, "Configuration of Neighborhood program is incorrect");
                return SEMANTIC_FAILURE;
            }
        }
        break;
    default:
        logError(logger, "Unknown program type.");
    }
    return SEMANTIC_FAILURE;
}