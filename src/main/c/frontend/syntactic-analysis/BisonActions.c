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

Program * DefaultProgramSemanticAction(CompilerState * compilerState, Option * justOptions) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->justOptions = justOptions;
	program->type = DEFAULT;
	compilerStateCheck(compilerState, program);
	return program;
}

Program * TransitionProgramSemanticAction(CompilerState * compilerState, Option * options, TransitionExpression * transitionExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->options = options;
	program->transitionExpression = transitionExpression;
	program->type = TRANSITION;
	compilerStateCheck(compilerState, program);
	return program;
}

Program * NeighborhoodProgramSemanticAction(CompilerState * compilerState, Option * options, NeighborhoodExpression * neigborhoodExpression) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Program * program = calloc(1, sizeof(Program));
	program->options = options;
	program->neighborhoodExpression = neigborhoodExpression;
	program->type = NEIGHBORHOOD;
	compilerStateCheck(compilerState, program);
	return program;
}

Constant * IntegerConstantSemanticAction(const int value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	constant->value = value;
	return constant;
}

Constant * StringConstantSemanticAction(const char * value) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	Constant * constant = calloc(1, sizeof(Constant));
	char * string = malloc(strlen(value) + 1);
	strcpy(string, value);
	constant->string = string;
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

ArithmeticExpression * UnaryArithmeticExpressionSemanticAction(ArithmeticExpression * expression, ArithmeticExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticExpression * expression = calloc(1, sizeof(ArithmeticExpression));
	expression->expression = expression;
	expression->type = type;
	return expression;
}

ArithmeticExpression * CellListArithmeticExpressionSemanticAction(CellList * cellList, ArithmeticExpressionType type) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticExpression * expression = calloc(1, sizeof(ArithmeticExpression));
	expression->cellList = cellList;
	expression->type = type;
	return expression;
}

ArithmeticExpression * ConstantArithmeticExpressionSemanticAction(Constant * constant) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	ArithmeticExpression * expression = calloc(1, sizeof(ArithmeticExpression));
	expression->cellList = constant;
	expression->type = CONSTANT;
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
StringArray * StringArraySemanticAction(const char * value, StringArray * arr) {
	_logSyntacticAnalyzerAction(__FUNCTION__);
	IntArray * intArray = calloc(1, sizeof(StringArray));
	char * string = malloc(strlen(value) + 1);
	strcpy(string, value);
	if (arr == NULL) {
		intArray->isLast = 1;
		intArray->lastValue = string;
	} else {
		intArray->isLast = 0;
		intArray->value = string;
		intArray->next = arr;
	}
	return intArray;
}
