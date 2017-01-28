#include "sholrparser.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ERROR
#define CAT_LOG_CLAZZ "ShoLrParser"
#include <logging/catlog.h>


/** Production table. */
const short sho_lr_parser_production_table[] = {
	/* the number of rows */
	36, 
	/* the number of columns for each row */
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	/* the raw table data */	/* start row 0 */
	0, 2	/* start row 1 */, 8, 1	/* start row 2 */, 7, 1	/* start row 3 */, 7, 1	/* start row 4 */, 7, 2	/* start row 5 */, 7, 2	/* start row 6 */, 9, 5	/* start row 7 */, 9, 4	/* start row 8 */, 11, 1	/* start row 9 */, 11, 3	/* start row 10 */, 10, 1	/* start row 11 */, 10, 2	/* start row 12 */,
	5, 2	/* start row 13 */, 5, 1	/* start row 14 */, 16, 1	/* start row 15 */, 16, 1	/* start row 16 */, 16, 1	/* start row 17 */, 13, 3	/* start row 18 */, 14, 2	/* start row 19 */, 14, 2	/* start row 20 */, 15, 2	/* start row 21 */, 4, 2	/* start row 22 */, 4, 1	/* start row 23 */, 3, 3	/* start row 24 */,
	12, 3	/* start row 25 */, 12, 4	/* start row 26 */, 6, 2	/* start row 27 */, 6, 3	/* start row 28 */, 2, 3	/* start row 29 */, 2, 1	/* start row 30 */, 1, 1	/* start row 31 */, 1, 1	/* start row 32 */, 1, 1	/* start row 33 */, 17, 1	/* start row 34 */, 17, 1	/* start row 35 */, 17, 1
};

/** Parse-action table. */
const short sho_lr_parser_action_table[] = {
	/* the number of rows */
	56, 
	/* the number of columns for each row */
	4, 6, 8, 4, 6, 16, 8, 8, 6, 8, 6, 4, 10, 8, 4, 4, 12, 10, 8, 14, 10, 12, 8, 6,
	8, 6, 6, 8, 16, 8, 14, 4, 6, 12, 4, 8, 12, 14, 6, 12, 8, 12, 6, 16, 6, 6, 4, 6,
	4, 4, 6, 6, 6, 6, 6, 4,
	/* the raw table data */	/* start row 0 */
	9, 6, -1, 0	/* start row 1 */, 0, -3, 9, -3, -1, 0	/* start row 2 */, 0, -15, 3, -15, 9, -15, -1, 0	/* start row 3 */, 0, 56, -1, 0	/* start row 4 */, 0, -2,
	9, 6, -1, 0	/* start row 5 */, 2, 20, 5, 13, 7, 11, 9, 15, 10, 10, 11, 18, 12, 21, -1, 0	/* start row 6 */, 0, -17, 3, -17,
	9, -17, -1, 0	/* start row 7 */, 0, -16, 3, -16, 9, -16, -1, 0	/* start row 8 */, 0, -4, 9, -4, -1, 0	/* start row 9 */, 0, -21, 3, -21, 9, -21,
	-1, 0	/* start row 10 */, 8, 47, 9, 44, -1, 0	/* start row 11 */, 2, 43, -1, 0	/* start row 12 */, 3, -36, 4, -36, 6, -36, 8, -36, -1, 0	/* start row 13 */, 0, -20,
	3, -20, 9, -20, -1, 0	/* start row 14 */, 7, 31, -1, 0	/* start row 15 */, 6, 41, -1, 0	/* start row 16 */, 0, -23, 3, -23, 4, -23, 8, -23, 9, -23,
	-1, 0	/* start row 17 */, 3, -34, 4, -34, 6, -34, 8, -34, -1, 0	/* start row 18 */, 0, -19, 3, -19, 9, -19, -1, 0	/* start row 19 */, 2, 20, 3, 22,
	5, 13, 9, 29, 11, 18, 12, 21, -1, 0	/* start row 20 */, 3, -35, 4, -35, 6, -35, 8, -35, -1, 0	/* start row 21 */, 0, -27, 3, -27,
	4, -27, 8, -27, 9, -27, -1, 0	/* start row 22 */, 3, -32, 4, -32, 8, -32, -1, 0	/* start row 23 */, 3, -14, 9, -14, -1, 0	/* start row 24 */, 3, -31,
	4, -31, 8, -31, -1, 0	/* start row 25 */, 3, 40, 4, 34, -1, 0	/* start row 26 */, 3, 37, 9, 38, -1, 0	/* start row 27 */, 3, -30, 4, -30, 8, -30,
	-1, 0	/* start row 28 */, 2, 20, 5, 13, 7, 31, 9, 15, 10, 10, 11, 18, 12, 21, -1, 0	/* start row 29 */, 3, -33, 4, -33, 8, -33,
	-1, 0	/* start row 30 */, 2, 20, 5, 13, 8, 32, 9, 15, 11, 18, 12, 21, -1, 0	/* start row 31 */, 2, -25, -1, 0	/* start row 32 */, 4, 34, 8, 35,
	-1, 0	/* start row 33 */, 2, 20, 5, 13, 9, 15, 11, 18, 12, 21, -1, 0	/* start row 34 */, 2, -26, -1, 0	/* start row 35 */, 3, -29, 4, -29, 8, -29,
	-1, 0	/* start row 36 */, 0, -28, 3, -28, 4, -28, 8, -28, 9, -28, -1, 0	/* start row 37 */, 2, 20, 5, 13, 9, 15, 10, 10, 11, 18,
	12, 21, -1, 0	/* start row 38 */, 3, -13, 9, -13, -1, 0	/* start row 39 */, 0, -24, 3, -24, 4, -24, 8, -24, 9, -24, -1, 0	/* start row 40 */, 0, -18,
	3, -18, 9, -18, -1, 0	/* start row 41 */, 0, -22, 3, -22, 4, -22, 8, -22, 9, -22, -1, 0	/* start row 42 */, 3, 22, 9, 38, -1, 0	/* start row 43 */,
	2, 20, 4, -11, 5, 13, 8, -11, 9, 15, 11, 18, 12, 21, -1, 0	/* start row 44 */, 4, 49, 8, 50, -1, 0	/* start row 45 */, 4, -9,
	8, -9, -1, 0	/* start row 46 */, 6, 48, -1, 0	/* start row 47 */, 0, -8, 9, -8, -1, 0	/* start row 48 */, 9, 44, -1, 0	/* start row 49 */, 6, 51, -1, 0	/* start row 50 */, 0, -7,
	9, -7, -1, 0	/* start row 51 */, 4, -10, 8, -10, -1, 0	/* start row 52 */, 4, -12, 8, -12, -1, 0	/* start row 53 */, 0, -5, 9, -5, -1, 0	/* start row 54 */, 0, -6,
	9, -6, -1, 0	/* start row 55 */, 0, -1, -1, 0
};

