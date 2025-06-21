#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = '\t';
const char _indentationSize = 1;
static Logger * _logger = NULL;

void initializeGeneratorModule() {
	_logger = createLogger("Generator");
}

void shutdownGeneratorModule() {
	if (_logger != NULL) {
		destroyLogger(_logger);
	}
}

/** PRIVATE FUNCTIONS */

static char * _arithmeticExpressionTypeToString(const ArithmeticExpressionType type);
static void _generateConstant(const Constant * constant);
static void _generateEpilogue(const int value);
static void _generateExpression(const unsigned int indentationLevel, Expression * expression);
static void _generateFactor(const unsigned int indentationLevel, Factor * factor);
static void _generateProgram(Program * program);
static void _generatePrologue(void);
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);

/**
 * Converts and expression type to the proper character of the operation
 * involved, or returns '\0' if that's not possible.
 */
static char MAX_OPERAND_LENGTH = 16; 

static char * _arithmeticExpressionTypeToString(const ArithmeticExpressionType type) {
	char * operand = calloc(MAX_OPERAND_LENGTH, sizeof(char));
	switch (type) {
		case ADDITION: 
			operand[0] = '+';
			break;
		case DIVISION: 
			operand[0] = '/';
			break;
		case MULTIPLICATION: 	
			operand[0] = '*';
			break;
		case SUBTRACTION: 
			operand[0] = '-';
			break;
		case MODULE: 
			operand[0] = '%';
			break;
		case LOGIC_AND: 
			operand[0] = '&';
			operand[1] = '&';
			break;
		case LOGIC_OR:
			operand[0] = '|';
			operand[1] = '|';
			break;
		case EQUALS:
			operand[0] = '=';
			break;
		case NOT_EQUALS:
			operand[0] = '!';
			operand[1] = '=';
			break;
		case LOWER_THAN:
			operand[0] = '<';
			break;
		case LOWER_THAN_OR_EQUAL:
			operand[0] = '<';
			operand[1] = '=';
			break;
		case GREATER_THAN:
			operand[0] = '>';
			break;
		case GREATER_THAN_OR_EQUAL:
			operand[0] = '>';
			operand[1] = '=';
			break;
		case LOGIC_NOT:
			operand[0] = '!';
			break;
		case ALL_ARE:
			strcpy(operand, "all(");
			break;
		case ANY_ARE:
			strcpy(operand, "any(");
			break;
		case AT_LEAST_ARE:
			strcpy(operand, "sum(");
			break;
		case FACTOR:
		case CONSTANT:
		case CELL_ARITHETIC_EXPRESSION:
			break;
		default:
			logError(_logger, "The specified expression type cannot be converted into character: %d", type);
			return '\0';
	}
	return operand;
}

static void _generateTransitionExpression(unsigned int indentation, const TransitionExpression * transitionExpression) {
	switch (transitionExpression->type)
	{
		case TRANSITION_ASSIGNMENT:
			_output(indentation,"%s=", transitionExpression->variable);
			_generateArithmeticExpression(transitionExpression->assignment);
			break;
		case TRANSITION_FOR_LOOP:
			_output(indentation, "for %s in ", transitionExpression->forVariable);
			_generateRange(transitionExpression->range);
			_output(0, ":\n");
			_generateTransitionExpression(indentation + 1, transitionExpression->forBody);
			break;
		case TRANSITION_IF:
			_output(indentation, "if ");
			_generateArithmeticExpression(transitionExpression->ifCondition);
			_output(0, ":\n");
			_generateTransitionExpression(indentation + 1, transitionExpression->ifBody); // Esto es un transition sequence, llamo a transition secuence en vez de expression 
			break;
		case TRANSITION_IF_ELSE:
			_output(indentation, "if ");
			_generateArithmeticExpression(transitionExpression->ifElseCondition);
			_output(0, ":\n");
			_generateTransitionExpression(indentation + 1, transitionExpression->ifElseIfBody);
			_output(indentation, "\nelse:\n");
			_generateTransitionExpression(indentation + 1, transitionExpression->ifElseElseBody);
			break;
		case RETURN_VALUE:
			_output(indentation, "return ");
			_generateArithmeticExpression(transitionExpression->returnValue);
			break;
	default:
		break;
	}
}

