#ifndef [%PRE%]PARSERBASE_H_
#define [%PRE%]PARSERBASE_H_

#include "[%pre%]parsercontext.h"
#include "[%pre%]iscanner.h"
#include "[%pre%]token.h"
#include "[%pre%]vector.h"
#include "[%pre%]2darray.h"
#include "[%pre%]virtualparsestack.h"


typedef [%Pre%]Token *(*[%Pre%]ParserActionsCB)([%Pre%]ParserContext *parser_context, int cup_action_id);

typedef struct s_[%Pre%]ParserBase       [%Pre%]ParserBase;
typedef struct s_[%Pre%]ParserBaseClass  [%Pre%]ParserBaseClass;

struct s_[%Pre%]ParserBase {
	[%Pre%]ParserBaseClass *classPtr;
	[%Pre%]2DArray *production_tab;
	[%Pre%]2DArray *action_tab;
	[%Pre%]2DArray *reduce_tab;
	int error_sync_size;

	[%Pre%]ScannerNextTokenCB scanner;
	void *scanner_data;
};


struct s_[%Pre%]ParserBaseClass {

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
	[%Pre%]ParserActionsCB action_cb;

};


[%Pre%]ParserBaseClass *[%pre%]_parser_base_get_class(void *ptr_to_obj);

void [%pre%]_parser_base_init([%Pre%]ParserBase *parser_base);

[%Pre%]ParserBase *[%pre%]_parser_base_new();


[%Pre%]Token *[%pre%]_parser_base_parse([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext);

[%Pre%]Token *[%pre%]_parser_base_scan([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext);

[%pre%]boolean [%pre%]_parser_base_error_recovery([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext, [%pre%]boolean debug);

short [%pre%]_parser_base_get_action([%Pre%]ParserBase *parser_base, int state, int sym);

short [%pre%]_parser_base_get_reduce([%Pre%]ParserBase *parser_base, int state, int sym);

[%pre%]boolean [%pre%]_parser_base_find_recovery_config([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext, [%pre%]boolean debug);


[%pre%]boolean [%pre%]_parser_base_shift_under_error([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext);

void [%pre%]_parser_base_parse_lookahead([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext, [%pre%]boolean debug);

void [%pre%]_parser_base_read_lookahead([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext);

[%pre%]boolean [%pre%]_parser_base_advance_lookahead([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext);

void [%pre%]_parser_base_restart_lookahead([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext);

[%pre%]boolean [%pre%]_parser_base_try_parse_ahead([%Pre%]ParserBase *parser_base, [%Pre%]ParserContext *parserContext, [%pre%]boolean debug);


#endif /* [%PRE%]PARSERBASE_H_ */
