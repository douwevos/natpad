#ifndef JAGPARSERCONTEXT_H_
#define JAGPARSERCONTEXT_H_

#include <caterpillar.h>
#include "jagiscanner.h"

G_BEGIN_DECLS

#define JAG_TYPE_PARSER_CONTEXT            (jag_parser_context_get_type())
#define JAG_PARSER_CONTEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_parser_context_get_type(), JagParserContext))
#define JAG_PARSER_CONTEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PARSER_CONTEXT, JagParserContextClass))
#define JAG_IS_PARSER_CONTEXT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PARSER_CONTEXT))
#define JAG_IS_PARSER_CONTEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PARSER_CONTEXT))
#define JAG_PARSER_CONTEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PARSER_CONTEXT, JagParserContextClass))

typedef struct _JagParserContext       JagParserContext;
typedef struct _JagParserContextClass  JagParserContextClass;


struct _JagParserContext {
	GObject parent;
	JagIScanner *scanner;
	CatArrayWo *e_stack;

	/** Internal flag to indicate when parser should quit. */
	gboolean done_parsing;

	/** Indication of the index for top of stack (for use by actions). */
	int tos;

	/** The current lookahead Symbol. */
	JagToken *current_token;

	/** Lookahead Symbols used for attempting error recovery "parse aheads". */
	CatArrayWo *e_lookahead;

	/** Position in lookahead input buffer used for "parse ahead". */
	int lookahead_pos;

};


struct _JagParserContextClass {
	GObjectClass parent_class;
	void (*shift)(JagParserContext *parser_context, JagToken *token);
	void (*push)(JagParserContext *parser_context, JagToken *token);
	JagToken *(*pop)(JagParserContext *parser_context);
	JagToken *(*peek)(JagParserContext *parser_context);
	JagToken *(*getFromTop)(JagParserContext *parser_context, int reverseIndex);
	
	
	JagToken *(*scanNext)(JagParserContext *parserContext, int eofSymbol);

	JagToken *(*peek_at)(JagParserContext *parser_context, int reverse_index);
	void (*reduce)(JagParserContext *parser_context, int replace_count, JagToken *replace_with_symbol);
};


GType jag_parser_context_get_type(void);

JagParserContext *jag_parser_context_new(JagIScanner *scanner);

void jag_parser_context_construct(JagParserContext *parser_context, JagIScanner *scanner);


/**
 * Get the next Symbol from the input (supplied by generated subclass). Once
 * end of file has been reached, all subsequent calls to scan should return
 * an EOF Symbol (which is Symbol number 0). By default this method returns
 * getScanner().next_token(); this implementation can be overriden by the
 * generated parser using the code declared in the "scan with" clause. Do
 * not recycle objects; every call to scan() should return a fresh object.
 */
JagToken *jag_parser_context_scan_next_real(JagParserContext *parserContext, int eofSymbol);


/**
 * This method is called to indicate that the parser should quit. This is
 * normally called by an accept action, but can be used to cancel parsing
 * early in other circumstances if desired.
 */
void jag_parser_context_done_parsing(JagParserContext *parser_context);
JagToken *jag_parser_context_peek_real(JagParserContext *parser_context);
JagToken *jag_parser_context_pop_real(JagParserContext *parser_context);
void jag_parser_context_push_real(JagParserContext *parser_context, JagToken *token);
void jag_parser_context_shift_real(JagParserContext *parser_context, JagToken *token);
JagToken *jag_parser_context_get_from_top_real(JagParserContext *parser_context, int reverseIndex);

JagToken *jag_parser_context_peek_at_real(JagParserContext *parser_context, int reverse_index);
void jag_parser_context_reduce_real(JagParserContext *parser_context, int replace_count, JagToken *replace_with_symbol);





/** 
 *Return the current lookahead in our error "parse ahead" buffer. 
 */
JagToken *jag_parser_context_current_error_token(JagParserContext *parserContext);


void jag_parser_context_debug(JagParserContext *parser_context);


G_END_DECLS
#endif /* JAGPARSERCONTEXT_H_ */
