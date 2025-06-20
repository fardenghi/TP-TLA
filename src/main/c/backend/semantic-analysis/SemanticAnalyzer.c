#include "SemanticAnalyzer.h"
#include "../../shared/Logger.h"
#include <stdbool.h>

static Logger * _logger = NULL;

void initializeSemanticAnalyzerModule() {
    _logger = createLogger("SemanticAnalyzer");
}

void shutdownSemanticAnalyzerModule() {
    if (_logger != NULL) {
        destroyLogger(_logger);
    }
}

SemanticAnalysisStatus checkSemantic(Program * program, Logger * logger){
    logDebugging(logger, "Checking semantic of the program...");

    //Preguntar si este hace falta.
    if(program == NULL || program->expression == NULL) {
        logError(logger, "The program or its expression is NULL.");
        return SEMANTIC_FAILURE;
    }

    program

}