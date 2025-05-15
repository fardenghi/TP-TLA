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

Program * DefaultProgramSemanticAction(CompilerState * compilerState, Configuration * justConfiguration);
Program * TransitionProgramSemanticAction(CompilerState * compilerState, Configuration * configuration, TransitionExpression * transitionExpression);
Program * NeighborhoodProgramSemanticAction(CompilerState * compilerState, Configuration * configuration, NeighborhoodExpression * neigborhoodExpression);

Configuration * ConfigurationSemanticAction(Option * option, Configuration * config);

Option * IntValuedOptionSemanticAction(const int value, OptionType type);
Option * IntArrayValuedOptionSemanticAction(IntArray * value);
Option * StringArrayValuedOptionSemanticAction(StringArray * value);
Option * FrontierOptionSemanticAction(const FrontierEnum value);
Option * NeighborhoodOptionSemanticAction(const NeighborhoodEnum value);
Option * EvolutionOptionSemanticAction(Evolution * value);

Evolution * EvolutionSemanticAction(IntArray * array, const int value, const EvolutionEnum type);

TransitionExpression * TransitionExpressionSemanticAction();
NeighborhoodExpression * NeighborhoodExpressionSemanticAction();

Cell * SingleCoordinateCellSemanticAction(Constant * c, DisplacementType type);
Cell * DoubleCoordinateCellSemanticAction(Constant * x, Constant * y);
CellList * CellListSemanticAction(Cell * cell, CellList * list);

ArithmeticExpression * BinaryArithmeticExpressionSemanticAction(ArithmeticExpression * leftExpression, ArithmeticExpression * rightExpression, ArithmeticExpressionType type);
ArithmeticExpression * UnaryArithmeticExpressionSemanticAction(ArithmeticExpression * expression, ArithmeticExpressionType type);
ArithmeticExpression * CellListArithmeticExpressionSemanticAction(CellList * cellList, ArithmeticExpressionType type);
ArithmeticExpression * ConstantArithmeticExpressionSemanticAction(Constant * cellList);

Constant * IntegerConstantSemanticAction(const int value);
Constant * StringConstantSemanticAction(const char * value);

IntArray * IntArraySemanticAction(const int value, IntArray * arr);
StringArray * StringArraySemanticAction(const char * value, StringArray * arr);
Range * RangeSemanticAction(IntArray * array, Constant * start, Constant * end);

#endif
