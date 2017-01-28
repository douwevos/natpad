#ifndef SMPLPARSERBASE_H_
#define SMPLPARSERBASE_H_

#include <caterpillar.h>
#include "smplparsercontext.h"
#include "smpltoken.h"
#include "smpl2darray.h"
#include "smplvirtualparsestack.h"

G_BEGIN_DECLS

#define SMPL_TYPE_PARSER_BASE            (smpl_parser_base_get_type())
#define SMPL_PARSER_BASE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), smpl_parser_base_get_type(), SmplParserBase))
#define SMPL_PARSER_BASE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SMPL_TYPE_PARSER_BASE, SmplParserBaseClass))
#define SMPL_IS_PARSER_BASE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SMPL_TYPE_PARSER_BASE))
#define SMPL_IS_PARSER_BASE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SMPL_TYPE_PARSER_BASE))
#define SMPL_PARSER_BASE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SMPL_TYPE_PARSER_BASE, SmplParserBaseClass))

typedef struct _SmplParserBase       SmplParserBase;
typedef struct _SmplParserBaseClass  SmplParserBaseClass;

//short *reddata;

struct _SmplParserBase {
	GObject parent;
	Smpl2DArray *production_tab;
	Smpl2DArray *action_tab;
	Smpl2DArray *reduce_tab;
	int error_sync_size;
};


struct _SmplParserBaseClass {
	GObjectClass parent_class;
//	Smpl2DArray *(*get_production_table)(SmplParserBase *parser_base);
//	Smpl2DArray *(*get_action_table)(SmplParserBase *parser_base);
//	Smpl2DArray *(*get_reduce_table)(SmplParserBase *parser_base);


	/** The index of the start state (supplied by generated subclass). */
	int (*start_state)(SmplParserBase *parser_base);

	/** The index of the start production (supplied by generated subclass). */
	int (*start_production)(SmplParserBase *parser_base);

	/**
	 * The index of the end of file terminal Symbol (supplied by generated
	 * subclass).
	 */
	int (*eof_symbol)(SmplParserBase *parser_base);

	/** The index of the special error Symbol (supplied by generated subclass). */
	int (*error_symbol)(SmplParserBase *parser_base);


	/**
	 * Perform a bit of user supplied action code (supplied by generated
	 * subclass). Actions are indexed by an internal action number assigned at
	 * parser generation time.
	 *
	 * @param parserContext  the parser context we are acting for.
	 * @param actionId       the internal index of the action to be performed.
	 */
	SmplToken *(*run_action)(SmplParserBase *parser_base, SmplParserContext *parserContext, int actionId);

};


GType smpl_parser_base_get_type(void);

SmplParserBase *smpl_parser_base_new();


SmplToken *smpl_parser_base_parse(SmplParserBase *parser_base, SmplParserContext *parserContext);


gboolean smpl_parser_base_error_recovery(SmplParserBase *parser_base, SmplParserContext *parserContext, gboolean debug);


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
short smpl_parser_base_get_action(SmplParserBase *parser_base, int state, int sym);

short smpl_parser_base_get_reduce(SmplParserBase *parser_base, int state, int sym);

gboolean smpl_parser_base_find_recovery_config(SmplParserBase *parser_base, SmplParserContext *parserContext, gboolean debug);


gboolean smpl_parser_base_shift_under_error(SmplParserBase *parser_base, SmplParserContext *parserContext);

void smpl_parser_base_parse_lookahead(SmplParserBase *parser_base, SmplParserContext *parserContext, gboolean debug);

void smpl_parser_base_read_lookahead(SmplParserBase *parser_base, SmplParserContext *parserContext);

gboolean smpl_parser_base_advance_lookahead(SmplParserBase *parser_base, SmplParserContext *parserContext);

void smpl_parser_base_restart_lookahead(SmplParserBase *parser_base, SmplParserContext *parserContext);

gboolean smpl_parser_base_try_parse_ahead(SmplParserBase *parser_base, SmplParserContext *parserContext, gboolean debug);


G_END_DECLS


#endif /* SMPLPARSERBASE_H_ */