/** Parse-action table. */
const short sho_lr_parser_reduce_table[] = {
	/* the number of rows */
	56, 
	/* the number of columns for each row */
	16, 2, 2, 2, 12, 12, 2, 2, 2, 2, 6, 4, 2, 2, 2, 2, 2, 2, 2, 26, 2, 2, 2, 2,
	2, 2, 10, 2, 12, 2, 16, 2, 2, 14, 2, 2, 2, 12, 2, 2, 2, 2, 12, 14, 2, 2, 2, 2,
	4, 2, 2, 2, 2, 2, 2, 2,
	/* the raw table data */	/* start row 0 */
	7, 4, 8, 3, 9, 1, 13, 6, 14, 2, 15, 7, 16, 8, -1, -1	/* start row 1 */, -1, -1	/* start row 2 */, -1, -1	/* start row 3 */, -1, -1	/* start row 4 */, 9, 53,
	13, 6, 14, 2, 15, 7, 16, 54, -1, -1	/* start row 5 */, 3, 13, 4, 18, 6, 16, 12, 11, 17, 15, -1, -1	/* start row 6 */, -1, -1	/* start row 7 */,
	-1, -1	/* start row 8 */, -1, -1	/* start row 9 */, -1, -1	/* start row 10 */, 10, 45, 11, 44, -1, -1	/* start row 11 */, 6, 41, -1, -1	/* start row 12 */, -1, -1	/* start row 13 */, -1, -1	/* start row 14 */, -1, -1	/* start row 15 */, -1, -1	/* start row 16 */,
	-1, -1	/* start row 17 */, -1, -1	/* start row 18 */, -1, -1	/* start row 19 */, 1, 27, 2, 25, 3, 22, 4, 24, 5, 26, 6, 16, 12, 11, 13, 6, 14, 2,
	15, 7, 16, 23, 17, 29, -1, -1	/* start row 20 */, -1, -1	/* start row 21 */, -1, -1	/* start row 22 */, -1, -1	/* start row 23 */, -1, -1	/* start row 24 */, -1, -1	/* start row 25 */, -1, -1	/* start row 26 */, 13, 6, 14, 2,
	15, 7, 16, 38, -1, -1	/* start row 27 */, -1, -1	/* start row 28 */, 3, 13, 4, 18, 6, 16, 12, 11, 17, 15, -1, -1	/* start row 29 */, -1, -1	/* start row 30 */, 1, 27,
	2, 32, 3, 22, 4, 24, 6, 16, 12, 11, 17, 29, -1, -1	/* start row 31 */, -1, -1	/* start row 32 */, -1, -1	/* start row 33 */, 1, 35, 3, 22, 4, 24,
	6, 16, 12, 11, 17, 29, -1, -1	/* start row 34 */, -1, -1	/* start row 35 */, -1, -1	/* start row 36 */, -1, -1	/* start row 37 */, 3, 13, 4, 18, 6, 16, 12, 11, 17, 15,
	-1, -1	/* start row 38 */, -1, -1	/* start row 39 */, -1, -1	/* start row 40 */, -1, -1	/* start row 41 */, -1, -1	/* start row 42 */, 5, 26, 13, 6, 14, 2, 15, 7, 16, 23, -1, -1	/* start row 43 */, 1, 52,
	3, 22, 4, 24, 6, 16, 12, 11, 17, 29, -1, -1	/* start row 44 */, -1, -1	/* start row 45 */, -1, -1	/* start row 46 */, -1, -1	/* start row 47 */, -1, -1	/* start row 48 */, 10, 51, -1, -1	/* start row 49 */,
	-1, -1	/* start row 50 */, -1, -1	/* start row 51 */, -1, -1	/* start row 52 */, -1, -1	/* start row 53 */, -1, -1	/* start row 54 */, -1, -1	/* start row 55 */, -1, -1
};

