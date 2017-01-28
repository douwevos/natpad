#ifndef [%PRE%]PARSERBASE_H_
#define [%PRE%]PARSERBASE_H_

#include <caterpillar.h>
#include "[%pre%]parsercontext.h"
#include "[%pre%]token.h"
#include "[%pre%]2darray.h"
#include "[%pre%]virtualparsestack.h"

G_BEGIN_DECLS

#define [%PRE%]_TYPE_PARSER_BASE            ([%pre%]_parser_base_get_type())
#define [%PRE%]_PARSER_BASE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), [%pre%]_parser_base_get_type(), [%Pre%]ParserBase))
#define [%PRE%]_PARSER_BASE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), [%PRE%]_TYPE_PARSER_BASE, [%Pre%]ParserBaseClass))
#define [%PRE%]_IS_PARSER_BASE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), [%PRE%]_TYPE_PARSER_BASE))
#define [%PRE%]_IS_PARSER_BASE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), [%PRE%]_TYPE_PARSER_BASE))
#define [%PRE%]_PARSER_BASE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), [%PRE%]_TYPE_PARSER_BASE, [%Pre%]ParserBaseClass))

typedef struct _[%Pre%]ParserBase       [%Pre%]ParserBase;
typedef struct _[%Pre%]ParserBaseClass  [%Pre%]ParserBaseClass;

//short *reddata;

struct _[%Pre%]ParserBase {
	GObject parent;
	[%Pre%]2DArray *production_tab;
	[%Pre%]2DArray *action_tab;
	[%Pre%]2DArray *reduce_tab;
	int error_sync_size;
};


struct _[%Pre%]ParserBaseClass {
	GObjectClass parent_class;
//	[%Pre%]2DArray *(*get_production_table)([%Pre%]ParserBase *parser_base);
//	[%Pre%]2DArray *(*get_action_table)([%Pre%]ParserBase *parser_base);
//	[%Pre%]2DArray *(*get_reduce_table)([%Pre%]ParserBase *parser_base);


	/** The index of the start state (supplied by generated subclass). */
	int (*start_state)([%Pre%]ParserBase *parser_base);

	/** The index of the start production (supplied by generated subclass). */
	int (*start_production)([%Pre%]ParserBase *parser_base);

	/**
	 * The index of the end of file terminal Symbol (supplied by generated
	 * subclass).
	 */
	int (*eof_symbol)([%Pre%]ParserBase *parser_base);

	/** The index of the special error Symbol (supplied by generated subclass). */
	int (*error_symbol)([%Pre%]ParserBase *parser_base);


	/**
	 * Perform a bit of user supplied action code (supplied by generated
	 * subclass). Actions are indexed by an internal action number assigned at
	 * parser generation time.
	 *
	 * @param parserContext  the parser context we are acting for.
	 * @param actionId       the internal index of the action to be performed.
	 */
	[%Pre%]Token *(*run_action)([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext, int actionId);

};


GType [%pre%]_parser_base_get_type(void);

[%Pre%]ParserBase *[%pre%]_parser_base_new();


[%Pre%]Token *[%pre%]_parser_base_parse([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext);


gboolean [%pre%]_parser_base_error_recovery([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext, gboolean debug);


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
short [%pre%]_parser_base_get_action([%Pre%]ParserBase *parser_base, int state, int sym);

short [%pre%]_parser_base_get_reduce([%Pre%]ParserBase *parser_base, int state, int sym);

gboolean [%pre%]_parser_base_find_recovery_config([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext, gboolean debug);


gboolean [%pre%]_parser_base_shift_under_error([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext);

void [%pre%]_parser_base_parse_lookahead([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext, gboolean debug);

void [%pre%]_parser_base_read_lookahead([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext);

gboolean [%pre%]_parser_base_advance_lookahead([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext);

void [%pre%]_parser_base_restart_lookahead([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext);

gboolean [%pre%]_parser_base_try_parse_ahead([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext, gboolean debug);


G_END_DECLS


#endif /* [%PRE%]PARSERBASE_H_ */
