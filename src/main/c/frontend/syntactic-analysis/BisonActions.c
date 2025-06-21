#include "BisonActions.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeBisonActionsModule() {
	_logger = createLogger("BisonActions");
}

void shutdownBisonActionsModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** IMPORTED FUNCTIONS */

extern unsigned int flexCurrentContext(void);

/* PRIVATE FUNCTIONS */

static void _logSyntacticAnalyzerAction(const char * functionName);

/**
 * Logs a syntactic-analyzer action in DEBUGGING level.
 */
static void _logSyntacticAnalyzerAction(const char * functionName) {
	logDebugging(_logger, "%s", functionName);
}

/* PUBLIC FUNCTIONS */

void compilerStateCheck(CompilerState * compilerState, Program * program) {
	compilerState->abstractSyntaxtTree = program;
	if (0 < flexCurrentContext()) {
		logError(_logger, "The final context is not the default (0): %d", flexCurrentContext());
		compilerState->succeed = false;
	}
	else {
		compilerState->succeed = true;
	}
}

Program * DefaultProgramSemanticAction(CompilerState * compilerState, Configuration * justConfiguration) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->justConfiguration = justConfiguration;
	program->type = DEFAULT;
	compilerStateCheck(compilerState, program);
	return program;
}

Program * TransitionProgramSemanticAction(CompilerState * compilerState, Configuration * configuration, TransitionSequence * transitionSequence) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->configuration = configuration;
	program->transitionSequence = transitionSequence;
	program->type = TRANSITION;
	compilerStateCheck(compilerState, program);
	return program;
}

Program * NeighborhoodProgramSemanticAction(CompilerState * compilerState, Configuration * configuration, NeighborhoodSequence * neigborhoodSequence) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->configuration = configuration;
	program->neighborhoodSequence = neigborhoodSequence;
	program->type = NEIGHBORHOOD_PROGRAM;
	compilerStateCheck(compilerState, program);
	return program;
}

Configuration * ConfigurationSemanticAction(Option * option, Configuration * config) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Configuration * configuration = calloc(1, sizeof(Configuration));
	if (config == NULL) {
		configuration->isLast = 1;
		configuration->lastOption = option;
	} else {
		configuration->isLast = 0;
		configuration->option = option;
		configuration->next = config;
	}
	return configuration;
}

TransitionSequence * TransitionBinarySequenceSemanticAction(TransitionSequence * sequence, TransitionExpression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TransitionSequence * rta = calloc(1, sizeof(TransitionSequence));
	rta->sequence = sequence;
	rta->rightExpression = expression;
	return rta;
}

TransitionExpression * TransitionAssignmentExpressionSemanticAction(char * variable, ArithmeticExpression * arithmeticExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TransitionExpression * expression = calloc(1, sizeof(TransitionExpression));
	expression->type = TRANSITION_ASSIGNMENT;
	expression->variable = variable;
	expression->assignment = arithmeticExpression;
	return expression;
}
TransitionExpression * TransitionForLoopExpressionSemanticAction(char * variable, Range * range, TransitionSequence * transitionExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TransitionExpression * expression = calloc(1, sizeof(TransitionExpression));
	expression->type = TRANSITION_FOR_LOOP;
	expression->forVariable = variable;
	expression->range = range;
	expression->forBody = transitionExpression;
	return expression;
}
TransitionExpression * TransitionIfExpressionSemanticAction(ArithmeticExpression * arithmeticExpression, TransitionSequence * transitionExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TransitionExpression * expression = calloc(1, sizeof(TransitionExpression));
	expression->type = TRANSITION_IF;
	expression->ifCondition = arithmeticExpression;
	expression->ifBody = transitionExpression;
	return expression;
}
TransitionExpression * TransitionIfElseExpressionSemanticAction(ArithmeticExpression * arithmeticExpression, TransitionSequence * ifTransitionExpression, TransitionSequence * elseTransitionExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TransitionExpression * expression = calloc(1, sizeof(TransitionExpression));
	expression->type = TRANSITION_IF_ELSE;
	expression->ifElseCondition = arithmeticExpression;
	expression->ifElseIfBody = ifTransitionExpression;
	expression->ifElseElseBody = elseTransitionExpression;
	return expression;
}
TransitionExpression * TransitionReturnExpressionSemanticAction(ArithmeticExpression * arithmeticExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	TransitionExpression * expression = calloc(1, sizeof(TransitionExpression));
	expression->type = RETURN_VALUE;
	expression->returnValue = arithmeticExpression;
	return expression;
}