static void _generateNeighborhoodExpression(unsigned int indentation, const NeighborhoodExpression * neighborhoodExpression) {
	switch (neighborhoodExpression->type)
	{
		case NEIGHBORHOOD_ASSIGNMENT:
			_output(indentation,"%s=",neighborhoodExpression->variable);
			_generateArithmeticExpression(neighborhoodExpression->assignment);
			break;
		case NEIGHBORHOOD_FOR_LOOP:
			_output(indentation, "for %s in ", neighborhoodExpression->forVariable);
			_generateRange(neighborhoodExpression->range);
			_output(0, ":\n");
			_generateNeighborhoodSequence(indentation + 1, neighborhoodExpression->forBody);
			break;
		case NEIGHBORHOOD_IF:
			_output(indentation, "if ");
			_generateArithmeticExpression(neighborhoodExpression->ifCondition);
			_output(0, ":\n");
			_generateNeighborhoodSequence(indentation + 1, neighborhoodExpression->ifBody);
			break;
		case NEIGHBORHOOD_IF_ELSE:
			_output(indentation, "if ");
			_generateArithmeticExpression(neighborhoodExpression->ifElseCondition);
			_output(0, ":\n");
			_generateNeighborhoodExpression(indentation + 1, neighborhoodExpression->ifElseIfBody);
			_output(indentation, "\nelse:\n");
			_generateNeighborhoodSequence(indentation + 1, neighborhoodExpression->ifElseElseBody);
			break;
		case ADD_CELL_EXP:
			_output(indentation, "neighbors.update(\n");
			_generateCellList(neighborhoodExpression->toAddList);
			_output(0, ")");
		case REMOVE_CELL_EXP:
			_output(indentation, "neighbors.difference_update(\n");
			_generateCellList(neighborhoodExpression->toRemoveList);
			_output(0, ")");
		break;
	default:
		break;
	}
}



static void _generateTransitionSequence(unsigned int indentation, const TransitionSequence * transitionSequence) {
	_generateTransitionExpression(indentation, transitionSequence->rightExpression);
	if (transitionSequence->sequence != NULL) {
		_generateTransitionSequence(indentation, transitionSequence->sequence);
	} else {
		_output(0, "\n");
	}
}

static void _generateNeighborhoodSequence(unsigned int indentation, const NeighborhoodSequence * neighborSequence) {
	_generateNeighborhoodExpression(indentation, neighborSequence->rightExpression);
	if (neighborSequence->sequence != NULL) {
		_generateNeighborhoodSequence(indentation, neighborSequence->sequence);
	} else {
		_output(indentation, "return neighbors\n");
	}
}

/**
 * Generates the output of a constant.
 */
static void _generateConstant(const Constant * constant) {
	switch (constant->type) {
	case INTEGER_C:
		_output(0, "%d", constant->value);
		break;
	case STRING_C:
		_output(0, "%s", constant->string);
		break;
	default:
		break;
	}
}

static char MAX_DISPLACEMENT_TAG_LENGTH = 256;

static char * _displacementTypeToString(const DisplacementType displacementType, Constant * value) {
	switch (displacementType)
	{
	case HORIZONTAL_D:
		_output(0, "row+");
		_generateConstant(value);
		_output(0, ",col");
		break;
	case VERTICAL_D:
		_output(0, "row,");
		_output(0, "col+");
		_generateConstant(value);
		break;
	case DIAGONAL_ASC_D:
	//@todo: ver para donde crecen las columnas (si es positivo para abajp para arriba)
		_output(0, "row+");
		_generateConstant(value);		
		_output(0, ",");
		_output(0, "col+");
		_generateConstant(value);		
		break;
	case DIAGONAL_DESC_D:
		_output(0, "row+");
		_generateConstant(value);		
		_output(0, ",");
		_output(0, "col-");
		_generateConstant(value);
		break;
	default:
		logError(_logger, "The specified displacement type is not valid: %d", displacementType);
		return '\0';
	}
}

static char * GET_CELL_VALUE_FUN = "get_cell_value(cells,";

static void _generateCell(const Cell * cell) {
	if (cell->isSingleCoordenate) {
		_output(0, GET_CELL_VALUE_FUN);
		_outputDisplacement(cell->displacementType, cell->displacement);
		_output(0,")");
	} else {
		_output(0,"%s%s", GET_CELL_VALUE_FUN, "row+");
		_generateConstant(cell->x);
		_output(0,",col+");
		_generateConstant(cell->y);
		_output(0,")");
	}
}

static void _generateCellList(const CellList * cellList) {
	_output(0, "[");
	_generateCellListRec(cellList);
	_output(0, "]");
}

