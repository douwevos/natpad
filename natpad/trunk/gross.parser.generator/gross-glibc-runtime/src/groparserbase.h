#ifndef GROPARSERBASE_H_
#define GROPARSERBASE_H_

#include "groglibctypes.h"
#include "groparsercontext.h"
#include "grotoken.h"
#include "gro2darray.h"
#include "grovirtualparsestack.h"

G_BEGIN_DECLS

struct _GroTableDefs {
	int *production_table;
	int *action_table;
	int *reduce_table;
	int start_state;
	int start_production;
	int eof_symbol;
	int error_symbol;
};

typedef struct _GroTableDefs GroTableDefs;

struct _GroParserBase;
typedef GroToken *(*GroRunActionCB)(GroParserContext *parserContext, int actionId);


#define GRO_TYPE_PARSER_BASE            (gro_parser_base_get_type())
#define GRO_PARSER_BASE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_parser_base_get_type(), GroParserBase))
#define GRO_PARSER_BASE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_PARSER_BASE, GroParserBaseClass))
#define GRO_IS_PARSER_BASE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_PARSER_BASE))
#define GRO_IS_PARSER_BASE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_PARSER_BASE))
#define GRO_PARSER_BASE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_PARSER_BASE, GroParserBaseClass))

typedef struct _GroParserBase              GroParserBase;
typedef struct _GroParserBasePrivate       GroParserBasePrivate;
typedef struct _GroParserBaseClass         GroParserBaseClass;

struct _GroParserBase {
	GObject parent;
};


struct _GroParserBaseClass {
	GObjectClass parent_class;

};


GType gro_parser_base_get_type(void);

GroParserBase *gro_parser_base_new(GroTableDefs *table_defs, GroRunActionCB run_action_cb);


GroToken *gro_parser_base_parse(GroParserBase *parser_base, GroParserContext *parserContext);


gboolean gro_parser_base_error_recovery(GroParserBase *parser_base, GroParserContext *parserContext, gboolean debug);


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
short gro_parser_base_get_action(GroParserBase *parser_base, int state, int sym);

short gro_parser_base_get_reduce(GroParserBase *parser_base, int state, int sym);

gboolean gro_parser_base_find_recovery_config(GroParserBase *parser_base, GroParserContext *parserContext, gboolean debug);


gboolean gro_parser_base_shift_under_error(GroParserBase *parser_base, GroParserContext *parserContext);

void gro_parser_base_parse_lookahead(GroParserBase *parser_base, GroParserContext *parserContext, gboolean debug);

void gro_parser_base_read_lookahead(GroParserBase *parser_base, GroParserContext *parserContext);

gboolean gro_parser_base_advance_lookahead(GroParserBase *parser_base, GroParserContext *parserContext);

void gro_parser_base_restart_lookahead(GroParserBase *parser_base, GroParserContext *parserContext);

gboolean gro_parser_base_try_parse_ahead(GroParserBase *parser_base, GroParserContext *parserContext, gboolean debug);


G_END_DECLS


#endif /* GROPARSERBASE_H_ */
