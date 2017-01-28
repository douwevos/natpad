#ifndef SHOPARSERCONTEXT_H_
#define SHOPARSERCONTEXT_H_

#include <caterpillar.h>
#include "shoiscanner.h"

G_BEGIN_DECLS

#define SHO_TYPE_PARSER_CONTEXT            (sho_parser_context_get_type())
#define SHO_PARSER_CONTEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_parser_context_get_type(), ShoParserContext))
#define SHO_PARSER_CONTEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_PARSER_CONTEXT, ShoParserContextClass))
#define SHO_IS_PARSER_CONTEXT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_PARSER_CONTEXT))
#define SHO_IS_PARSER_CONTEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_PARSER_CONTEXT))
#define SHO_PARSER_CONTEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_PARSER_CONTEXT, ShoParserContextClass))

typedef struct _ShoParserContext       ShoParserContext;
typedef struct _ShoParserContextClass  ShoParserContextClass;


struct _ShoParserContext {
	GObject parent;
	ShoIScanner *scanner;
	CatArrayWo *e_stack;

	/** Internal flag to indicate when parser should quit. */
	gboolean done_parsing;

	/** Indication of the index for top of stack (for use by actions). */
	int tos;

	/** The current lookahead Symbol. */
	ShoToken *current_token;

	/** Lookahead Symbols used for attempting error recovery "parse aheads". */
	CatArrayWo *e_lookahead;

	/** Position in lookahead input buffer used for "parse ahead". */
	int lookahead_pos;

};


struct _ShoParserContextClass {
	GObjectClass parent_class;
	void (*shift)(ShoParserContext *parser_context, ShoToken *token);
	void (*push)(ShoParserContext *parser_context, ShoToken *token);
	ShoToken *(*pop)(ShoParserContext *parser_context);
	ShoToken *(*peek)(ShoParserContext *parser_context);
	ShoToken *(*getFromTop)(ShoParserContext *parser_context, int reverseIndex);
	
	
	ShoToken *(*scanNext)(ShoParserContext *parserContext, int eofSymbol);

	ShoToken *(*peek_at)(ShoParserContext *parser_context, int reverse_index);
	void (*reduce)(ShoParserContext *parser_context, int replace_count, ShoToken *replace_with_symbol);
};


GType sho_parser_context_get_type(void);

ShoParserContext *sho_parser_context_new(ShoIScanner *scanner);

void sho_parser_context_construct(ShoParserContext *parser_context, ShoIScanner *scanner);


/**
 * Get the next Symbol from the input (supplied by generated subclass). Once
 * end of file has been reached, all subsequent calls to scan should return
 * an EOF Symbol (which is Symbol number 0). By default this method returns
 * getScanner().next_token(); this implementation can be overriden by the
 * generated parser using the code declared in the "scan with" clause. Do
 * not recycle objects; every call to scan() should return a fresh object.
 */
ShoToken *sho_parser_context_scan_next_real(ShoParserContext *parserContext, int eofSymbol);


/**
 * This method is called to indicate that the parser should quit. This is
 * normally called by an accept action, but can be used to cancel parsing
 * early in other circumstances if desired.
 */
void sho_parser_context_done_parsing(ShoParserContext *parser_context);
ShoToken *sho_parser_context_peek_real(ShoParserContext *parser_context);
ShoToken *sho_parser_context_pop_real(ShoParserContext *parser_context);
void sho_parser_context_push_real(ShoParserContext *parser_context, ShoToken *token);
void sho_parser_context_shift_real(ShoParserContext *parser_context, ShoToken *token);
ShoToken *sho_parser_context_get_from_top_real(ShoParserContext *parser_context, int reverseIndex);

ShoToken *sho_parser_context_peek_at_real(ShoParserContext *parser_context, int reverse_index);
void sho_parser_context_reduce_real(ShoParserContext *parser_context, int replace_count, ShoToken *replace_with_symbol);


/** 
 *Return the current lookahead in our error "parse ahead" buffer. 
 */
ShoToken *sho_parser_context_current_error_token(ShoParserContext *parserContext);


void sho_parser_context_debug(ShoParserContext *parser_context);


G_END_DECLS
#endif /* SHOPARSERCONTEXT_H_ */
