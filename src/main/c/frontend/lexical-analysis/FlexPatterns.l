%{

#include "FlexActions.h"

%}

/**
 * Enable start condition manipulation functions.
 */
%option stack

/**
 * Flex contexts (a.k.a. start conditions).
 *
 * @see https://westes.github.io/flex/manual/Start-Conditions.html
 */
%x MULTILINE_COMMENT

/**
 * Reusable patterns.
 *
 * @see https://westes.github.io/flex/manual/Matching.html
 * @see https://westes.github.io/flex/manual/Patterns.html
 */

%%

"/*"								{ BEGIN(MULTILINE_COMMENT); BeginMultilineCommentLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>"*/"				{ EndMultilineCommentLexemeAction(createLexicalAnalyzerContext()); BEGIN(INITIAL); }
<MULTILINE_COMMENT>[[:space:]]+		{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>[^*]+			{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
<MULTILINE_COMMENT>.				{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }

"configuration" 				    { return KeyWordLexemeAction(createLexicalAnalyzerContext(), CONFIGURATION); }
"transition" 				        { return KeyWordLexemeAction(createLexicalAnalyzerContext(), TRANSITION_FUNCTION); }
"neighborhood" 				        { return KeyWordLexemeAction(createLexicalAnalyzerContext(), NEIGHBORHOOD_FUNCTION); }
"Height" 				            { return KeyWordLexemeAction(createLexicalAnalyzerContext(), HEIGHT); }
"Width" 		        		    { return KeyWordLexemeAction(createLexicalAnalyzerContext(), WIDTH); }
"Frontier"      				    { return KeyWordLexemeAction(createLexicalAnalyzerContext(), FRONTIER); }
"Colors"          				    { return KeyWordLexemeAction(createLexicalAnalyzerContext(), COLORS); }
"States"         				    { return KeyWordLexemeAction(createLexicalAnalyzerContext(), STATES); }
"Neighborhood"      				{ return KeyWordLexemeAction(createLexicalAnalyzerContext(), NEIGHBORHOOD); }
"Evolution"      		    		{ return KeyWordLexemeAction(createLexicalAnalyzerContext(), EVOLUTION); }
","                                 { return KeyWordLexemeAction(createLexicalAnalyzerContext(), COMMA); }
";"                                 { return KeyWordLexemeAction(createLexicalAnalyzerContext(), SEMICOLON); }
":"                                 { return KeyWordLexemeAction(createLexicalAnalyzerContext(), COLON); }
"{"                                 { return KeyWordLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }
"}"                                 { return KeyWordLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE); }
"["                                 { return KeyWordLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACKET); }
"]"                                 { return KeyWordLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACKET); }
"Periodic"                          { return FrontierLexemeAction(createLexicalAnalyzerContext(), PERIODIC); }
"Mirror"                            { return FrontierLexemeAction(createLexicalAnalyzerContext(), MIRROR); }
"Open"                              { return FrontierLexemeAction(createLexicalAnalyzerContext(), OPEN); }
"MOORE"                             { return NeighborhoodLexemeAction(createLexicalAnalyzerContext(), MOORE); }
"CUSTOM"                            { return NeighborhoodLexemeAction(createLexicalAnalyzerContext(), CUSTOM); }
"VON_NEUMANN"                       { return NeighborhoodLexemeAction(createLexicalAnalyzerContext(), VON_NEUMANN); }
"K_NEIGHBORHOOD"                    { return NeighborhoodLexemeAction(createLexicalAnalyzerContext(), K_NEIGHBORHOOD); }
"CONWAY"                            { return EvolutionLexemeAction(createLexicalAnalyzerContext(), CONWAY); }



"if"                                { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), IF); }
"all"                               { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), ALL_OPERAND); }
"are"                               { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), ARE); }
"then"                              { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), THEN); }
"else"                              { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), ELSE); }
"end"                               { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), END); }
"for"                               { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), FOR); }
"in"                                { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), IN); }
"do"                                { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), DO); }
"add"                               { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), ADD_CELL); }
"remove"                               { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), REMOVE_CELL); }

"->"                                { return ReturnOperatorLexemeAction(createLexicalAnalyzerContext()); }

"diag_asc"                          { return DirectionLexemeAction(createLexicalAnalyzerContext(), DIAGONAL_ASC_D); }
"diag_dec"                          { return DirectionLexemeAction(createLexicalAnalyzerContext(), DIAGONAL_DESC_D); }
"vert"                              { return DirectionLexemeAction(createLexicalAnalyzerContext(), VERTICAL_D); }
"hor"                               { return DirectionLexemeAction(createLexicalAnalyzerContext(), HORIZONTAL_D); }

"-"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), SUB); }
"*"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MUL); }
"/"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), DIV); }
"+"									{ return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), ADD); }

"%"                                 { return ArithmeticOperatorLexemeAction(createLexicalAnalyzerContext(), MOD); }

"=="                                { return RelationalOperatorLexemeAction(createLexicalAnalyzerContext(), EQ); }
"!="                                { return RelationalOperatorLexemeAction(createLexicalAnalyzerContext(), NEQ); }
"<"                                 { return RelationalOperatorLexemeAction(createLexicalAnalyzerContext(), LT); }
"<="                                { return RelationalOperatorLexemeAction(createLexicalAnalyzerContext(), LTE); }
">"                                 { return RelationalOperatorLexemeAction(createLexicalAnalyzerContext(), GT); }
">="                                { return RelationalOperatorLexemeAction(createLexicalAnalyzerContext(), GTE); }

"="                                 { return KeyWordLexemeAction(createLexicalAnalyzerContext(), ASSIGNMENT); }

"&&"                                { return LogicalOperatorLexemeAction(createLexicalAnalyzerContext(), AND); }
"||"                                { return LogicalOperatorLexemeAction(createLexicalAnalyzerContext(), OR); }
"!"                                 { return LogicalOperatorLexemeAction(createLexicalAnalyzerContext(), NOT); }

"("									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }

[[:digit:]]+						{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }
[[:alnum:]]+                        { return StringLexemeAction(createLexicalAnalyzerContext()); }
[[:]]
[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }


%%

#include "FlexExport.h"
