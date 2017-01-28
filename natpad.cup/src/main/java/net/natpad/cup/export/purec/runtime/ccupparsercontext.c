#include "[%pre%]parsercontext.h"
#include <stdlib.h>
#include <stdio.h>

void [%pre%]_parser_context_dispose([%Pre%]ParserContext *context) {
	if (context) {
		free(context);
	}
}


[%Pre%]ParserContext *[%pre%]_parser_context_new() {
	[%Pre%]ParserContext *result = ([%Pre%]ParserContext *) malloc(sizeof([%Pre%]ParserContext));
//	result->scanner = scanner;
	result->stack = [%pre%]_vector_new(NULL);
	result->done_parsing = 0;
	result->tos = 0;
	result->current_token = NULL;
	result->lookahead = NULL;
	return result;
}

/**
 * This method is called to indicate that the parser should quit. This is
 * normally called by an accept action, but can be used to cancel parsing
 * early in other circumstances if desired.
 */
void [%pre%]_parser_context_done_parsing([%Pre%]ParserContext *parser_context) {
	parser_context->done_parsing = -1;
}


[%Pre%]Token *[%pre%]_parser_context_peek([%Pre%]ParserContext *parser_context) {
	int idx = [%pre%]_vector_count(parser_context->stack);
	if (idx<=0) {
		return NULL;
	}
	return ([%Pre%]Token *) [%pre%]_vector_get_at(parser_context->stack, idx-1);
}


[%Pre%]Token *[%pre%]_parser_context_pop([%Pre%]ParserContext *parser_context) {
	int idx = [%pre%]_vector_count(parser_context->stack);
	if (idx<=0) {
		return NULL;
	}
	parser_context->tos--;
	[%Pre%]Token *result = NULL;
	result = ([%Pre%]Token *) [%pre%]_vector_pop(parser_context->stack);
	// TODO provide a callback routine for freeing tokens
	return result;
}


void [%pre%]_parser_context_push([%Pre%]ParserContext *parser_context, [%Pre%]Token *token) {
	[%pre%]_vector_add(parser_context->stack, token);
}




void [%pre%]_parser_context_shift([%Pre%]ParserContext *parser_context, [%Pre%]Token *token) {
	[%pre%]_vector_add(parser_context->stack, token);
	parser_context->tos++;
}

[%Pre%]Token *[%pre%]_parser_context_get_from_top([%Pre%]ParserContext *parser_context, int reverseIndex) {
	int idx = parser_context->tos-reverseIndex;
	if (idx<0 || idx>=[%pre%]_vector_count(parser_context->stack)) {
		return NULL;
	}

	return ([%Pre%]Token *) [%pre%]_vector_get_at(parser_context->stack, idx);
}


/** Return the current lookahead in our error "parse ahead" buffer. */
[%Pre%]Token *[%pre%]_parser_context_current_error_token([%Pre%]ParserContext *parserContext) {
	return ([%Pre%]Token *) [%pre%]_vector_get_at(parserContext->lookahead, parserContext->lookahead_pos);
}



[%Pre%]Token *[%pre%]_parser_context_debug([%Pre%]ParserContext *parser_context) {
	int stackIdx;
	for(stackIdx=0; stackIdx<[%pre%]_vector_count(parser_context->stack); stackIdx++) {
		[%Pre%]Token *token = ([%Pre%]Token *) [%pre%]_vector_get_at(parser_context->stack, stackIdx);
		if (token!=NULL) {
			printf("stack[%d%]=Token[sym=%d, state=%d[%if-set:debug%], text=%s[%end-if:debug%]]\n", stackIdx, token->sym, token->parse_state[%if-set:debug%], token->symbol_text[%end-if:debug%]);
		}
	}
}