G_DEFINE_TYPE(ShoLrParser, sho_lr_parser, SHO_TYPE_PARSER_BASE)

static void _dispose(GObject *object);
static ShoToken *sho_lr_parser_run_action(ShoParserBase *parser_base, ShoParserContext *parserContext, int actionId);
static int sho_lr_parser_start_state(ShoParserBase *parser_base);
static int sho_lr_parser_start_production(ShoParserBase *parser_base);
static int sho_lr_parser_eof_symbol(ShoParserBase *parser_base);
static int sho_lr_parser_error_symbol(ShoParserBase *parser_base);


static void sho_lr_parser_class_init(ShoLrParserClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;

	ShoParserBaseClass *parser_base_class = SHO_PARSER_BASE_CLASS(clazz);
	parser_base_class->start_state = sho_lr_parser_start_state;
	parser_base_class->start_production = sho_lr_parser_start_production;
	parser_base_class->eof_symbol = sho_lr_parser_eof_symbol;
	parser_base_class->error_symbol = sho_lr_parser_error_symbol;
	parser_base_class->run_action = sho_lr_parser_run_action;
}

static void sho_lr_parser_init(ShoLrParser *parser) {
	ShoParserBase *parser_base = SHO_PARSER_BASE(parser);
	parser_base->production_tab = sho_2d_array_new((short *) sho_lr_parser_production_table);
	parser_base->action_tab = sho_2d_array_new((short *) sho_lr_parser_action_table);
	parser_base->reduce_tab = sho_2d_array_new((short *) sho_lr_parser_reduce_table);
	parser_base->error_sync_size = 5;
}

static void _dispose(GObject *object) {
	ShoParserBase *parser_base = SHO_PARSER_BASE(object);
	if (parser_base->production_tab) {
		parser_base->production_tab->data = NULL;
		cat_unref_ptr(parser_base->production_tab);
	}
	if (parser_base->action_tab) {
		parser_base->action_tab->data = NULL;
		cat_unref_ptr(parser_base->action_tab);
	}
	if (parser_base->reduce_tab) {
		parser_base->reduce_tab->data = NULL;
		cat_unref_ptr(parser_base->reduce_tab);
	}
	cat_unref_ptr(SHO_LR_PARSER(object)->parser_actions);
}

ShoLrParser *sho_lr_parser_new(ShoIScanner *scanner) {
	ShoLrParser *result = g_object_new(SHO_TYPE_LR_PARSER, NULL);
	cat_ref_anounce(result);
	result->parser_actions = sho_parser_actions_new();
	return result;
}


/** Invoke a user supplied parse action. */
static ShoToken *sho_lr_parser_run_action(ShoParserBase *parser_base, ShoParserContext *parserContext, int actionId) {
	/* call code in generated class */
	ShoLrParser *parser = (ShoLrParser *) parser_base;
	ShoParserActions *parser_actions = (ShoParserActions *) parser->parser_actions;
	return sho_parser_actions_run_action(parser_actions, parserContext, actionId);
}

/** Indicates start state. */
static int sho_lr_parser_start_state(ShoParserBase *parser_base) {
	return 0;
}
/** Indicates start production. */
static int sho_lr_parser_start_production(ShoParserBase *parser_base) {
	return 0;
}

/** <code>EOF</code> Symbol index. */
static int sho_lr_parser_eof_symbol(ShoParserBase *parser_base) {
	return 0;
}

/** <code>error</code> Symbol index. */
static int sho_lr_parser_error_symbol(ShoParserBase *parser_base) {
	return 1;
}
