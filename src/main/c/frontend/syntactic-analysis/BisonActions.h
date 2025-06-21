#ifndef BISON_ACTIONS_HEADER
#define BISON_ACTIONS_HEADER

#include "../../shared/CompilerState.h"
#include "../../shared/Logger.h"
#include "../../shared/Type.h"
#include "AbstractSyntaxTree.h"
#include "SyntacticAnalyzer.h"
#include <stdlib.h>
#include "../../shared/SymbolTable.h"

/** Initialize module's internal state. */
void initializeBisonActionsModule();

/** Shutdown module's internal state. */
void shutdownBisonActionsModule();

/**
 * Bison semantic actions.
 */

Program * DefaultProgramSemanticAction(CompilerState * compilerState, Configuration * justConfiguration);
Program * TransitionProgramSemanticAction(CompilerState * compilerState, Configuration * configuration, TransitionSequence * transitionSequence);
Program * NeighborhoodProgramSemanticAction(CompilerState * compilerState, Configuration * configuration, NeighborhoodSequence * neigborhoodSequence);

Configuration * ConfigurationSemanticAction(Option * option, Configuration * config);

Option * IntValuedOptionSemanticAction(const int value, OptionType type);
Option * IntArrayValuedOptionSemanticAction(IntArray * value);
Option * StringArrayValuedOptionSemanticAction(StringArray * value);
Option * FrontierOptionSemanticAction(const FrontierEnum value);
Option * NeighborhoodOptionSemanticAction(const NeighborhoodEnum value);
Option * EvolutionOptionSemanticAction(Evolution * value);

Evolution * EvolutionSemanticAction(IntArray * array, const int value, const EvolutionEnum type);

TransitionSequence * TransitionUnarySequenceSemanticAction(TransitionExpression * expression);
TransitionSequence * TransitionBinarySequenceSemanticAction(TransitionSequence * sequence, TransitionExpression * expression);

TransitionExpression * TransitionAssignmentExpressionSemanticAction(char * variable, ArithmeticExpression * arithmeticExpression);
TransitionExpression * TransitionForLoopExpressionSemanticAction(char * variable, Range * range, TransitionSequence * transitionExpression);
TransitionExpression * TransitionIfExpressionSemanticAction(ArithmeticExpression * arithmeticExpression, TransitionSequence * transitionExpression);
TransitionExpression * TransitionIfElseExpressionSemanticAction(ArithmeticExpression * arithmeticExpression, TransitionSequence * ifTransitionExpression, TransitionSequence * elseTransitionExpression);
TransitionExpression * TransitionReturnExpressionSemanticAction(ArithmeticExpression * arithmeticExpression);

NeighborhoodSequence * NeighborhoodUnarySequenceSemanticAction(NeighborhoodExpression * expression);
NeighborhoodSequence * NeighborhoodBinarySequenceSemanticAction(NeighborhoodSequence * sequence, NeighborhoodExpression * expression);

NeighborhoodExpression * NeighborhoodAssignmentExpressionSemanticAction(char * variable, ArithmeticExpression * arithmeticExpression);
NeighborhoodExpression * NeighborhoodForLoopExpressionSemanticAction(char * variable, Range * range, NeighborhoodSequence * neighborhoodExpression);
NeighborhoodExpression * NeighborhoodIfExpressionSemanticAction(ArithmeticExpression * arithmeticExpression, NeighborhoodSequence * neighborhoodExpression);
NeighborhoodExpression * NeighborhoodIfElseExpressionSemanticAction(ArithmeticExpression * arithmeticExpression, NeighborhoodSequence * ifNeighborhoodExpression, NeighborhoodSequence * elseNeighborhoodExpression);
NeighborhoodExpression * NeighborhoodCellExpressionSemanticAction(boolean add, CellList * cellList);


Cell * SingleCoordinateCellSemanticAction(Constant * c, DisplacementType type);
Cell * DoubleCoordinateCellSemanticAction(Constant * x, Constant * y);
CellList * CellListSemanticAction(Cell * cell, CellList * list);

ArithmeticExpression * BinaryArithmeticExpressionSemanticAction(ArithmeticExpression * leftExpression, ArithmeticExpression * rightExpression, ArithmeticExpressionType type);
ArithmeticExpression * UnaryArithmeticExpressionSemanticAction(ArithmeticExpression * expression, ArithmeticExpressionType type);
ArithmeticExpression * CellListArithmeticExpressionSemanticAction(CellList * cellList, ArithmeticExpressionType type, int count, char * state);
ArithmeticExpression * ConstantArithmeticExpressionSemanticAction(Constant * cellList);
ArithmeticExpression * CellArithmeticExpressionSemanticAction(Cell * cell);

Constant * IntegerConstantSemanticAction(const int value);
Constant * StringConstantSemanticAction(char * value);

IntArray * IntArraySemanticAction(const int value, IntArray * arr);
StringArray * StringArraySemanticAction(char * value, StringArray * arr);
ConstantArray * ConstantArraySemanticAction(Constant * value, ConstantArray * arr);
Range * RangeSemanticAction(ConstantArray * array, Constant * start, Constant * end);

#endif
