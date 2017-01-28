#ifndef [%PRE%]PARSERCONTEXT_H_
#define [%PRE%]PARSERCONTEXT_H_

#include "[%pre%]iscanner.h"
#include "[%pre%]vector.h"

typedef struct _[%Pre%]ParserContext       [%Pre%]ParserContext;

struct _[%Pre%]ParserContext {
//	[%Pre%]IScanner *scanner;
	[%Pre%]Vector *stack;

	/** Internal flag to indicate when parser should quit. */
	[%pre%]boolean done_parsing;

	/** Indication of the index for top of stack (for use by actions). */
	int tos;

	/** The current lookahead Symbol. */
	[%Pre%]Token *current_token;

	/** Lookahead Symbols used for attempting error recovery "parse aheads". */
	[%Pre%]Vector *lookahead;

	/** Position in lookahead input buffer used for "parse ahead". */
	int lookahead_pos;

};


[%Pre%]ParserContext *[%pre%]_parser_context_new();


void [%pre%]_parser_context_done_parsing([%Pre%]ParserContext *parser_context);
[%Pre%]Token *[%pre%]_parser_context_peek([%Pre%]ParserContext *parser_context);
[%Pre%]Token *[%pre%]_parser_context_pop([%Pre%]ParserContext *parser_context);
void [%pre%]_parser_context_push([%Pre%]ParserContext *parser_context, [%Pre%]Token *token);
void [%pre%]_parser_context_shift([%Pre%]ParserContext *parser_context, [%Pre%]Token *token);
[%Pre%]Token *[%pre%]_parser_context_get_from_top([%Pre%]ParserContext *parser_context, int reverseIndex);

[%Pre%]Token *[%pre%]_parser_context_current_error_token([%Pre%]ParserContext *parserContext);

[%Pre%]Token *[%pre%]_parser_context_debug([%Pre%]ParserContext *parser_context);


#endif /* [%PRE%]PARSERCONTEXT_H_ */
