#include "Generator.h"

/* MODULE INTERNAL STATE */

const char _indentationCharacter = ' ';
const char _indentationSize = 4;
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

static const char _expressionTypeToCharacter(const ExpressionType type);
static void _generateConstant(const unsigned int indentationLevel, Constant * constant);
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
static const char _expressionTypeToCharacter(const ExpressionType type) {
	switch (type) {
		case ADDITION: return '+';
		case DIVISION: return '/';
		case MULTIPLICATION: return '*';
		case SUBTRACTION: return '-';
		default:
			logError(_logger, "The specified expression type cannot be converted into character: %d", type);
			return '\0';
	}
}

/**
 * Generates the output of a constant.
 */
static void _generateConstant(const unsigned int indentationLevel, Constant * constant) {
	_output(indentationLevel, "%s", "[ $C$, circle, draw, black!20\n");
	_output(1 + indentationLevel, "%s%d%s", "[ $", constant->value, "$, circle, draw ]\n");
	_output(indentationLevel, "%s", "]\n");
}

/**
 * Creates the epilogue of the generated output, that is, the final lines that
 * completes a valid Latex document.
 */
static void _generateEpilogue(const int value) {
	_output(0, "%s",
		"N_CELLS_X = SCREEN_WIDTH // CELL_SIZE\n"
		"N_CELLS_Y = SCREEN_HEIGHT // CELL_SIZE\n\n"
		"def main():\n"
		"    pygame.init()\n"
		"    screen = pygame.display.set_mode((SCREEN_WIDTH, SCREEN_HEIGHT))\n\n"
		"    cells = np.zeros((N_CELLS_Y, N_CELLS_X))\n"
		"    screen.fill(COLOR_GRID)\n"
		"    update(screen, cells, CELL_SIZE)\n\n"
		"    pygame.display.flip()\n"
		"    \n"
		"    running = False\n\n"
		"    while True:\n"
		"        for event in pygame.event.get():\n"
		"            if event.type == pygame.QUIT:\n"
		"                pygame.quit()\n"
		"                return\n"
		"            elif event.type == pygame.KEYDOWN:\n"
		"                if event.key == pygame.K_SPACE:\n"
		"                    running = not running\n"
		"                    update(screen, cells, CELL_SIZE)\n"
		"                elif event.key == pygame.K_c:\n"
		"                    running = False\n"
		"                    cells = np.zeros((N_CELLS_Y, N_CELLS_X))\n"
		"                    screen.fill(COLOR_GRID)\n"
		"                    update(screen, cells, CELL_SIZE)\n"
		"            \n"
		"            if pygame.mouse.get_pressed()[0]:\n"
		"                pos = pygame.mouse.get_pos()\n"
		"                row, col = pos[1] // CELL_SIZE, pos[0] // CELL_SIZE\n"
		"                if row < N_CELLS_Y and col < N_CELLS_X:\n"
		"                    cells[row, col] = 1\n"
		"                    update(screen, cells, CELL_SIZE)\n"
		"                    pygame.display.update()\n"
		"            elif pygame.mouse.get_pressed()[2]:\n"
		"                pos = pygame.mouse.get_pos()\n"
		"                row, col = pos[1] // CELL_SIZE, pos[0] // CELL_SIZE\n"
		"                if row < N_CELLS_Y and col < N_CELLS_X:\n"
		"                    cells[row, col] = 0\n"
		"                    update(screen, cells, CELL_SIZE)\n"
		"                    pygame.display.update()\n\n"
		"        screen.fill(COLOR_GRID)\n\n"
		"        if running:\n"
		"            cells = update(screen, cells, CELL_SIZE, with_progress=True)\n"
		"            pygame.display.update()\n\n"
		"        time.sleep(0.01)\n\n"
		"if __name__ == '__main__':\n"
		"    main()\n"
	);
}

z
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