static void _generateCellListRec(const CellList * cellList) {
	if (cellList->isLast) {
		_generateCell(cellList->cell);
		return;
	}
	_generateCell(cellList->cell);
	_output(0,",");
	_generateCellListRec(cellList);
}

static void _generateConstantArray(const ConstantArray * arr) {
	_output(0, "[");
	_generateConstantArrayRec(arr);
	_output(0, "]");

}

static void _generateConstantArrayRec(const ConstantArray * arr) {
	if (arr->isLast) {
		_generateCell(arr->value);
		return;
	}
	_generateConstant(arr->value);
	_output(0,",");
	_generateConstantArrayRec(arr);
}

static void _generateIntArray(const IntArray * arr) {
	_output(0, "[");
	_generateIntArrayRec(arr);
	_output(0, "]");
}

static void _generateIntArrayRec(const IntArray * arr) {
	if (arr->isLast) {
		_output(0,"%d", arr->value);
		return;
	}
	_output(0,"%d,", arr->value);
	_generateIntArrayRec(arr);
}

static void _generateStringArray(const StringArray * arr) {
	_output(0, "{");
	_generateStringArrayRec(0, arr);
	_output(0, "}");
}

static void _generateStringArrayRec(unsigned int depth, const StringArray * arr) {
	if (arr->isLast) {
		_output(0,"'%s': %d", arr->value, depth);
		return;
	}
	_output(0,"'%s': %d", arr->value, depth);
	_generateStringArrayRec(depth + 1, arr);
}

static void _generateRange(const Range * range) {
	if (range->type == ARRAY) {
		_generateConstantArray(range->array);
	} else {
		_output(0, "range(");
		_generateConstant(range->start);
		_output(0, ",");
		_generateConstant(range->end);
		_output(0, ")");
	}
}

static void _generateArithmeticExpression(const ArithmeticExpression * arithmeticExpression) {
	if (arithmeticExpression->type <= GREATER_THAN_OR_EQUAL) {
		_generateArithmeticExpression(arithmeticExpression->leftExpression);
		_output(0, _arithmeticExpressionTypeToString(arithmeticExpression->type));
		_generateArithmeticExpression(arithmeticExpression->rightExpression);
	} else if (arithmeticExpression->type == LOGIC_NOT) {
		_output(0, _arithmeticExpressionTypeToString(arithmeticExpression->type));
		_generateArithmeticExpression(arithmeticExpression->expression);
	} else if (arithmeticExpression->type == FACTOR) {
		_output(0, "(");
		_generateArithmeticExpression(arithmeticExpression->expression);
		_output(0, ")");
	} else if (arithmeticExpression->type == CONSTANT) {
		_generateConstant(arithmeticExpression->constant);
	} else if (arithmeticExpression->type == CELL_ARITHETIC_EXPRESSION) {
		_generateCell(arithmeticExpression->cell);
	} else {
		_output(0, _arithmeticExpressionTypeToString(arithmeticExpression->type));
		_output(0, "x == %s for x in ", arithmeticExpression->state);
		_generateCellList(arithmeticExpression->cellList);
		_output(0, ")");
		if (arithmeticExpression->type == AT_LEAST_ARE) {
			_output(0," >= %d", arithmeticExpression->count);
		}
	}
}

static char MAX_FRONTIER_TYPE_LENGTH = 16;

static char * _getStringFromFrontierType(const FrontierEnum type) {
	char * operand = calloc(MAX_FRONTIER_TYPE_LENGTH, sizeof(char));
	switch (type) {
		case PERIODIC:
			strcpy(operand, "Periodic");
			break;
		case OPEN:
			strcpy(operand, "Open");
			break;
		case MIRROR:
			strcpy(operand, "Mirror");
			break;
		default:
			logError(_logger, "The specified frontier type is not valid: %d", type);
			break;
	}
}

static char * _getStringFromEvolutionType(const EvolutionEnum type) {
	char * operand = calloc(MAX_FRONTIER_TYPE_LENGTH, sizeof(char));
	switch (type) {
		case CONWAY:
			strcpy(operand, "Conway");
			break;
		case SEEDS:
			strcpy(operand, "Seeds");
			break;
			logError(_logger, "The specified evolution type is not valid: %d", type);
			break;
	}
}