NeighborhoodSequence * NeighborhoodBinarySequenceSemanticAction(NeighborhoodSequence * sequence, NeighborhoodExpression * expression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NeighborhoodSequence * rta = calloc(1, sizeof(NeighborhoodSequence));
	rta->sequence = sequence;
	rta->rightExpression = expression;
	return rta;
}

NeighborhoodExpression * NeighborhoodAssignmentExpressionSemanticAction(char * variable, ArithmeticExpression * arithmeticExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NeighborhoodExpression * expression = calloc(1, sizeof(NeighborhoodExpression));
	expression->type = NEIGHBORHOOD_ASSIGNMENT;
	expression->variable = variable;
	expression->assignment = arithmeticExpression;
	return expression;
}
NeighborhoodExpression * NeighborhoodForLoopExpressionSemanticAction(char * variable, Range * range, NeighborhoodSequence * neighborhoodExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NeighborhoodExpression * expression = calloc(1, sizeof(NeighborhoodExpression));
	expression->type = NEIGHBORHOOD_FOR_LOOP;
	expression->forVariable = variable;
	expression->range = range;
	expression->forBody = neighborhoodExpression;
	return expression;
}
NeighborhoodExpression * NeighborhoodIfExpressionSemanticAction(ArithmeticExpression * arithmeticExpression, NeighborhoodSequence * neighborhoodExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NeighborhoodExpression * expression = calloc(1, sizeof(NeighborhoodExpression));
	expression->type = NEIGHBORHOOD_IF;
	expression->ifCondition = arithmeticExpression;
	expression->ifBody = neighborhoodExpression;
	return expression;
}
NeighborhoodExpression * NeighborhoodIfElseExpressionSemanticAction(ArithmeticExpression * arithmeticExpression, NeighborhoodSequence * ifNeighborhoodExpression, NeighborhoodSequence * elseNeighborhoodExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NeighborhoodExpression * expression = calloc(1, sizeof(NeighborhoodExpression));
	expression->type = NEIGHBORHOOD_IF_ELSE;
	expression->ifElseCondition = arithmeticExpression;
	expression->ifElseIfBody = ifNeighborhoodExpression;
	expression->ifElseElseBody = elseNeighborhoodExpression;
	return expression;
}
NeighborhoodExpression * NeighborhoodCellExpressionSemanticAction(boolean add, CellList * cellList) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	NeighborhoodExpression * expression = calloc(1, sizeof(NeighborhoodExpression));
	if (add) {
		expression->type = ADD_CELL_EXP;
		expression->toAddList = cellList;
	} else {
		expression->type = REMOVE_CELL_EXP;
		expression->toRemoveList = cellList;
	}
	
	return expression;
}

Option * IntValuedOptionSemanticAction(const int value, OptionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Option * option = calloc(1, sizeof(Option));
	option->type = type;
	option->value = value;
	return option;
}
Option * IntArrayValuedOptionSemanticAction(IntArray * value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Option * option = calloc(1, sizeof(Option));
	option->type = COLORS_OPTION;
	option->colors = value;
	return option;
}
Option * StringArrayValuedOptionSemanticAction(StringArray * value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Option * option = calloc(1, sizeof(Option));
	option->type = STATES_OPTION;
	option->states = value;
	const CompilerState * compilerState = currentCompilerState();
	for (const StringArray * current = value; current != NULL; current = current->next) {
		if (current->isLast) break;
		if (current->value == NULL) {
			logError(_logger, "StringArrayValuedOptionSemanticAction: A string in the array is NULL.");
			free(option);
			return NULL;
		}
		insertReadOnlySymbol(compilerState->symbolTable, current->value);
	}
	return option;
}
Option * FrontierOptionSemanticAction(const FrontierEnum value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Option * option = calloc(1, sizeof(Option));
	option->type = FRONTIER_OPTION;
	option->frontierType = value;
	return option;
}
Option * NeighborhoodOptionSemanticAction(const NeighborhoodEnum value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Option * option = calloc(1, sizeof(Option));
	option->type = NEIGHBORHOOD_OPTION;
	option->neighborhoodEnum = value;
	return option;
}
Option * EvolutionOptionSemanticAction(Evolution * value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Option * option = calloc(1, sizeof(Option));
	option->type = EVOLUTION_OPTION;
	option->evolution = value;
	return option;
}

