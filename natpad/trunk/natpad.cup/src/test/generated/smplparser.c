#include "smplparser.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ERROR
#define CAT_LOG_CLAZZ "SmplParser"
#include <logging/catlog.h>


/** Production table. */
const short smpl_parser_production_table[] = {
	/* the number of rows */
	16, 
	/* the number of columns for each row */
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	/* the raw table data */
	0, 2, 1, 1, 2, 2, 2, 1, 3, 1, 4, 4, 5, 3, 5, 3, 5, 1, 6, 3, 6, 3, 6, 1,
	8, 1, 8, 3, 9, 3, 9, 1
};

/** Parse-action table. */
const short smpl_parser_action_table[] = {
	/* the number of rows */
	29, 
	/* the number of columns for each row */
	4, 6, 6, 4, 6, 6, 6, 4, 6, 6, 10, 6, 10, 8, 10, 6, 6, 6, 10, 10, 6, 6, 10, 10,
	8, 10, 6, 6, 4,
	/* the raw table data */
	3, 6, -1, 0, 0, -4, 3, -4, -1, 0, 0, -5, 3, -5, -1, 0, 0, 29, -1, 0, 0, -2, 3, 6,
	-1, 0, 2, -16, 10, -16, -1, 0, 2, 8, 10, 9, -1, 0, 3, 27, -1, 0, 3, 15, 11, 10, -1, 0,
	3, 15, 11, 10, -1, 0, 6, -12, 7, -12, 12, -12, 13, -12, -1, 0, 4, 22, 5, 21, -1, 0, 6, -9,
	7, -9, 12, -9, 13, -9, -1, 0, 6, 18, 7, 17, 13, 16, -1, 0, 6, -13, 7, -13, 12, -13, 13, -13,
	-1, 0, 0, -6, 3, -6, -1, 0, 3, 15, 11, 10, -1, 0, 3, 15, 11, 10, -1, 0, 6, -7, 7, -7,
	12, -7, 13, -7, -1, 0, 6, -8, 7, -8, 12, -8, 13, -8, -1, 0, 3, 15, 11, 10, -1, 0, 3, 15,
	11, 10, -1, 0, 6, -10, 7, -10, 12, -10, 13, -10, -1, 0, 6, -11, 7, -11, 12, -11, 13, -11, -1, 0,
	6, 18, 7, 17, 12, 26, -1, 0, 6, -14, 7, -14, 12, -14, 13, -14, -1, 0, 2, -15, 10, -15, -1, 0,
	0, -3, 3, -3, -1, 0, 0, -1, -1, 0
};

/** Parse-action table. */
const short smpl_parser_reduce_table[] = {
	/* the number of rows */
	29, 
	/* the number of columns for each row */
	12, 2, 2, 2, 8, 2, 2, 2, 10, 10, 2, 2, 2, 2, 2, 2, 8, 8, 2, 2, 4, 4, 2, 2,
	2, 2, 2, 2, 2,
	/* the raw table data */
	1, 3, 2, 4, 3, 1, 4, 2, 9, 6, -1, -1, -1, -1, -1, -1, -1, -1, 3, 27, 4, 2, 9, 6,
	-1, -1, -1, -1, -1, -1, -1, -1, 5, 13, 6, 12, 7, 11, 8, 10, -1, -1, 5, 24, 6, 12, 7, 11,
	8, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 6, 19, 7, 11, 8, 10, -1, -1,
	6, 18, 7, 11, 8, 10, -1, -1, -1, -1, -1, -1, 8, 23, -1, -1, 8, 22, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1
};

G_DEFINE_TYPE(SmplParser, smpl_parser, SMPL_TYPE_PARSER_BASE)

static void _dispose(GObject *object);
static SmplToken *smpl_parser_run_action(SmplParserBase *parser_base, SmplParserContext *parserContext, int actionId);
static int smpl_parser_start_state(SmplParserBase *parser_base);
static int smpl_parser_start_production(SmplParserBase *parser_base);
static int smpl_parser_eof_symbol(SmplParserBase *parser_base);
static int smpl_parser_error_symbol(SmplParserBase *parser_base);


static void smpl_parser_class_init(SmplParserClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;

	SmplParserBaseClass *parser_base_class = SMPL_PARSER_BASE_CLASS(clazz);
	parser_base_class->start_state = smpl_parser_start_state;
	parser_base_class->start_production = smpl_parser_start_production;
	parser_base_class->eof_symbol = smpl_parser_eof_symbol;
	parser_base_class->error_symbol = smpl_parser_error_symbol;
	parser_base_class->run_action = smpl_parser_run_action;
}

static void smpl_parser_init(SmplParser *parser) {
	SmplParserBase *parser_base = SMPL_PARSER_BASE(parser);
	parser_base->production_tab = smpl_2d_array_new((short *) smpl_parser_production_table);
	parser_base->action_tab = smpl_2d_array_new((short *) smpl_parser_action_table);
	parser_base->reduce_tab = smpl_2d_array_new((short *) smpl_parser_reduce_table);
	parser_base->error_sync_size = 5;
}

static void _dispose(GObject *object) {
	SmplParserBase *parser_base = SMPL_PARSER_BASE(object);
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
	cat_unref_ptr(SMPL_PARSER(object)->parser_actions);
}

SmplParser *smpl_parser_new(SmplIScanner *scanner) {
	SmplParser *result = g_object_new(SMPL_TYPE_PARSER, NULL);
	cat_ref_anounce(result);
	result->parser_actions = smpl_parser_actions_new();
	return result;
}


/** Invoke a user supplied parse action. */
static SmplToken *smpl_parser_run_action(SmplParserBase *parser_base, SmplParserContext *parserContext, int actionId) {
	/* call code in generated class */
	SmplParser *parser = (SmplParser *) parser_base;
	SmplParserActions *parser_actions = (SmplParserActions *) parser->parser_actions;
	return smpl_parser_actions_run_action(parser_actions, parserContext, actionId);
}

/** Indicates start state. */
static int smpl_parser_start_state(SmplParserBase *parser_base) {
	return 0;
}
/** Indicates start production. */
static int smpl_parser_start_production(SmplParserBase *parser_base) {
	return 0;
}

/** <code>EOF</code> Symbol index. */
static int smpl_parser_eof_symbol(SmplParserBase *parser_base) {
	return 0;
}

/** <code>error</code> Symbol index. */
static int smpl_parser_error_symbol(SmplParserBase *parser_base) {
	return 1;
}
