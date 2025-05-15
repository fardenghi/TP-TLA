%{

#include "BisonActions.h"

%}

// You touch this, and you die.
%define api.value.union.name SemanticValue

%union {
	/** Terminals. */

	int integer;
	FrontierEnum frontier;
	NeighborhoodEnum neighborhood;
	EvolutionEnum evolution;
	char * string;
	Token token;

	/** Non-terminals. */

	Constant * constant;
	Expression * expression;
	TransitionExpression * transition_expression;
	NeighborhoodExpression * neighborhood_expression;
	ArithmeticExpression * arithmetic_expression;
	Factor * factor;
	Program * program;
	Configuration * configuration;
	Option * option;
	IntArray * int_array;
	StringArray * string_array;
	Cell * cell;
	CellList * cell_list;
	Range * range;
}

/**
 * Destructors. This functions are executed after the parsing ends, so if the
 * AST must be used in the following phases of the compiler you shouldn't used
 * this approach. To use this mechanism, the AST must be translated into
 * another structure.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Destructor-Decl.html
 */
/*
%destructor { releaseConstant($$); } <constant>
%destructor { releaseExpression($$); } <expression>
%destructor { releaseFactor($$); } <factor>
%destructor { releaseProgram($$); } <program>
*/

/** Terminals. */
%token <integer> INTEGER
%token <string> STRING
%token <frontier> FRONTIER_ENUM
%token <neighborhood> NEIGHBORHOOD_ENUM
%token <evolution> EVOLUTION_ENUM

%token <token> OPEN_PARENTHESIS
%token <token> CLOSE_PARENTHESIS

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

%token <token> ALL
%token <token> ANY
%token <token> AT_LEAST
%token <token> ARE

%token <token> OPEN_BRACE
%token <token> CLOSE_BRACE
%token <token> OPEN_BRACKET
%token <token> CLOSE_BRACKET
%token <token> COMMA
%token <token> SEMICOLON

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
%token <token> DIAG_ASC
%token <token> DIAG_DESC
%token <token> HORIZONTAL
%token <token> VERTICAL


%token <token> UNKNOWN

/** Non-terminals. */
%type <program> program
%type <transition_expression> transition_expression
%type <neighborhood_expression> neighborhood_expression
%type <arithmetic_expression> arithmetic_expression
%type <constant> constant
%type <option> option
%type <evolution> evolution
%type <int_array> int_array
%type <string_array> string_array
%type <cell> cell
%type <cell_list> cell_list
%type <range> range

/**
 * Precedence and associativity.
 *
 * @see https://www.gnu.org/software/bison/manual/html_node/Precedence.html
 */
%left ADD SUB
%left MUL DIV

%%

// IMPORTANT: To use λ in the following grammar, use the %empty symbol.

program: CONFIGURATION option[options] TRANSITION_FUNCTION transition_expression[exp]	{ $$ = TransitionProgramSemanticAction(currentCompilerState(), $options, $exp)}
	| CONFIGURATION option[options] NEIGHBORHOOD_FUNCTION neighborhood_expression[exp]	{ $$ = NeighborhoodProgramSemanticAction(currentCompilerState(), $options, $exp)}
	| CONFIGURATION option[options]														{ $$ = DefaultProgramSemanticAction(currentCompilerState(), $options)}
	;

option: option
	| option option
	| HEIGHT INTEGER SEMICOLON
    | WIDTH INTEGER SEMICOLON
    | FRONTIER FRONTIER_ENUM SEMICOLON
    | COLORS OPEN_BRACE int_array CLOSE_BRACE SEMICOLON
    | STATES OPEN_BRACE string_array SEMICOLON
    | NEIGHBORHOOD NEIGHBORHOOD_ENUM SEMICOLON
    | EVOLUTION evolution SEMICOLON
	;

evolution: EVOLUTION_ENUM
	| int_array DIV INTEGER

int_array: INTEGER																		{$$ = IntArraySemanticAction($1, NULL)}
	| INTEGER COMMA int_array[arr]														{$$ = IntArraySemanticAction($1, $arr)}
	;

