#ifndef SHOPARSERBASE_H_
#define SHOPARSERBASE_H_

#include <caterpillar.h>
#include "shoparsercontext.h"
#include "shotoken.h"
#include "sho2darray.h"
#include "shovirtualparsestack.h"

G_BEGIN_DECLS

#define SHO_TYPE_PARSER_BASE            (sho_parser_base_get_type())
#define SHO_PARSER_BASE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_parser_base_get_type(), ShoParserBase))
#define SHO_PARSER_BASE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_PARSER_BASE, ShoParserBaseClass))
#define SHO_IS_PARSER_BASE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_PARSER_BASE))
#define SHO_IS_PARSER_BASE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_PARSER_BASE))
#define SHO_PARSER_BASE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_PARSER_BASE, ShoParserBaseClass))

typedef struct _ShoParserBase       ShoParserBase;
typedef struct _ShoParserBaseClass  ShoParserBaseClass;

//short *reddata;

struct _ShoParserBase {
	GObject parent;
	Sho2DArray *production_tab;
	Sho2DArray *action_tab;
	Sho2DArray *reduce_tab;
	int error_sync_size;
};


struct _ShoParserBaseClass {
	GObjectClass parent_class;
//	Sho2DArray *(*get_production_table)(ShoParserBase *parser_base);
//	Sho2DArray *(*get_action_table)(ShoParserBase *parser_base);
//	Sho2DArray *(*get_reduce_table)(ShoParserBase *parser_base);


	/** The index of the start state (supplied by generated subclass). */
	int (*start_state)(ShoParserBase *parser_base);

	/** The index of the start production (supplied by generated subclass). */
	int (*start_production)(ShoParserBase *parser_base);

	/**
	 * The index of the end of file terminal Symbol (supplied by generated
	 * subclass).
	 */
	int (*eof_symbol)(ShoParserBase *parser_base);

	/** The index of the special error Symbol (supplied by generated subclass). */
	int (*error_symbol)(ShoParserBase *parser_base);


	/**
	 * Perform a bit of user supplied action code (supplied by generated
	 * subclass). Actions are indexed by an internal action number assigned at
	 * parser generation time.
	 *
	 * @param parserContext  the parser context we are acting for.
	 * @param actionId       the internal index of the action to be performed.
	 */
	ShoToken *(*run_action)(ShoParserBase *parser_base, ShoParserContext *parserContext, int actionId);

};


GType sho_parser_base_get_type(void);

ShoParserBase *sho_parser_base_new();


ShoToken *sho_parser_base_parse(ShoParserBase *parser_base, ShoParserContext *parserContext);


gboolean sho_parser_base_error_recovery(ShoParserBase *parser_base, ShoParserContext *parserContext, gboolean debug);


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
short sho_parser_base_get_action(ShoParserBase *parser_base, int state, int sym);

short sho_parser_base_get_reduce(ShoParserBase *parser_base, int state, int sym);

gboolean sho_parser_base_find_recovery_config(ShoParserBase *parser_base, ShoParserContext *parserContext, gboolean debug);


gboolean sho_parser_base_shift_under_error(ShoParserBase *parser_base, ShoParserContext *parserContext);

void sho_parser_base_parse_lookahead(ShoParserBase *parser_base, ShoParserContext *parserContext, gboolean debug);

void sho_parser_base_read_lookahead(ShoParserBase *parser_base, ShoParserContext *parserContext);

gboolean sho_parser_base_advance_lookahead(ShoParserBase *parser_base, ShoParserContext *parserContext);

void sho_parser_base_restart_lookahead(ShoParserBase *parser_base, ShoParserContext *parserContext);

gboolean sho_parser_base_try_parse_ahead(ShoParserBase *parser_base, ShoParserContext *parserContext, gboolean debug);


G_END_DECLS


#endif /* SHOPARSERBASE_H_ */
