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
static void _generateOption(const Option * option);
static void _generateProgram(Program * program);
static void _generateNeighborhoodSequence(unsigned int indentation, const NeighborhoodSequence * neighborSequence);
static void _generateTransitionSequence(unsigned int indentation, const TransitionSequence * transitionSequence);
static void _generateTransitionExpression(unsigned int indentation, const TransitionExpression * transitionExpression);
static void _generateNeighborhoodExpression(unsigned int indentation, const NeighborhoodExpression * neighborhoodExpression);

static void _generateArithmeticExpression(const ArithmeticExpression * arithmeticExpression);
static void _generateConfiguration(Configuration * config);

static void _generateNeighbourhoodCellList(const CellList * cellList);
static void _generateTransitionCellList(const CellList * cellList);
static void _generateRange(const Range * range);

static void _generateConstantArrayRec(const ConstantArray * arr);
static void _generateStringArray(const StringArray * arr);
static void _generateIntArray(const IntArray * arr);

static void _generatePrologue(void);
static char * _indentation(const unsigned int indentationLevel);
static void _output(const unsigned int indentationLevel, const char * const format, ...);

/**
 * Converts and expression type to the proper character of the operation
 * involved, or returns '\0' if that's not possible.
 */
static char MAX_OPERAND_LENGTH = 16; 

void _generateProgram(Program * program) {
	_generateConfiguration(program->configuration);
	switch (program->type)
	{
		//@todo: si el usuario no pone return al final se puede crashear (ponemos por default que retorne el estado actual de la celda?)
	case TRANSITION:
		_output(0, "EVOLUTION_MODE='Transition'\n");
		_output(0,"def transition_function(cells, row, col):\n");
		_generateTransitionSequence(1, program->transitionSequence);
		break;
	case NEIGHBORHOOD_PROGRAM:
		_output(0,"def neighborhood_function(row, col):\n");
		_output(1,"neighbors = set()\n");
		_generateNeighborhoodSequence(1, program->neighborhoodSequence);
		_output(1,"return  neighbors\n");
		break;
	default:
		break;
	}
	
}

void _generateConfiguration(Configuration * config) {
	if (config->isLast) {
		_generateOption(config->lastOption);
	} else {
		_generateOption(config->option);
		_generateConfiguration(config->next);
	}
}

static char * _arithmeticExpressionTypeToString(const ArithmeticExpressionType type) {
	switch (type) {
		case ADDITION: 
			return " + ";
		case DIVISION: 
			return " // ";
		case MULTIPLICATION: 	
			return " * ";
		case SUBTRACTION: 
			return " - ";
		case MODULE: 
			return " % ";
		case LOGIC_AND: 
			return " and ";
		case LOGIC_OR:
			return " or ";
		case EQUALS:
			return " == ";
		case NOT_EQUALS:
			return " != ";
		case LOWER_THAN:
			return " < ";
		case LOWER_THAN_OR_EQUAL:
			return " <= ";
		case GREATER_THAN:
			return " > ";
		case GREATER_THAN_OR_EQUAL:
			return " >= ";
		case LOGIC_NOT:
			return " not ";
		case ALL_ARE:
			return " all(";
		case ANY_ARE:
			return " any(";
		case AT_LEAST_ARE:
			return " sum(";
		case FACTOR:
		case CONSTANT:
		case CELL_ARITHETIC_EXPRESSION:
			return "";
		default:
			logError(_logger, "The specified expression type cannot be converted into character: %d", type);
			return "";
	}
}

