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
		if (constant->type == STRING_C) {
			free(constant->string);
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
				releaseArithmeticExpression(expression->expression);
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
	if (program != NULL) {
		switch (program->type) {
			case DEFAULT:
				releaseConfiguration(program->justConfiguration);
				break;
			case TRANSITION:
				releaseConfiguration(program->configuration);
				releaseTransitionSequence(program->transitionSequence);
				break;
			case NEIGHBORHOOD_PROGRAM:
				releaseConfiguration(program->configuration);
				releaseNeighborhoodSequence(program->neighborhoodSequence);
				break;
		}
		free(program);
	}
}

void releaseConfiguration(Configuration * configuration) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (configuration != NULL) {
		if (configuration->isLast) {
			releaseOption(configuration->lastOption);
		} else {
			releaseOption(configuration->option);
			releaseConfiguration(configuration->next);
		}
		free(configuration);
	}
}


void releaseOption(Option * option) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (option != NULL) {
		switch (option->type) {
			case COLORS_OPTION:
				releaseIntArray(option->colors);
				break;
			case STATES_OPTION:
				releaseStringArray(option->states);
				break;
			case EVOLUTION_OPTION:
				releaseEvolution(option->evolution);
				break;
			default:
				break;
		}
		free(option);
	}
}

void releaseEvolution(Evolution * evolution) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (evolution != NULL) {
		if (!evolution->isDefault) {
			releaseIntArray(evolution->array);
		}
		free(evolution);
	}
}

void releaseIntArray(IntArray * array) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (array != NULL) {
		if (!array->isLast) {
			releaseIntArray(array->next);
		}
		free(array);
	}
}

void releaseStringArray(StringArray * array) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (array != NULL) {
		if (array->isLast) {
			free(array->lastValue);
		} else {
			free(array->value);
			releaseStringArray(array->next);
		}
		free(array);
	}
}

void releaseConstantArray(ConstantArray * array) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (array != NULL) {
		if (array->isLast) {
			releaseConstant(array->lastValue);
		} else {
			releaseConstant(array->value);
			releaseConstantArray(array->next);
		}
		free(array);
	}
}

void releaseCell(Cell * cell) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (cell != NULL) {
		if (cell->isSingleCoordenate) {
			releaseConstant(cell->displacement);
		} else {
			releaseConstant(cell->x);
			releaseConstant(cell->y);
		}
		free(cell);
	}
}

void releaseCellList(CellList * list) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (list != NULL) {
		if (list->isLast) {
			releaseCell(list->last);
		} else {
			releaseCell(list->cell);
			releaseCellList(list->next);
		}
		free(list);
	}
}

void releaseRange(Range * range) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (range != NULL) {
		switch (range->type) {
			case ARRAY:
				releaseConstantArray(range->array);
				break;
			case INTERVAL:
				releaseConstant(range->start);
				releaseConstant(range->end);
				break;
		}
		free(range);
	}
}

void releaseNeighborhoodSequence(NeighborhoodSequence * sequence) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (sequence != NULL) {
		if (sequence->binary) {
			releaseNeighborhoodSequence(sequence->sequence);
			releaseNeighborhoodExpression(sequence->rightExpression);
		} else {
			releaseNeighborhoodExpression(sequence->expression);
		}
		free(sequence);
	}
}

void releaseTransitionSequence(TransitionSequence * sequence) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (sequence != NULL) {
		if (sequence->binary) {
			releaseTransitionSequence(sequence->sequence);
			releaseTransitionExpression(sequence->rightExpression);
		} else {
			releaseTransitionExpression(sequence->expression);
		}
		free(sequence);
	}
}

void releaseNeighborhoodExpression(NeighborhoodExpression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case NEIGHBORHOOD_FOR_LOOP:
				releaseRange(expression->range);
				releaseNeighborhoodSequence(expression->forBody);
				break;
			case NEIGHBORHOOD_IF:
				releaseArithmeticExpression(expression->ifCondition);
				releaseNeighborhoodSequence(expression->ifBody);
				break;
			case NEIGHBORHOOD_IF_ELSE:
				releaseArithmeticExpression(expression->ifElseCondition);
				releaseNeighborhoodSequence(expression->ifElseIfBody);
				releaseNeighborhoodSequence(expression->ifElseElseBody);
				break;
			case NEIGHBORHOOD_ASSIGNMENT:
				releaseArithmeticExpression(expression->assignment);
				free(expression->variable);
				break;
			case ADD_CELL_EXP:
				releaseCellList(expression->toAddList);
				break;
			case REMOVE_CELL_EXP:
				releaseCellList(expression->toRemoveList);
				break;
		}
		free(expression);
	}
}

void releaseTransitionExpression(TransitionExpression * expression) {
	logDebugging(_logger, "Executing destructor: %s", __FUNCTION__);
	if (expression != NULL) {
		switch (expression->type) {
			case TRANSITION_FOR_LOOP:
				releaseRange(expression->range);
				releaseTransitionSequence(expression->forBody);
				break;
			case TRANSITION_IF:
				releaseArithmeticExpression(expression->ifCondition);
				releaseTransitionSequence(expression->ifBody);
				break;
			case TRANSITION_IF_ELSE:
				releaseArithmeticExpression(expression->ifElseCondition);
				releaseTransitionSequence(expression->ifElseIfBody);
				releaseTransitionSequence(expression->ifElseElseBody);
				break;
			case TRANSITION_ASSIGNMENT:
				releaseArithmeticExpression(expression->assignment);
				free(expression->variable);
				break;
			case RETURN_VALUE:
				releaseArithmeticExpression(expression->returnValue);
				break;
		}
		free(expression);
	}
}