string_array: STRING
	| STRING COMMA STRING
	;

transition_expression: transition_expression transition_expression
	| STRING EQ arithmetic_expression SEMICOLON
	| FOR STRING IN range DO transition_expression END
	| IF arithmetic_expression THEN transition_expression END
	| IF arithmetic_expression THEN transition_expression ELSE transition_expression END
	| RETURN STRING
	| RETURN arithmetic_expression
	;

neighborhood_expression: neighborhood_expression neighborhood_expression
	| STRING EQ arithmetic_expression SEMICOLON
	| FOR STRING IN range DO neighborhood_expression END
	| IF arithmetic_expression THEN neighborhood_expression END
	| IF arithmetic_expression THEN neighborhood_expression ELSE neighborhood_expression END
	| ADD_CELL OPEN_PARENTHESIS cell_list CLOSE_PARENTHESIS
	| REMOVE_CELL OPEN_PARENTHESIS cell_list CLOSE_PARENTHESIS
	;

cell: OPEN_PARENTHESIS constant COMMA constant CLOSE_PARENTHESIS
	| HORIZONTAL OPEN_PARENTHESIS constant CLOSE_PARENTHESIS
	| VERTICAL OPEN_PARENTHESIS constant CLOSE_PARENTHESIS
	| DIAG_ASC OPEN_PARENTHESIS constant CLOSE_PARENTHESIS
	| DIAG_DESC OPEN_PARENTHESIS constant CLOSE_PARENTHESIS
	;

cell_list: cell
	| cell COMMA cell_list
	;

range: OPEN_BRACKET int_array CLOSE_BRACKET
	| OPEN_BRACKET constant SUB constant CLOSE_BRACKET
	;

arithmetic_expression: arithmetic_expression[left] ADD arithmetic_expression[right]					{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, ADDITION); }
	| arithmetic_expression[left] DIV arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, DIVISION); }
	| arithmetic_expression[left] MUL arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, MULTIPLICATION); }
	| arithmetic_expression[left] SUB arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, SUBTRACTION); }
	| arithmetic_expression[left] MOD arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, MODULE); }
	| arithmetic_expression[left] AND arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, LOGIC_AND); }
	| arithmetic_expression[left] OR arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, LOGIC_OR); }
	| NOT arithmetic_expression[single]
	| arithmetic_expression[left] EQ arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, EQUALS); }
	| arithmetic_expression[left] NEQ arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, NOT_EQUALS); }
	| arithmetic_expression[left] LT arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, LOWER_THAN); }
	| arithmetic_expression[left] LTE arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, LOWER_THAN_OR_EQUAL); }
	| arithmetic_expression[left] GT arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, GREATER_THAN); }
	| arithmetic_expression[left] GTE arithmetic_expression[right]									{ $$ = BinaryArithmeticExpressionSemanticAction($left, $right, GREATER_THAN_OR_EQUAL); }
	| ALL OPEN_BRACE cell_list[single] CLOSE_BRACE ARE STRING										{ $$ = CellListArithmeticExpressionSemanticAction($single, ALL_ARE); }
	| ANY OPEN_BRACE cell_list[single] CLOSE_BRACE ARE STRING										{ $$ = CellListArithmeticExpressionSemanticAction($single, ANY_ARE); }
	| AT_LEAST INTEGER OPEN_BRACE cell_list[single] CLOSE_BRACE ARE STRING							{ $$ = CellListArithmeticExpressionSemanticAction($single, AT_LEAST_ARE); }
	| OPEN_PARENTHESIS arithmetic_expression[single] CLOSE_PARENTHESIS								{ $$ = UnaryArithmeticExpressionSemanticAction($single, FACTOR); }
	| constant[single]																				{ $$ = UnaryArithmeticExpressionSemanticAction($single, CONSTANT); }
	;

constant: INTEGER																					{ $$ = IntegerConstantSemanticAction($1); }
	| STRING																						{ $$ = StringConstantSemanticAction($1); }
	;


%%
