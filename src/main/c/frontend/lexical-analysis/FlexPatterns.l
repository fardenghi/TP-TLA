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

"configuration:" 				    { return KeyWordLexemeAction(createLexicalAnalyzerContext(), CONFIGURATION); }
"Height:" 				            { return KeyWordLexemeAction(createLexicalAnalyzerContext(), HEIGHT); }
"Width:" 		        		    { return KeyWordLexemeAction(createLexicalAnalyzerContext(), WIDTH); }
"Frontier:"      				    { return KeyWordLexemeAction(createLexicalAnalyzerContext(), FRONTIER); }
"Colors:"          				    { return KeyWordLexemeAction(createLexicalAnalyzerContext(), COLORS); }
"States:"         				    { return KeyWordLexemeAction(createLexicalAnalyzerContext(), STATES); }
"Neighborhood:"      				{ return KeyWordLexemeAction(createLexicalAnalyzerContext(), NEIGHBORHOOD); }
"Evolution:"      		    		{ return KeyWordLexemeAction(createLexicalAnalyzerContext(), EVOLUTION); }
","                                 { return KeyWordLexemeAction(createLexicalAnalyzerContext(), COMMA); }
";"                                 { return KeyWordLexemeAction(createLexicalAnalyzerContext(), SEMICOLON); }
"{"                                 { return KeyWordLexemeAction(createLexicalAnalyzerContext(), OPEN_BRACE); }
"}"                                 { return KeyWordLexemeAction(createLexicalAnalyzerContext(), CLOSE_BRACE); }
"Periodic"                          { return FrontierLexemeAction(createLexicalAnalyzerContext(), PERIODIC); }
"Mirror"                            { return FrontierLexemeAction(createLexicalAnalyzerContext(), MIRROR); }
"MOORE"                             { return NeighborhoodLexemeAction(createLexicalAnalyzerContext(), MOORE); }
"CONWAY"                            { return EvolutionLexemeAction(createLexicalAnalyzerContext(), CONWAY); }



"if"                                { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), IF); }
"all"                               { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), ALL); }
"are"                               { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), ARE); }
"then"                              { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), THEN); }
"else"                              { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), ELSE); }
"endif"                             { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), ENDIF); }
"for"                               { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), FOR); }
"in"                                { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), IN); }
"do"                                { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), DO); }
"add"                               { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), ADD); }
"end"                               { return ControlFlowLexemeAction(createLexicalAnalyzerContext(), END); }

"->"                                { return ReturnOperatorLexemeAction(createLexicalAnalyzerContext()); }

"diag_asc"                          { return DirectionLexemeAction(createLexicalAnalyzerContext(), DIAG_ASC); }
"diag_dec"                          { return DirectionLexemeAction(createLexicalAnalyzerContext(), DIAG_DEC); }
"vert"                              { return DirectionLexemeAction(createLexicalAnalyzerContext(), VERT); }
"hor"                               { return DirectionLexemeAction(createLexicalAnalyzerContext(), HOR); }

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

"&&"                                { return LogicalOperatorLexemeAction(createLexicalAnalyzerContext(), AND); }
"||"                                { return LogicalOperatorLexemeAction(createLexicalAnalyzerContext(), OR); }
"!"                                 { return LogicalOperatorLexemeAction(createLexicalAnalyzerContext(), NOT); }

"("									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), OPEN_PARENTHESIS); }
")"									{ return ParenthesisLexemeAction(createLexicalAnalyzerContext(), CLOSE_PARENTHESIS); }

[[:digit:]]+						{ return IntegerLexemeAction(createLexicalAnalyzerContext()); }
[[:]]
[[:space:]]+						{ IgnoredLexemeAction(createLexicalAnalyzerContext()); }
.									{ return UnknownLexemeAction(createLexicalAnalyzerContext()); }


%%

#include "FlexExport.h"
