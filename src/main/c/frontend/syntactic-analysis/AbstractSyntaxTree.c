#include "AbstractSyntaxTree.h"

/* MODULE INTERNAL STATE */

static Logger * _logger = NULL;

void initializeAbstractSyntaxTreeModule() {
	_logger = createLogger("AbstractSyntxTree");
}

void shutdownAbstractSyntaxTreeModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PUBLIC FUNCTIONS */

void releaseConstant(Constant * constant) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (constant != NULL) {
		if (constant->type == STRING) {
			free(constant->value);
		}
		free(constant);
	}
}

void releaseArithmeticExpression(ArithmeticExpression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		if (expression->type < LOGIC_NOT) {
				releaseArithmeticExpression(expression->leftExpression);
				releaseArithmeticExpression(expression->rightExpression);
		} else {
			switch (expression->type) {
			case ALL_ARE:
			case ANY_ARE:
			case AT_LEAST_ARE:
				releaseCellList(expression->cellList);
				break;
			case FACTOR:
				releaseExpression(expression->expression);
				break;
			default:
				releaseConstant(expression->constant);
				break;
			}
		}
		free(expression);
	}
}

void releaseProgram(Program * program) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	
}
