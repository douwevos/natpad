#ifndef GROPARSERCONTEXT_H_
#define GROPARSERCONTEXT_H_

#include "groiscanner.h"

G_BEGIN_DECLS

#define GRO_TYPE_PARSER_CONTEXT            (gro_parser_context_get_type())
#define GRO_PARSER_CONTEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_parser_context_get_type(), GroParserContext))
#define GRO_PARSER_CONTEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_PARSER_CONTEXT, GroParserContextClass))
#define GRO_IS_PARSER_CONTEXT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_PARSER_CONTEXT))
#define GRO_IS_PARSER_CONTEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_PARSER_CONTEXT))
#define GRO_PARSER_CONTEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_PARSER_CONTEXT, GroParserContextClass))

typedef struct _GroParserContext       GroParserContext;
typedef struct _GroParserContextClass  GroParserContextClass;


struct _GroParserContext {
	GObject parent;
	GroIScanner *scanner;
	GPtrArray *e_stack;

	/** Internal flag to indicate when parser should quit. */
	gboolean done_parsing;

	/** Indication of the index for top of stack (for use by actions). */
	int tos;

	/** The current lookahead Symbol. */
	GroToken *current_token;

	/** Lookahead Symbols used for attempting error recovery "parse aheads". */
	GPtrArray *e_lookahead;

	/** Position in lookahead input buffer used for "parse ahead". */
	int lookahead_pos;

};


struct _GroParserContextClass {
	GObjectClass parent_class;
	void (*shift)(GroParserContext *parser_context, GroToken *token);
	void (*push)(GroParserContext *parser_context, GroToken *token);
	GroToken *(*pop)(GroParserContext *parser_context);
	GroToken *(*peek)(GroParserContext *parser_context);
	GroToken *(*getFromTop)(GroParserContext *parser_context, int reverseIndex);
	
	
	GroToken *(*scanNext)(GroParserContext *parserContext, int eofSymbol);

	GroToken *(*peek_at)(GroParserContext *parser_context, int reverse_index);
	void (*reduce)(GroParserContext *parser_context, int replace_count, GroToken *replace_with_symbol);
};


GType gro_parser_context_get_type(void);

GroParserContext *gro_parser_context_new(GroIScanner *scanner);

void gro_parser_context_construct(GroParserContext *parser_context, GroIScanner *scanner);


/**
 * Get the next Symbol from the input (supplied by generated subclass). Once
 * end of file has been reached, all subsequent calls to scan should return
 * an EOF Symbol (which is Symbol number 0). By default this method returns
 * getScanner().next_token(); this implementation can be overriden by the
 * generated parser using the code declared in the "scan with" clause. Do
 * not recycle objects; every call to scan() should return a fresh object.
 */
GroToken *gro_parser_context_scan_next_real(GroParserContext *parserContext, int eofSymbol);


/**
 * This method is called to indicate that the parser should quit. This is
 * normally called by an accept action, but can be used to cancel parsing
 * early in other circumstances if desired.
 */
void gro_parser_context_done_parsing(GroParserContext *parser_context);
GroToken *gro_parser_context_peek_real(GroParserContext *parser_context);
GroToken *gro_parser_context_pop_real(GroParserContext *parser_context);
void gro_parser_context_push_real(GroParserContext *parser_context, GroToken *token);
void gro_parser_context_shift_real(GroParserContext *parser_context, GroToken *token);
GroToken *gro_parser_context_get_from_top_real(GroParserContext *parser_context, int reverseIndex);

GroToken *gro_parser_context_peek_at_real(GroParserContext *parser_context, int reverse_index);
void gro_parser_context_reduce_real(GroParserContext *parser_context, int replace_count, GroToken *replace_with_symbol);

void gro_parser_context_dump(GroParserContext *parser_context);



/** 
 *Return the current lookahead in our error "parse ahead" buffer. 
 */
GroToken *gro_parser_context_current_error_token(GroParserContext *parserContext);



G_END_DECLS
#endif /* GROPARSERCONTEXT_H_ */
