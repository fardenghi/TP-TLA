#ifndef SEMANTIC_ANALYZER_HEADER
#define SEMANTIC_ANALYZER_HEADER
#include "../../shared/Logger.h"
#include "../../shared/CompilerState.h"
#include "../../syntactic-analysis/AbstractSyntaxTree.h"
#include "../../syntactic-analysis/SyntacticAnalyzer.h"

typedef enum {
    SEMANTIC_SUCCESS,
    SEMANTIC_FAILURE
}SemanticAnalysisStatus;

void initializeSemanticAnalyzerModule();
void shutdownSemanticAnalyzerModule();

SemanticAnalysisStatus checkSemantic(Program * program,Logger * logger);

#endif