#ifndef [%PRE%]PARSERCONTEXT_H_
#define [%PRE%]PARSERCONTEXT_H_

#include <caterpillar.h>
#include "[%pre%]iscanner.h"

G_BEGIN_DECLS

#define [%PRE%]_TYPE_PARSER_CONTEXT            ([%pre%]_parser_context_get_type())
#define [%PRE%]_PARSER_CONTEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), [%pre%]_parser_context_get_type(), [%Pre%]ParserContext))
#define [%PRE%]_PARSER_CONTEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), [%PRE%]_TYPE_PARSER_CONTEXT, [%Pre%]ParserContextClass))
#define [%PRE%]_IS_PARSER_CONTEXT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), [%PRE%]_TYPE_PARSER_CONTEXT))
#define [%PRE%]_IS_PARSER_CONTEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), [%PRE%]_TYPE_PARSER_CONTEXT))
#define [%PRE%]_PARSER_CONTEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), [%PRE%]_TYPE_PARSER_CONTEXT, [%Pre%]ParserContextClass))

typedef struct _[%Pre%]ParserContext       [%Pre%]ParserContext;
typedef struct _[%Pre%]ParserContextClass  [%Pre%]ParserContextClass;


struct _[%Pre%]ParserContext {
	GObject parent;
	[%Pre%]IScanner *scanner;
	CatArrayWo *e_stack;

	/** Internal flag to indicate when parser should quit. */
	gboolean done_parsing;

	/** Indication of the index for top of stack (for use by actions). */
	int tos;

	/** The current lookahead Symbol. */
	[%Pre%]Token *current_token;

	/** Lookahead Symbols used for attempting error recovery "parse aheads". */
	CatArrayWo *e_lookahead;

	/** Position in lookahead input buffer used for "parse ahead". */
	int lookahead_pos;

};


struct _[%Pre%]ParserContextClass {
	GObjectClass parent_class;
	void (*shift)([%Pre%]ParserContext *parser_context, [%Pre%]Token *token);
	void (*push)([%Pre%]ParserContext *parser_context, [%Pre%]Token *token);
	[%Pre%]Token *(*pop)([%Pre%]ParserContext *parser_context);
	[%Pre%]Token *(*peek)([%Pre%]ParserContext *parser_context);
	[%Pre%]Token *(*getFromTop)([%Pre%]ParserContext *parser_context, int reverseIndex);
	
	
	[%Pre%]Token *(*scanNext)([%Pre%]ParserContext *parserContext, int eofSymbol);

	[%Pre%]Token *(*peek_at)([%Pre%]ParserContext *parser_context, int reverse_index);
	void (*reduce)([%Pre%]ParserContext *parser_context, int replace_count, [%Pre%]Token *replace_with_symbol);
};


GType [%pre%]_parser_context_get_type(void);

[%Pre%]ParserContext *[%pre%]_parser_context_new([%Pre%]IScanner *scanner);

void [%pre%]_parser_context_construct([%Pre%]ParserContext *parser_context, [%Pre%]IScanner *scanner);


/**
 * Get the next Symbol from the input (supplied by generated subclass). Once
 * end of file has been reached, all subsequent calls to scan should return
 * an EOF Symbol (which is Symbol number 0). By default this method returns
 * getScanner().next_token(); this implementation can be overriden by the
 * generated parser using the code declared in the "scan with" clause. Do
 * not recycle objects; every call to scan() should return a fresh object.
 */
[%Pre%]Token *[%pre%]_parser_context_scan_next_real([%Pre%]ParserContext *parserContext, int eofSymbol);


/**
 * This method is called to indicate that the parser should quit. This is
 * normally called by an accept action, but can be used to cancel parsing
 * early in other circumstances if desired.
 */
void [%pre%]_parser_context_done_parsing([%Pre%]ParserContext *parser_context);
[%Pre%]Token *[%pre%]_parser_context_peek_real([%Pre%]ParserContext *parser_context);
[%Pre%]Token *[%pre%]_parser_context_pop_real([%Pre%]ParserContext *parser_context);
void [%pre%]_parser_context_push_real([%Pre%]ParserContext *parser_context, [%Pre%]Token *token);
void [%pre%]_parser_context_shift_real([%Pre%]ParserContext *parser_context, [%Pre%]Token *token);
[%Pre%]Token *[%pre%]_parser_context_get_from_top_real([%Pre%]ParserContext *parser_context, int reverseIndex);

[%Pre%]Token *[%pre%]_parser_context_peek_at_real([%Pre%]ParserContext *parser_context, int reverse_index);
void [%pre%]_parser_context_reduce_real([%Pre%]ParserContext *parser_context, int replace_count, [%Pre%]Token *replace_with_symbol);





/** 
 *Return the current lookahead in our error "parse ahead" buffer. 
 */
[%Pre%]Token *[%pre%]_parser_context_current_error_token([%Pre%]ParserContext *parserContext);

[%if-set:debug%]
void [%pre%]_parser_context_debug([%Pre%]ParserContext *parser_context);
[%end-if:debug%]

G_END_DECLS
#endif /* [%PRE%]PARSERCONTEXT_H_ */