static void _generateOption(const Option * option) {
	switch (option->type){
		case HEIGHT_OPTION:
			_output(0,"SCREEN_HEIGHT=%d\n", option->value);
			break; 
		case WIDTH_OPTION:   
			_output(0,"SCREEN_WIDTH=%d\n", option->value);
			break;          
		case FRONTIER_OPTION:   
			_output(0,"FRONTIER_MODE=%s\n", _getStringFromFrontierType(option->frontierType));
			break;          
		case COLORS_OPTION:
			_output(0,"STATE_COLORS=");
			_generateIntArray(option->colors);
			_output(0,"\n");
			break;                
		case STATES_OPTION: 
			_output(0,"STATE=");
			_generateStringArray(option->states);
			_output(0,"\n");  
			break;            
		case NEIGHBORHOOD_OPTION:
			break;      
		case EVOLUTION_OPTION:
			if(option->evolution->isDefault) {
				_output(0,"EVOLUTION_MODE=%s\n", _getStringFromEvolutionType(option->value));
				
			} else {
				_output(0,"EVOLUTION_MODE='SB'\n");
				_output(0,"SURVIVE_RULES=");
				_generateIntArray(option->evolution->array);
				_output(0,"\n");
				//@todo: cambiar para que sea un array tambien
				_output(0,"BIRTH_RULES=");
				_output(0,"[%d]", option->evolution->value);
				_output(0,"\n");
			}
			break;
		default:
			logError(_logger, "The specified option type is not valid: %d", option->type);
			break;
	}
}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
static void _generateEpilogue(const int value) {
	_output(0, "%s",
		
		"N_CELLS_X = SCREEN_WIDTH // CELL_SIZE\n"
		"N_CELLS_Y = SCREEN_HEIGHT // CELL_SIZE\n\n"
		"def get_cell_value(cells, row, col):\n"
		"    \n"
		"    if FRONTIER_MODE == 'Periodic':\n"
		"        row = row % N_CELLS_Y\n"
		"        col = col % N_CELLS_X\n"
		"        return cells[row, col]\n"
		"\n"
		"    if FRONTIER_MODE == 'Mirror':\n"
		"        if row < 0: row = -row\n"
		"        if col < 0: col = -col\n"
		"        if row >= N_CELLS_Y: row = 2 * (N_CELLS_Y - 1) - row\n"
		"        if col >= N_CELLS_X: col = 2 * (N_CELLS_X - 1) - col\n"
		"        return cells[row, col]\n"
		"        \n"
		"    if 0 <= row < N_CELLS_Y and 0 <= col < N_CELLS_X:\n"
		"        return cells[row, col]\n"
		"    else:\n"
		"        return 0\n"
		"\n"
		"def update(screen, cells):\n"
		"    \n"
		"    updated_cells = np.zeros((N_CELLS_Y, N_CELLS_X))\n"
		"\n"
		"    for row, col in np.ndindex(cells.shape):\n"
		"        \n"
		"        if EVOLUTION_MODE == 'SB':\n"
		"            alive_neighbors = 0\n"
		"            for dr, dc in NEIGHBORHOOD:\n"
		"                if get_cell_value(cells, row + dr, col + dc) == 1:\n"
		"                    alive_neighbors += 1\n"
		"            \n"
		"            current_state = cells[row, col]\n"
		"            \n"
		"            if current_state == 1:\n"
		"                if alive_neighbors in SURVIVE_RULES:\n"
		"                    updated_cells[row, col] = 1\n"
		"                else:\n"
		"                    updated_cells[row, col] = 0\n"
		"            else:\n"
		"                if alive_neighbors in BIRTH_RULES:\n"
		"                    updated_cells[row, col] = 1\n"
		"                else:\n"
		"                    updated_cells[row, col] = 0\n"
		"\n"
		"        elif EVOLUTION_MODE == 'Transition':\n"
		"            updated_cells[row, col] = transition_function(cells, row, col)\n"
		"\n"
		"        color = STATE_COLORS[int(updated_cells[row, col])]\n"
		"        pygame.draw.rect(screen, color, (col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE -1, CELL_SIZE -1))\n"
		"\n"
		"    return updated_cells\n"
		"\n"
		"def main():\n"
		"    pygame.init()\n"
		"    pygame.display.set_caption(\"Cellular Automata\")\n"
		"    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))\n"
		"\n"
		"    cells = np.zeros((N_CELLS_Y, N_CELLS_X))\n"
		"    \n"
		"    for row, col in np.ndindex(cells.shape):\n"
		"        color = STATE_COLORS[0]\n"
		"        pygame.draw.rect(screen, color, (col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1))\n"
		"\n"
		"    pygame.display.flip()\n"
		"    \n"
		"    running = False\n"
		"    num_states = len(STATES)\n"
		"    \n"
		"    drawing_mode = 0\n"
		"    painted_cells_this_drag = set()\n"
		"\n"
		"    while True:\n"
		"        for event in pygame.event.get():\n"
		"            if event.type == pygame.QUIT:\n"
		"                pygame.quit()\n"
		"                return\n"
		"            elif event.type == pygame.KEYDOWN:\n"
		"                if event.key == pygame.K_SPACE:\n"
		"                    running = not running\n"
		"                elif event.key == pygame.K_c:\n"
		"                    running = False\n"
		"                    cells = np.zeros((N_CELLS_Y, N_CELLS_X))\n"
		"            \n"
		"            elif event.type == pygame.MOUSEBUTTONDOWN:\n"
		"                drawing_mode = event.button\n"
		"                painted_cells_this_drag.clear()\n"
		"                pos = pygame.mouse.get_pos()\n"
		"                row, col = pos[1] // CELL_SIZE, pos[0] // CELL_SIZE\n"
		"                if row < N_CELLS_Y and col < N_CELLS_X:\n"
		"                    if drawing_mode == 1:\n"
		"                        cells[row, col] = (cells[row, col] + 1) % num_states\n"
		"                    elif drawing_mode == 3:\n"
		"                        cells[row, col] = 0\n"
		"                    painted_cells_this_drag.add((row, col))\n"
		"\n"
		"            elif event.type == pygame.MOUSEBUTTONUP:\n"
		"                drawing_mode = 0\n"
		"                painted_cells_this_drag.clear()\n"
		"\n"
		"            elif event.type == pygame.MOUSEMOTION:\n"
		"                if drawing_mode != 0:\n"
		"                    pos = event.pos\n"
		"                    row, col = pos[1] // CELL_SIZE, pos[0] // CELL_SIZE\n"
		"                    if (row, col) not in painted_cells_this_drag:\n"
		"                        if row < N_CELLS_Y and col < N_CELLS_X:\n"
		"                            if drawing_mode == 1:\n"
		"                                cells[row, col] = (cells[row, col] + 1) % num_states\n"
		"                            elif drawing_mode == 3:\n"
		"                                cells[row, col] = 0\n"
		"                            painted_cells_this_drag.add((row, col))\n"
		"\n"
		"        screen.fill((40,40,40))\n"
		"\n"
		"        if running:\n"
		"            cells = update(screen, cells)\n"
		"        else:\n"
		"            for row, col in np.ndindex(cells.shape):\n"
		"                color = STATE_COLORS[int(cells[row, col])]\n"
		"                pygame.draw.rect(screen, color, (col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1))\n"
		"\n"
		"        pygame.display.update()\n"
		"        time.sleep(0.01)\n"
		"\n"
		"if __name__ == '__main__':\n"
		"    main()\n"
	);
}