Evolution * EvolutionSemanticAction(IntArray * surviveArray, IntArray * birthArray, const EvolutionEnum type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Evolution * evolution = calloc(1, sizeof(Evolution));
	if (surviveArray != NULL && birthArray != NULL) {
		evolution->isDefault = false;
		evolution->surviveArray = surviveArray;
		evolution->birthArray = birthArray;
	} else {
		evolution->isDefault = true;
		evolution->evolutionTypes = type;
	}
	return evolution;
}



Cell * SingleCoordinateCellSemanticAction(Constant * c, DisplacementType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Cell * cell = calloc(1, sizeof(Cell));
	cell->isSingleCoordenate = true;
	cell->displacement = c;
	cell->displacementType = type;
	return cell;
}

Cell * DoubleCoordinateCellSemanticAction(Constant * x, Constant * y) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Cell * cell = calloc(1, sizeof(Cell));
	cell->isSingleCoordenate = false;
	cell->x = x;
	cell->y = y;
	return cell;
}

CellList * CellListSemanticAction(Cell * cell, CellList * list) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	CellList * cellList = calloc(1, sizeof(CellList));
	if (list == NULL) {
		cellList->isLast = true;
		cellList->last = cell;
	} else {
		cellList->isLast = false;
		cellList->cell = cell;
		cellList->next = list;
	}
	
	return cellList;
}

Constant * IntegerConstantSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->value = value;
	constant->type = INTEGER_C;
	return constant;
}

Constant * StringConstantSemanticAction(char * value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	char * string = malloc(strlen(value) + 1);
	strcpy(string, value);
	constant->string = string;
	constant->type = STRING_C;
	return constant;
}

ArithmeticExpression * BinaryArithmeticExpressionSemanticAction(ArithmeticExpression * leftExpression, ArithmeticExpression * rightExpression, ArithmeticExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticExpression * expression = calloc(1, sizeof(ArithmeticExpression));
	expression->leftExpression = leftExpression;
	expression->rightExpression = rightExpression;
	expression->type = type;
	return expression;
}

ArithmeticExpression * UnaryArithmeticExpressionSemanticAction(ArithmeticExpression * innerExpression, ArithmeticExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticExpression * expression = calloc(1, sizeof(ArithmeticExpression));
	expression->expression = innerExpression;
	expression->type = type;
	return expression;
}

ArithmeticExpression * CellListArithmeticExpressionSemanticAction(CellList * cellList, ArithmeticExpressionType type, int count, char * state) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticExpression * expression = calloc(1, sizeof(ArithmeticExpression));
	expression->cellList = cellList;
	expression->count = count;
	expression->type = type;
	expression->state = state;
	return expression;
}

ArithmeticExpression * ConstantArithmeticExpressionSemanticAction(Constant * constant) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticExpression * expression = calloc(1, sizeof(ArithmeticExpression));
	expression->constant = constant;
	expression->type = CONSTANT;
	return expression;
}

ArithmeticExpression * CellArithmeticExpressionSemanticAction(Cell * cell) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticExpression * expression = calloc(1, sizeof(ArithmeticExpression));
	expression->cell = cell;
	expression->type = CELL_ARITHETIC_EXPRESSION;
	return expression;
}


IntArray * IntArraySemanticAction(const int value, IntArray * arr) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	IntArray * intArray = calloc(1, sizeof(IntArray));
	if (arr == NULL) {
		intArray->isLast = 1;
		intArray->lastValue = value;
	} else {
		intArray->isLast = 0;
		intArray->value = value;
		intArray->next = arr;
	}
	return intArray;
}

StringArray * StringArraySemanticAction(char * value, StringArray * arr) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	StringArray * stringArray = calloc(1, sizeof(StringArray));
	char * string = malloc(strlen(value) + 1);
	strcpy(string, value);
	if (arr == NULL) {
		stringArray->isLast = 1;
		stringArray->lastValue = string;
	} else {
		stringArray->isLast = 0;
		stringArray->value = string;
		stringArray->next = arr;
	}
	return stringArray;
}

ConstantArray * ConstantArraySemanticAction(Constant * value, ConstantArray * arr) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ConstantArray * constantArray = calloc(1, sizeof(ConstantArray));
	if (arr == NULL) {
		constantArray->isLast = 1;
		constantArray->lastValue = value;
	} else {
		constantArray->isLast = 0;
		constantArray->value = value;
		constantArray->next = arr;
	}
	return constantArray;
}

Range * RangeSemanticAction(ConstantArray * array, Constant * start, Constant * end) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Range * range = calloc(1, sizeof(Range));
	if (array == NULL) {
		range->type = INTERVAL;
		range->start = start;
		range->end = end;
	} else {
		range->type = ARRAY;
		range->array = array;
	}
	return range;
}
