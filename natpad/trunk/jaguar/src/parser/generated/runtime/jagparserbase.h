#ifndef JAGPARSERBASE_H_
#define JAGPARSERBASE_H_

#include <caterpillar.h>
#include "jagparsercontext.h"
#include "jagtoken.h"
#include "jag2darray.h"
#include "jagvirtualparsestack.h"

G_BEGIN_DECLS

#define JAG_TYPE_PARSER_BASE            (jag_parser_base_get_type())
#define JAG_PARSER_BASE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_parser_base_get_type(), JagParserBase))
#define JAG_PARSER_BASE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_PARSER_BASE, JagParserBaseClass))
#define JAG_IS_PARSER_BASE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_PARSER_BASE))
#define JAG_IS_PARSER_BASE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_PARSER_BASE))
#define JAG_PARSER_BASE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_PARSER_BASE, JagParserBaseClass))

typedef struct _JagParserBase       JagParserBase;
typedef struct _JagParserBaseClass  JagParserBaseClass;

//short *reddata;

struct _JagParserBase {
	GObject parent;
	Jag2DArray *production_tab;
	Jag2DArray *action_tab;
	Jag2DArray *reduce_tab;
	int error_sync_size;
};


struct _JagParserBaseClass {
	GObjectClass parent_class;
//	Jag2DArray *(*get_production_table)(JagParserBase *parser_base);
//	Jag2DArray *(*get_action_table)(JagParserBase *parser_base);
//	Jag2DArray *(*get_reduce_table)(JagParserBase *parser_base);


	/** The index of the start state (supplied by generated subclass). */
	int (*start_state)(JagParserBase *parser_base);

	/** The index of the start production (supplied by generated subclass). */
	int (*start_production)(JagParserBase *parser_base);

	/**
	 * The index of the end of file terminal Symbol (supplied by generated
	 * subclass).
	 */
	int (*eof_symbol)(JagParserBase *parser_base);

	/** The index of the special error Symbol (supplied by generated subclass). */
	int (*error_symbol)(JagParserBase *parser_base);


	/**
	 * Perform a bit of user supplied action code (supplied by generated
	 * subclass). Actions are indexed by an internal action number assigned at
	 * parser generation time.
	 *
	 * @param parserContext  the parser context we are acting for.
	 * @param actionId       the internal index of the action to be performed.
	 */
	JagToken *(*run_action)(JagParserBase *parser_base, JagParserContext *parserContext, int actionId);

};


GType jag_parser_base_get_type(void);

JagParserBase *jag_parser_base_new();


JagToken *jag_parser_base_parse(JagParserBase *parser_base, JagParserContext *parserContext);


gboolean jag_parser_base_error_recovery(JagParserBase *parser_base, JagParserContext *parserContext, gboolean debug);


/**
 * Fetch an action from the action table. The table is broken up into rows,
 * one per state (rows are indexed directly by state number). Within each
 * row, a list of index, value pairs are given (as sequential entries in the
 * table), and the list is terminated by a default entry (denoted with a
 * Symbol index of -1). To find the proper entry in a row we do a linear or
 * binary search (depending on the size of the row).
 *
 * @param state   the state index of the action being accessed.
 * @param sym     the Symbol index of the action being accessed.
 */
short jag_parser_base_get_action(JagParserBase *parser_base, int state, int sym);

short jag_parser_base_get_reduce(JagParserBase *parser_base, int state, int sym);

gboolean jag_parser_base_find_recovery_config(JagParserBase *parser_base, JagParserContext *parserContext, gboolean debug);


gboolean jag_parser_base_shift_under_error(JagParserBase *parser_base, JagParserContext *parserContext);

void jag_parser_base_parse_lookahead(JagParserBase *parser_base, JagParserContext *parserContext, gboolean debug);

void jag_parser_base_read_lookahead(JagParserBase *parser_base, JagParserContext *parserContext);

gboolean jag_parser_base_advance_lookahead(JagParserBase *parser_base, JagParserContext *parserContext);

void jag_parser_base_restart_lookahead(JagParserBase *parser_base, JagParserContext *parserContext);

gboolean jag_parser_base_try_parse_ahead(JagParserBase *parser_base, JagParserContext *parserContext, gboolean debug);


G_END_DECLS


#endif /* JAGPARSERBASE_H_ */
