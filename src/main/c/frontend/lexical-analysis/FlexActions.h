#ifndef FLEX_ACTIONS_HEADER
#define FLEX_ACTIONS_HEADER

#include "../../shared/Environment.h"
#include "../../shared/Logger.h"
#include "../../shared/String.h"
#include "../../shared/Type.h"
#include "../syntactic-analysis/AbstractSyntaxTree.h"
#include "../syntactic-analysis/BisonParser.h"
#include "LexicalAnalyzerContext.h"
#include <stdio.h>
#include <stdlib.h>

/** Initialize module's internal state. */
void initializeFlexActionsModule();

/** Shutdown module's internal state. */
void shutdownFlexActionsModule();

/**
 * Flex lexeme processing actions.
 */

void BeginMultilineCommentLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
void EndMultilineCommentLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
void IgnoredLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);

Token ArithmeticOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token IntegerLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token ParenthesisLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);

Token IntegerLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token ColorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);

Token StringLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);

Token FrontierLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, FrontierEnum frontier);
Token NeighborhoodLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, NeighborhoodEnum neighborhood);
Token EvolutionLexemeAction(LexicalAnalyzerContext * lexicalAnalyzerContext, EvolutionEnum evolution);

Token DirectionLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, DisplacementType type);

Token ReturnOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);

Token KeyWordLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token ControlFlowLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token ReturnOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);
Token LogicalOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);
Token RelationalOperatorLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext, Token token);

Token UnknownLexemeAction(LexicalAnalyzerContext *lexicalAnalyzerContext);

#endif