/**
 * Creates the prologue of the generated output, a Latex document that renders
 * a tree thanks to the Forest package.
 *
 * @see https://ctan.dcc.uchile.cl/graphics/pgf/contrib/forest/forest-doc.pdf
 */
static void _generatePrologue(void) {
	_output(0, "%s",
		"\\import time\n"
		"\\import pygame\n"
		"\\import numpy as np\n\n"
		"\\CELL_SIZE = 20\n"
	);
}

/**
 * Generates an indentation string for the specified level.
 */
static char * _indentation(const unsigned int level) {
	return indentation(_indentationCharacter, level, _indentationSize);
}

/**
 * Outputs a formatted string to standard output. The "fflush" instruction
 * allows to see the output even close to a failure, because it drops the
 * buffering.
 */
static void _output(const unsigned int indentationLevel, const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	char * indentation = _indentation(indentationLevel);
	char * effectiveFormat = concatenate(2, indentation, format);
	vfprintf(stdout, effectiveFormat, arguments);
	fflush(stdout);
	free(effectiveFormat);
	free(indentation);
	va_end(arguments);
}

/** PUBLIC FUNCTIONS */

void generate(CompilerState * compilerState) {
	logDebugging(_logger, "Generating final output...");
	_generatePrologue();
	_generateProgram(compilerState->abstractSyntaxtTree);
	_generateEpilogue(compilerState->value);
	logDebugging(_logger, "Generation is done.");
}
