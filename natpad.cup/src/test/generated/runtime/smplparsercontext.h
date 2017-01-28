#ifndef SMPLPARSERCONTEXT_H_
#define SMPLPARSERCONTEXT_H_

#include <caterpillar.h>
#include "smpliscanner.h"

G_BEGIN_DECLS

#define SMPL_TYPE_PARSER_CONTEXT            (smpl_parser_context_get_type())
#define SMPL_PARSER_CONTEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), smpl_parser_context_get_type(), SmplParserContext))
#define SMPL_PARSER_CONTEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SMPL_TYPE_PARSER_CONTEXT, SmplParserContextClass))
#define SMPL_IS_PARSER_CONTEXT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SMPL_TYPE_PARSER_CONTEXT))
#define SMPL_IS_PARSER_CONTEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SMPL_TYPE_PARSER_CONTEXT))
#define SMPL_PARSER_CONTEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SMPL_TYPE_PARSER_CONTEXT, SmplParserContextClass))

typedef struct _SmplParserContext       SmplParserContext;
typedef struct _SmplParserContextClass  SmplParserContextClass;


struct _SmplParserContext {
	GObject parent;
	SmplIScanner *scanner;
	CatArray *stack;

	/** Internal flag to indicate when parser should quit. */
	gboolean done_parsing;

	/** Indication of the index for top of stack (for use by actions). */
	int tos;

	/** The current lookahead Symbol. */
	SmplToken *current_token;

	/** Lookahead Symbols used for attempting error recovery "parse aheads". */
	CatArray *lookahead;

	/** Position in lookahead input buffer used for "parse ahead". */
	int lookahead_pos;

};


struct _SmplParserContextClass {
	GObjectClass parent_class;
	void (*shift)(SmplParserContext *parser_context, SmplToken *token);
	void (*push)(SmplParserContext *parser_context, SmplToken *token);
	SmplToken *(*pop)(SmplParserContext *parser_context);
	SmplToken *(*peek)(SmplParserContext *parser_context);
	SmplToken *(*getFromTop)(SmplParserContext *parser_context, int reverseIndex);
	
	
	SmplToken *(*scanNext)(SmplParserContext *parserContext, int eofSymbol);

	SmplToken *(*peek_at)(SmplParserContext *parser_context, int reverse_index);
	void (*reduce)(SmplParserContext *parser_context, int replace_count, SmplToken *replace_with_symbol);
};


GType smpl_parser_context_get_type(void);

SmplParserContext *smpl_parser_context_new(SmplIScanner *scanner);

void smpl_parser_context_construct(SmplParserContext *parser_context, SmplIScanner *scanner);


/**
 * Get the next Symbol from the input (supplied by generated subclass). Once
 * end of file has been reached, all subsequent calls to scan should return
 * an EOF Symbol (which is Symbol number 0). By default this method returns
 * getScanner().next_token(); this implementation can be overriden by the
 * generated parser using the code declared in the "scan with" clause. Do
 * not recycle objects; every call to scan() should return a fresh object.
 */
SmplToken *smpl_parser_context_scan_next_real(SmplParserContext *parserContext, int eofSymbol);


/**
 * This method is called to indicate that the parser should quit. This is
 * normally called by an accept action, but can be used to cancel parsing
 * early in other circumstances if desired.
 */
void smpl_parser_context_done_parsing(SmplParserContext *parser_context);
SmplToken *smpl_parser_context_peek_real(SmplParserContext *parser_context);
SmplToken *smpl_parser_context_pop_real(SmplParserContext *parser_context);
void smpl_parser_context_push_real(SmplParserContext *parser_context, SmplToken *token);
void smpl_parser_context_shift_real(SmplParserContext *parser_context, SmplToken *token);
SmplToken *smpl_parser_context_get_from_top_real(SmplParserContext *parser_context, int reverseIndex);

SmplToken *smpl_parser_context_peek_at_real(SmplParserContext *parser_context, int reverse_index);
void smpl_parser_context_reduce_real(SmplParserContext *parser_context, int replace_count, SmplToken *replace_with_symbol);





/** 
 *Return the current lookahead in our error "parse ahead" buffer. 
 */
SmplToken *smpl_parser_context_current_error_token(SmplParserContext *parserContext);



G_END_DECLS
#endif /* SMPLPARSERCONTEXT_H_ */
