#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

Program * DefaultProgramSemanticAction(CompilerState * compilerState, Option * finalOptions);
Program * TransitionProgramSemanticAction(CompilerState * compilerState, Option * options, TransitionExpression * transitionExpression);
Program * NeighborhoodProgramSemanticAction(CompilerState * compilerState, Option * options, NeighborhoodExpression * neigborhoodExpression);

Constant * IntegerConstantSemanticAction(const int value);
Constant * StringConstantSemanticAction(const char * value);
ArithmeticExpression * BinaryArithmeticExpressionSemanticAction(ArithmeticExpression * leftExpression, ArithmeticExpression * rightExpression, ArithmeticExpressionType type);
ArithmeticExpression * UnaryArithmeticExpressionSemanticAction(ArithmeticExpression * expression, ArithmeticExpressionType type);
ArithmeticExpression * CellListArithmeticExpressionSemanticAction(CellList * cellList, ArithmeticExpressionType type);
ArithmeticExpression * ConstantArithmeticExpressionSemanticAction(Constant * cellList);

IntArray * IntArraySemanticAction(const int value, IntArray * arr);
StringArray * StringArraySemanticAction(const char * value, StringArray * arr);

#endif