static void _generateTransitionExpression(unsigned int indentation, const TransitionExpression * transitionExpression) {
	switch (transitionExpression->type)
	{
		case TRANSITION_ASSIGNMENT:
			_output(indentation,"%s=", transitionExpression->variable);
			_generateArithmeticExpression(transitionExpression->assignment);
			_output(0,"\n");
			break;
		case TRANSITION_FOR_LOOP:
			_output(indentation, "for %s in ", transitionExpression->forVariable);
			_generateRange(transitionExpression->range);
			_output(0, ":\n");
			_generateTransitionSequence(indentation + 1, transitionExpression->forBody);
			break;
		case TRANSITION_IF:
			_output(indentation, "if ");
			_generateArithmeticExpression(transitionExpression->ifCondition);
			_output(0, ":\n");
			_generateTransitionSequence(indentation + 1, transitionExpression->ifBody);
			break;
		case TRANSITION_IF_ELSE:
			_output(indentation, "if ");
			_generateArithmeticExpression(transitionExpression->ifElseCondition);
			_output(0, ":\n");
			_generateTransitionSequence(indentation + 1, transitionExpression->ifElseIfBody);
			_output(indentation, "else:\n");
			_generateTransitionSequence(indentation + 1, transitionExpression->ifElseElseBody);
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
			_generateNeighborhoodSequence(indentation + 1, neighborhoodExpression->ifElseIfBody);
			_output(indentation, "\nelse:\n");
			_generateNeighborhoodSequence(indentation + 1, neighborhoodExpression->ifElseElseBody);
			break;
		case ADD_CELL_EXP:
			_output(indentation, "neighbors.update(");
			_generateNeighbourhoodCellList(neighborhoodExpression->toAddList);
			_output(0, ")\n");
		case REMOVE_CELL_EXP:
			_output(indentation, "neighbors.difference_update(");
			_generateNeighbourhoodCellList(neighborhoodExpression->toRemoveList);
			_output(0, ")\n");
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

static void _outputDisplacement(const DisplacementType displacementType, Constant * value) {
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
	}
}

static char * GET_CELL_VALUE_FUN = "get_cell_value(cells,";

static void _generateTransitionCell(const Cell * cell) {
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

static void _generateNeighbourhoodCell(const Cell * cell) {
	if (cell->isSingleCoordenate) {
		_output(0, "()");
		_outputDisplacement(cell->displacementType, cell->displacement);
		_output(0,")");
	} else {
		_output(0,"(row+");
		_generateConstant(cell->x);
		_output(0,",col+");
		_generateConstant(cell->y);
		_output(0,")");
	}
}

static void _generateTransitionCellListRec(const CellList * cellList) {
	if (cellList->isLast) {
		_generateTransitionCell(cellList->cell);
		return;
	}
	_generateTransitionCell(cellList->cell);
	_output(0,",");
	_generateTransitionCellListRec(cellList->next);
}
static void _generateTransitionCellList(const CellList * cellList) {
	_output(0, "[");
	_generateTransitionCellListRec(cellList);
	_output(0, "]");
}

static void _generateNeighbourhoodCellListRec(const CellList * cellList) {
	if (cellList->isLast) {
		_generateNeighbourhoodCell(cellList->cell);
		return;
	}
	_generateNeighbourhoodCell(cellList->cell);
	_output(0,",");
	_generateNeighbourhoodCellListRec(cellList->next);
}

static void _generateNeighbourhoodCellList(const CellList * cellList) {
	_output(0, "[");
	_generateNeighbourhoodCellListRec(cellList);
	_output(0, "]");
}

static void _generateConstantArrayRec(const ConstantArray * arr) {
	if (arr->isLast) {
		_generateConstant(arr->value);
		return;
	}
	_generateConstant(arr->value);
	_output(0,",");
	_generateConstantArrayRec(arr->next);
}

static void _generateConstantArray(const ConstantArray * arr) {
	_output(0, "[");
	_generateConstantArrayRec(arr);
	_output(0, "]");

}

static void _generateIntArrayRec(const IntArray * arr) {
	if (arr->isLast) {
		_output(0,"%d", arr->value);
		return;
	}
	_output(0,"%d,", arr->value);
	_generateIntArrayRec(arr->next);
}

static void _generateIntArray(const IntArray * arr) {
	_output(0, "[");
	_generateIntArrayRec(arr);
	_output(0, "]");
}

static void _generateStringArrayRec(unsigned int depth, const StringArray * arr) {
	if (arr->isLast) {
		_output(0,"'%s': %d", arr->value, depth);
		return;
	}
	_output(0,"'%s': %d, ", arr->value, depth);
	_generateStringArrayRec(depth + 1, arr->next);
}

static void _generateStringArray(const StringArray * arr) {
	_output(0, "{");
	_generateStringArrayRec(0, arr);
	_output(0, "}");
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
		_generateTransitionCell(arithmeticExpression->cell);
	} else {
		_output(0, _arithmeticExpressionTypeToString(arithmeticExpression->type));
		_output(0, "x == %s for x in ", arithmeticExpression->state);
		_generateTransitionCellList(arithmeticExpression->cellList);
		_output(0, ")");
		if (arithmeticExpression->type == AT_LEAST_ARE) {
			_output(0," >= %d", arithmeticExpression->count);
		}
	}
}

static char MAX_FRONTIER_TYPE_LENGTH = 16;

static const char * _getStringFromFrontierType(const FrontierEnum type) {
    switch (type) {
        case PERIODIC: 
			return "Periodic";
        case OPEN: 
			return "Open";
        case MIRROR: 
			return "Mirror";
        default:
            logError(_logger, "The specified frontier type is not valid: %d", type);
			return NULL;
    }
}

static char * _getStringFromEvolutionType(const EvolutionEnum type) {
	char * operand = calloc(MAX_FRONTIER_TYPE_LENGTH, sizeof(char));
	switch (type) {
		case CONWAY:
			strcpy(operand, "Conway");
			return operand;
		case SEEDS:
			strcpy(operand, "Seeds");
			return operand;
		default:
			logError(_logger, "The specified evolution type is not valid: %d", type);
			return NULL;
	}
}
static void _generateStateVariablesRec(unsigned int value, StringArray * states) {
	if (states->isLast) {
		_output(0,"%s=%d\n", states->lastValue, value);
		return;
	}
	_output(0,"%s=%d\n", states->value, value);
	_generateStateVariablesRec(value + 1, states->next);
}

static void _generateStateVariables(StringArray * states) {
	_generateStateVariablesRec(0, states);
}

static void _generateOption(const Option * option) {
	switch (option->type){
		case HEIGHT_OPTION:
		//@todo: aca deberia ir la cantidad de celdas, no el tamaño de la pantalla
			_output(0,"N_CELLS_Y=%d\n", option->value);
			break; 
		case WIDTH_OPTION:   
			_output(0,"N_CELLS_X=%d\n", option->value);
			break;          
		case FRONTIER_OPTION:   
			_output(0,"FRONTIER_MODE='%s'\n", _getStringFromFrontierType(option->frontierType));
			break;          
		case COLORS_OPTION:
			_output(0,"STATE_COLORS=");
			_generateIntArray(option->colors);
			_output(0,"\n");
			break;                
		case STATES_OPTION: 
			_generateStateVariables(option->states);
			_output(0,"STATES=");
			_generateStringArray(option->states);
			_output(0,"\n");  
			break;            
		case NEIGHBORHOOD_OPTION:
			if (option->neighborhoodEnum == MOORE) {
				_output(0, "%s",
							"def neighborhood_function(row, col):\n"
							"    return {\n"
							"        (-1, -1), (-1, 0), (-1, 1),\n"
							"        (0, -1),          (0, 1),\n"
							"        (1, -1),  (1, 0),  (1, 1)\n"
							"    }\n");
			}
			else if (option->neighborhoodEnum == VON_NEUMANN) {
				_output(0, "%s",
							"def neighborhood_function(row, col):\n"
							"    return {\n"
							"        (-1, 0), (1, 0), (0, -1), (0, 1)\n"
							"    }\n");
			}
			break;
		case EVOLUTION_OPTION:
			_output(0,"EVOLUTION_MODE='SB'\n");
			_output(0,"SURVIVE_RULES = ");
			if(option->evolution->isDefault) {
					if (option->evolution->evolutionTypes == CONWAY)
					{
						_output(0,"[2,3]\n");
						_output(0,"BIRTH_RULES = [3]\n");
					} else if (option->evolution->evolutionTypes == SEEDS) {
						_output(0,"[]\n");
						_output(0,"BIRTH_RULES = [2]\n");
					} 
					else {
						logError(_logger, "The specified evolution type is not valid: %d", option->evolution->evolutionTypes);
					}
			} else {
				_generateIntArray(option->evolution->surviveArray);
				_output(0,"\nBIRTH_RULES = ");
				_generateIntArray(option->evolution->birthArray);
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
		
		"cell_size_w = MAX_SCREEN_WIDTH // N_CELLS_X\n"
		"cell_size_h = MAX_SCREEN_HEIGHT // N_CELLS_Y\n"
		"\n"
		"CELL_SIZE = min(cell_size_w, cell_size_h)\n"
		"\n"
		"SCREEN_WIDTH = N_CELLS_X * CELL_SIZE\n"
		"SCREEN_HEIGHT = N_CELLS_Y * CELL_SIZE\n"
		"\n"
		"def get_cell_value(cells, row, col):\n"
		"\n"
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
		"\n"
		"    if 0 <= row < N_CELLS_Y and 0 <= col < N_CELLS_X:\n"
		"        return cells[row, col]\n"
		"    else:\n"
		"        return 0\n"
		"\n"
		"def update(screen, cells):\n"
		"\n"
		"    updated_cells = np.zeros((N_CELLS_Y, N_CELLS_X))\n"
		"\n"
		"    for row, col in np.ndindex(cells.shape):\n"
		"\n"
		"        if EVOLUTION_MODE == 'SB':\n"
		"            alive_neighbors = 0\n"
		"            for dr, dc in neighborhood_function(row, col):\n"
		"                if get_cell_value(cells, row + dr, col + dc) == 1:\n"
		"                    alive_neighbors += 1\n"
		"\n"
		"            current_state = cells[row, col]\n"
		"\n"
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
		"    pygame.display.set_caption(\"Cellular Automata\")\n"
		"    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))\n"
		"\n"
		"    cells = np.zeros((N_CELLS_Y, N_CELLS_X))\n"
		"\n"
		"    for row, col in np.ndindex(cells.shape):\n"
		"        color = STATE_COLORS[0]\n"
		"        pygame.draw.rect(screen, color, (col * CELL_SIZE, row * CELL_SIZE, CELL_SIZE - 1, CELL_SIZE - 1))\n"
		"\n"
		"    pygame.display.flip()\n"
		"\n"
		"    running = False\n"
		"    num_states = len(STATES)\n"
		"\n"
		"    drawing_mode = 0\n"
		"    painted_cells_this_drag = set()\n"
		"    \n"
		"    is_dropdown_open = False\n"
		"    is_dragging_slider = False\n"
		"    speed_slider_value = 0.9     \n"
		"\n"
		"    is_dropdown_open = False\n"
		"\n"
		"    GUI_BG_COLOR = (45, 52, 54)\n"
		"    GUI_ACCENT_COLOR = (99, 110, 114)\n"
		"    GUI_TEXT_COLOR = (223, 230, 232)\n"
		"    GUI_BORDER_RADIUS = 8\n"
		"    GUI_PADDING = 10\n"
		"    SYMBOL_SIZE = 10\n"
		"\n"
		"    font_names = [\"Calibri\", \"Arial\", \"Helvetica\", \"sans-serif\"]\n"
		"    title_font = pygame.font.SysFont(font_names, 26)\n"
		"    title_font.set_bold(True)\n"
		"    controls_font = pygame.font.SysFont(font_names, 22)\n"
		"    symbol_font = pygame.font.SysFont(font_names, 24)\n"
		"\n"
		"    controls_font_bold = pygame.font.SysFont(font_names, 22, bold=True)\n"
		"    controls_font_regular = pygame.font.SysFont(font_names, 22)\n"
		"\n"
		"    title_surface = title_font.render(\"Controls\", True, GUI_TEXT_COLOR)\n"
		"\n"
		"    title_rect_width = title_surface.get_width() + SYMBOL_SIZE + GUI_PADDING * 3\n"
		"    title_rect = pygame.Rect(GUI_PADDING, GUI_PADDING, title_rect_width, title_surface.get_height() + GUI_PADDING)\n"
		"\n"
		"    controls_data = [\n"
		"        (\"Left Click / Drag:\", \" Cycle cell state\"),\n"
		"        (\"Right Click / Drag:\", \" Kill cell (state 0)\"),\n"
		"        (\"Spacebar:\", \" Play / Pause simulation\"),\n"
		"        (\"'C' Key:\", \" Clear the grid\"),\n"
		"    ]\n"
		"\n"
		"    controls_surface_pairs = []\n"
		"    for key, description in controls_data:\n"
		"        key_surf = controls_font_bold.render(key, True, GUI_TEXT_COLOR)\n"
		"        desc_surf = controls_font_regular.render(description, True, GUI_TEXT_COLOR)\n"
		"        controls_surface_pairs.append((key_surf, desc_surf))\n"
		"\n"
		"    speed_label_surf = controls_font_bold.render(\"Speed:\", True, GUI_TEXT_COLOR)\n"
		"\n"
		"    max_text_width = max(pair[0].get_width() + pair[1].get_width() for pair in controls_surface_pairs)\n"
		"    dropdown_width = max(title_rect.width, max_text_width + GUI_PADDING * 2)\n"
		"\n"
		"    slider_y_pos = title_rect.bottom + (controls_font_regular.get_height() + 5) * (len(controls_data) + 1)\n"
		"    slider_bar_rect = pygame.Rect(title_rect.left + speed_label_surf.get_width() + GUI_PADDING, slider_y_pos, dropdown_width - speed_label_surf.get_width() - GUI_PADDING * 2, 8)\n"
		"\n"
		"    dropdown_height = (slider_bar_rect.bottom - title_rect.bottom) + GUI_PADDING\n"
		"    dropdown_rect = pygame.Rect(title_rect.left, title_rect.bottom + 5, dropdown_width, dropdown_height)\n"
		"\n"
		"    while True:\n"
		"        mouse_pos = pygame.mouse.get_pos()\n"
		"\n"
		"        for event in pygame.event.get():\n"
		"            if event.type == pygame.QUIT:\n"
		"                pygame.quit()\n"
		"                return\n"
		"            \n"
		"            elif event.type == pygame.MOUSEBUTTONDOWN:\n"
		"                if event.button == 1:\n"
		"                    if title_rect.collidepoint(event.pos):\n"
		"                        is_dropdown_open = not is_dropdown_open\n"
		"                    elif is_dropdown_open and slider_bar_rect.inflate(0, 20).collidepoint(event.pos):\n"
		"                        is_dragging_slider = True\n"
		"                        click_offset = event.pos[0] - slider_bar_rect.x\n"
		"                        speed_slider_value = max(0.0, min(1.0, click_offset / slider_bar_rect.width))\n"
		"                    else:\n"
		"                        drawing_mode = 1\n"
		"                        painted_cells_this_drag.clear()\n"
		"                        row, col = event.pos[1] // CELL_SIZE, event.pos[0] // CELL_SIZE\n"
		"                        if 0 <= row < N_CELLS_Y and 0 <= col < N_CELLS_X:\n"
		"                            cells[row, col] = (cells[row, col] + 1) % num_states\n"
		"                            painted_cells_this_drag.add((row, col))\n"
		"                elif event.button == 3:\n"
		"                    drawing_mode = 3\n"
		"                    painted_cells_this_drag.clear()\n"
		"                    row, col = event.pos[1] // CELL_SIZE, event.pos[0] // CELL_SIZE\n"
		"                    if 0 <= row < N_CELLS_Y and 0 <= col < N_CELLS_X:\n"
		"                        cells[row, col] = 0\n"
		"                        painted_cells_this_drag.add((row, col))\n"
		"\n"
		"            elif event.type == pygame.KEYDOWN:\n"
		"                if event.key == pygame.K_SPACE:\n"
		"                    running = not running\n"
		"                elif event.key == pygame.K_c:\n"
		"                    running = False\n"
		"                    cells = np.zeros((N_CELLS_Y, N_CELLS_X))\n"
		"            \n"
		"            elif event.type == pygame.MOUSEBUTTONUP:\n"
		"                drawing_mode = 0\n"
		"                is_dragging_slider = False\n"
		"\n"
		"            elif event.type == pygame.MOUSEMOTION:\n"
		"                if is_dragging_slider:\n"
		"                    click_offset = event.pos[0] - slider_bar_rect.x\n"
		"                    speed_slider_value = max(0.0, min(1.0, click_offset / slider_bar_rect.width))\n"
		"                elif drawing_mode != 0:\n"
		"                    pos = event.pos\n"
		"                    row, col = pos[1] // CELL_SIZE, pos[0] // CELL_SIZE\n"
		"                    if (row, col) not in painted_cells_this_drag:\n"
		"                        if 0 <= row < N_CELLS_Y and 0 <= col < N_CELLS_X:\n"
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
		"        \n"
		"        is_hovering = title_rect.collidepoint(mouse_pos)\n"
		"        current_title_bg = GUI_ACCENT_COLOR if is_hovering else GUI_BG_COLOR\n"
		"        \n"
		"        pygame.draw.rect(screen, current_title_bg, title_rect, border_radius=GUI_BORDER_RADIUS)\n"
		"        screen.blit(title_surface, (title_rect.left + GUI_PADDING, title_rect.centery - title_surface.get_height() // 2))\n"
		"        \n"
		"        symbol_cx = title_rect.right - GUI_PADDING - (SYMBOL_SIZE // 2)\n"
		"        symbol_cy = title_rect.centery\n"
		"        if is_dropdown_open:\n"
		"            points = [(symbol_cx - SYMBOL_SIZE // 2, symbol_cy + SYMBOL_SIZE // 3), (symbol_cx + SYMBOL_SIZE // 2, symbol_cy + SYMBOL_SIZE // 3), (symbol_cx, symbol_cy - SYMBOL_SIZE // 2)]\n"
		"        else:\n"
		"            points = [(symbol_cx - SYMBOL_SIZE // 2, symbol_cy - SYMBOL_SIZE // 3), (symbol_cx + SYMBOL_SIZE // 2, symbol_cy - SYMBOL_SIZE // 3), (symbol_cx, symbol_cy + SYMBOL_SIZE // 2)]\n"
		"        pygame.draw.polygon(screen, GUI_TEXT_COLOR, points)\n"
		"\n"
		"        if is_dropdown_open:\n"
		"            pygame.draw.rect(screen, GUI_BG_COLOR, dropdown_rect, border_radius=GUI_BORDER_RADIUS)\n"
		"            current_y = dropdown_rect.top + 5\n"
		"            for key_surf, desc_surf in controls_surface_pairs:\n"
		"                screen.blit(key_surf, (dropdown_rect.left + GUI_PADDING, current_y))\n"
		"                screen.blit(desc_surf, (dropdown_rect.left + GUI_PADDING + key_surf.get_width(), current_y))\n"
		"                current_y += key_surf.get_height() + 5\n"
		"            \n"
		"                screen.blit(speed_label_surf, (dropdown_rect.left + GUI_PADDING, slider_bar_rect.centery - speed_label_surf.get_height()//2))\n"
		"                pygame.draw.rect(screen, GUI_ACCENT_COLOR, slider_bar_rect, border_radius=4)\n"
		"                handle_x = slider_bar_rect.x + speed_slider_value * slider_bar_rect.width\n"
		"                handle_rect = pygame.Rect(0, 0, 12, slider_bar_rect.height + 8)\n"
		"                handle_rect.center = (handle_x, slider_bar_rect.centery)\n"
		"                pygame.draw.rect(screen, GUI_TEXT_COLOR, handle_rect, border_radius=6)\n"
		"        \n"
		"        pygame.display.update()\n"
		"        \n"
		"        min_sleep, max_sleep = 0.01, 1.0\n"
		"        log_min, log_max = math.log(min_sleep), math.log(max_sleep)\n"
		"        log_sleep = log_max - (speed_slider_value * (log_max - log_min))\n"
		"        sleep_duration = math.exp(log_sleep)\n"
		"        time.sleep(sleep_duration)\n"
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
		"import time\n"
		"import pygame\n"
		"import numpy as np\n"
		"import math\n\n"
		"pygame.init()\n"
		"display_info = pygame.display.Info()\n"
		"MAX_SCREEN_WIDTH = display_info.current_w - 100\n"
		"MAX_SCREEN_HEIGHT = display_info.current_h - 100\n"
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
