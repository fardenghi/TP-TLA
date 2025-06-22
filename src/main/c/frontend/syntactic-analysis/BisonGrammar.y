%{

#include "BisonActions.h"
#include "../../shared/SymbolTable.h"
#include "../../shared/CompilerState.h"
#include "SyntacticAnalyzer.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer;
	FrontierEnum frontier;
	NeighborhoodEnum neighborhood;
	EvolutionEnum evolution;
	DisplacementType displacement;
	char * string;
	Token token;

	/** Non-terminals. */

	ForVariableDeclaration * for_variable_declaration;
	Constant * constant;
	Expression * expression;
	TransitionSequence * transition_sequence;
	NeighborhoodSequence * neighborhood_sequence;
	TransitionExpression * transition_expression;
	NeighborhoodExpression * neighborhood_expression;
	ArithmeticExpression * arithmetic_expression;
	Factor * factor;
	Program * program;
	Configuration * configuration;
	Option * option;
	IntArray * int_array;
	StringArray * string_array;
	ConstantArray * constant_array;
	Cell * cell;
	CellList * cell_list;
	Range * range;
	Configuration * config;
	Evolution * evolution_t;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach. To use this mechanism, the AST must be translated into
 * another structure.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */

%destructor { releaseTransitionSequence($$); } <transition_sequence>
%destructor { releaseNeighborhoodSequence($$); } <neighborhood_sequence>
%destructor { releaseTransitionExpression($$); } <transition_expression>
%destructor { releaseNeighborhoodExpression($$); } <neighborhood_expression>
%destructor { releaseArithmeticExpression($$); } <arithmetic_expression>
%destructor { releaseConstant($$); } <constant>
%destructor { releaseConfiguration($$); } <config>
%destructor { releaseOption($$); } <option>
%destructor { releaseEvolution($$); } <evolution_t>
%destructor { releaseIntArray($$); } <int_array>
%destructor { releaseStringArray($$); } <string_array>
%destructor { releaseConstantArray($$); } <constant_array>
%destructor { releaseCell($$); } <cell>
%destructor { releaseCellList($$); } <cell_list>
%destructor { releaseRange($$); } <range>


/** Terminals. */
%token <integer> INTEGER
%token <string> STRING
%token <frontier> FRONTIER_ENUM
%token <neighborhood> NEIGHBORHOOD_ENUM
%token <evolution> EVOLUTION_ENUM
%token <displacement> DISPLACEMENT_TYPE

%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS
%token <token> ASSIGNMENT


	/*arithmetic*/
%token <token> ADD
%token <token> DIV
%token <token> MOD
%token <token> MUL
%token <token> SUB

%token <token> AND
%token <token> OR
%token <token> NOT
%token <token> EQ
%token <token> NEQ
%token <token> LT
%token <token> LTE
%token <token> GT
%token <token> GTE

%token <token> ALL_OPERAND
%token <token> ANY
%token <token> AT_LEAST
%token <token> ARE

%token <token> OPEN_BRACE
%token <token> CLOSE_BRACE
%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET
%token <token> COMMA
%token <token> SEMICOLON
%token <token> COLON

	/*control*/
%token <token> FOR
%token <token> IN
%token <token> DO
%token <token> END
%token <token> IF
%token <token> THEN
%token <token> ELSE
%token <token> RETURN

	/*config*/
%token <token> CONFIGURATION
%token <token> HEIGHT
%token <token> WIDTH
%token <token> FRONTIER
%token <token> COLORS
%token <token> STATES
%token <token> NEIGHBORHOOD
%token <token> EVOLUTION

%token <token> TRANSITION_FUNCTION
%token <token> NEIGHBORHOOD_FUNCTION

	/*cell*/
%token <token> ADD_CELL
%token <token> REMOVE_CELL

%token <token> UNKNOWN

/** Non-terminals. */
%type <program> program
%type <for_variable_declaration> for_variable_declaration
%type <transition_sequence> transition_sequence
%type <neighborhood_sequence> neighborhood_sequence
%type <transition_expression> transition_expression
%type <neighborhood_expression> neighborhood_expression
%type <arithmetic_expression> arithmetic_expression
%type <constant> constant
%type <config> config
%type <option> option
%type <evolution_t> evolution
%type <int_array> int_array
%type <string_array> string_array
%type <constant_array> constant_array
%type <cell> cell
%type <cell_list> cell_list
%type <range> range

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left OR
%left AND
%right NOT

%nonassoc EQ NEQ LT LTE GT GTE

%left ADD SUB
%left MUL DIV MOD

%nonassoc IF
%nonassoc ELSE

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: CONFIGURATION COLON config[options] TRANSITION_FUNCTION COLON transition_sequence[exp]	{ $$ = TransitionProgramSemanticAction(currentCompilerState(), $options, $exp); }
	| CONFIGURATION COLON config[options] NEIGHBORHOOD_FUNCTION COLON neighborhood_sequence[exp]	{ $$ = NeighborhoodProgramSemanticAction(currentCompilerState(), $options, $exp); }
	| CONFIGURATION COLON config[options]														{ $$ = DefaultProgramSemanticAction(currentCompilerState(), $options); }
	;

config: option																			{ $$ = ConfigurationSemanticAction($1, NULL); }
	| option config																		{ $$ = ConfigurationSemanticAction($1, $2); }
	;

option: HEIGHT COLON INTEGER SEMICOLON														{ $$ = IntValuedOptionSemanticAction($3, HEIGHT_OPTION); }
    | WIDTH COLON INTEGER SEMICOLON															{ $$ = IntValuedOptionSemanticAction($3, WIDTH_OPTION); }
    | FRONTIER COLON FRONTIER_ENUM SEMICOLON													{ $$ = FrontierOptionSemanticAction($3); }
    | COLORS COLON OPEN_BRACE int_array CLOSE_BRACE SEMICOLON									{ $$ = IntArrayValuedOptionSemanticAction($4); }
    | STATES COLON OPEN_BRACE string_array CLOSE_BRACE SEMICOLON											{ $$ = StringArrayValuedOptionSemanticAction($4); }
    | NEIGHBORHOOD COLON NEIGHBORHOOD_ENUM SEMICOLON											{ $$ = NeighborhoodOptionSemanticAction($3); }
    | EVOLUTION COLON evolution[ev] SEMICOLON														{ $$ = EvolutionOptionSemanticAction($ev); }
	;

evolution: EVOLUTION_ENUM																{ $$ = EvolutionSemanticAction(NULL, NULL, $1); }
	| int_array DIV int_array															{ $$ = EvolutionSemanticAction($1, $3, 0); }

int_array: INTEGER																		{ $$ = IntArraySemanticAction($1, NULL); }
	| INTEGER COMMA int_array[arr]														{ $$ = IntArraySemanticAction($1, $arr); }
	;

string_array: STRING																	{ $$ = StringArraySemanticAction($1, NULL); }
	| STRING COMMA string_array[arr]													{ $$ = StringArraySemanticAction($1, $arr); }
	;

constant_array: constant																{ $$ = ConstantArraySemanticAction($1, NULL); }
	| constant COMMA constant_array[arr]												{ $$ = ConstantArraySemanticAction($1, $arr); }
	;

transition_sequence: transition_expression	transition_sequence 						{ $$ = TransitionBinarySequenceSemanticAction($2, $1); }
	| %empty																			{ $$ = NULL; }
	;

transition_expression: STRING ASSIGNMENT arithmetic_expression SEMICOLON						{ $$ = TransitionAssignmentExpressionSemanticAction($1, $3); }
	| FOR scope_entry for_variable_declaration DO transition_sequence scope_exit END									{ $$ = TransitionForLoopExpressionSemanticAction($3, $5); }
	| IF scope_entry arithmetic_expression THEN transition_sequence scope_exit END								{ $$ = TransitionIfExpressionSemanticAction($3, $5); }
	| IF scope_entry arithmetic_expression THEN transition_sequence scope_exit ELSE scope_entry transition_sequence scope_exit END 	{ $$ = TransitionIfElseExpressionSemanticAction($3, $5, $9); }
	| RETURN arithmetic_expression														{ $$ = TransitionReturnExpressionSemanticAction($2); }
	;

neighborhood_sequence: neighborhood_expression neighborhood_sequence					{ $$ = NeighborhoodBinarySequenceSemanticAction($2, $1); }
	| %empty																			{ $$ = NULL; }
	;

neighborhood_expression: STRING ASSIGNMENT arithmetic_expression SEMICOLON						{ $$ = NeighborhoodAssignmentExpressionSemanticAction($1, $3); }
	| FOR scope_entry for_variable_declaration DO neighborhood_sequence scope_exit END								{ $$ = NeighborhoodForLoopExpressionSemanticAction($3, $5); }
	| IF scope_entry arithmetic_expression THEN neighborhood_sequence scope_exit END							{ $$ = NeighborhoodIfExpressionSemanticAction($3, $5); }
	| IF scope_entry arithmetic_expression THEN neighborhood_sequence scope_exit ELSE scope_entry neighborhood_sequence END scope_exit { $$ = NeighborhoodIfElseExpressionSemanticAction($3, $5, $9); }
	| ADD_CELL OPEN_PARENTHESIS cell_list CLOSE_PARENTHESIS	SEMICOLON					{ $$ = NeighborhoodCellExpressionSemanticAction(true, $3); }
	| REMOVE_CELL OPEN_PARENTHESIS cell_list CLOSE_PARENTHESIS	SEMICOLON				{ $$ = NeighborhoodCellExpressionSemanticAction(false, $3); }
	;

for_variable_declaration: STRING IN range												{ $$ = ForVariableDeclarationSemanticAction($1, $3); }

cell: OPEN_PARENTHESIS constant[x] COMMA constant[y] CLOSE_PARENTHESIS									{ $$ = DoubleCoordinateCellSemanticAction($x, $y); }
	| DISPLACEMENT_TYPE OPEN_PARENTHESIS constant CLOSE_PARENTHESIS										{ $$ = SingleCoordinateCellSemanticAction($3, $1); }
	;

cell_list: cell																						{ $$ = CellListSemanticAction($1, NULL); }
	| cell COMMA cell_list																			{ $$ = CellListSemanticAction($1, $3); }
	;

range: OPEN_BRACE constant_array[array] CLOSE_BRACE														{ $$ = RangeSemanticAction($array, NULL, NULL); }
	| OPEN_BRACKET constant[c1] COMMA constant[c2] CLOSE_BRACKET										{ $$ = RangeSemanticAction(NULL, $c1, $c2); }
	;

arithmetic_expression: arithmetic_expression[left] ADD arithmetic_expression[right]					{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| arithmetic_expression[left] DIV arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| arithmetic_expression[left] MUL arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| arithmetic_expression[left] SUB arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| arithmetic_expression[left] MOD arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, MODULE); }
	| arithmetic_expression[left] AND arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, LOGIC_AND); }
	| arithmetic_expression[left] OR arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, LOGIC_OR); }
	| NOT arithmetic_expression[single]																{ $$ = UnaryArithmeticExpressionSemanticAction($single, LOGIC_NOT); }
	| arithmetic_expression[left] EQ arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, EQUALS); }
	| arithmetic_expression[left] NEQ arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, NOT_EQUALS); }
	| arithmetic_expression[left] LT arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, LOWER_THAN); }
	| arithmetic_expression[left] LTE arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, LOWER_THAN_OR_EQUAL); }
	| arithmetic_expression[left] GT arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, GREATER_THAN); }
	| arithmetic_expression[left] GTE arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, GREATER_THAN_OR_EQUAL); }
	| ALL_OPERAND OPEN_BRACE cell_list[single] CLOSE_BRACE ARE STRING[string]						{ $$ = CellListArithmeticExpressionSemanticAction($single, ALL_ARE, -1, $string); }
	| ANY OPEN_BRACE cell_list[single] CLOSE_BRACE ARE STRING[string]								{ $$ = CellListArithmeticExpressionSemanticAction($single, ANY_ARE, 1, $string); }
	| AT_LEAST INTEGER OPEN_BRACE cell_list[single] CLOSE_BRACE ARE STRING[string]					{ $$ = CellListArithmeticExpressionSemanticAction($single, AT_LEAST_ARE, $2, $string); }
	| OPEN_PARENTHESIS arithmetic_expression[single] CLOSE_PARENTHESIS								{ $$ = UnaryArithmeticExpressionSemanticAction($single, FACTOR); }
	| constant[single]																				{ $$ = ConstantArithmeticExpressionSemanticAction($single); }
	| cell																				 			{ $$ = CellArithmeticExpressionSemanticAction($1); }
	;

constant: INTEGER																					{ $$ = IntegerConstantSemanticAction($1); }
	| STRING																						{ $$ = StringConstantSemanticAction($1); }
	;

scope_entry:
    %empty { pushScope(currentCompilerState()->symbolTable); }

scope_exit:
    %empty { popScope(currentCompilerState()->symbolTable); }
%%
