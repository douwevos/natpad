/*
   File:    armpparser.c
   Project: armadillo
   Author:  Douwe Vos
   Date:    Mar 10, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include "armpparser.h"
#include "../../src-generated/armpparserconfig.h"
//#include "../src-generated/ast/armpastactions.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ArmPParser"
#include <logging/catlog.h>

struct _ArmPParserPrivate {
	ArmPScanner *scanner;
	GroRunITokenFactory *token_factory;
	GroRunModel *model;
	CatArrayWo *token_cash;
	CatArrayWo *messages;
};

static void l_scanner_iface_init(GroRunIScannerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ArmPParser, armp_parser, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ArmPParser)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void armp_parser_class_init(ArmPParserClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void armp_parser_init(ArmPParser *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ArmPParser *instance = ARMP_PARSER(object);
//	ArmPParserPrivate *priv = armp_parser_get_instance_private(instance);
	G_OBJECT_CLASS(armp_parser_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(armp_parser_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static gint l_message_compare(gconstpointer a, gconstpointer b) {
	GroRunLocation *loc_a = grorun_message_get_location((GroRunMessage *) a);
	GroRunLocation *loc_b = grorun_message_get_location((GroRunMessage *) b);
	int col_a, col_b;
	long row_a, row_b;
	grorun_location_get_begin(loc_a, &col_a, &row_a);
	grorun_location_get_begin(loc_b, &col_b, &row_b);

	if (row_a<row_b) {
		return -1;
	} else if (row_a>row_b) {
		return 1;
	} else if (col_a<col_b) {
		return -1;
	} else if (col_a>col_b) {
		return 1;
	}
	return 0;
}


ArmPParser *armp_parser_new(ArmPScanner *scanner, GroRunITokenFactory *token_factory, GroRunModel *model) {
	ArmPParser *result = g_object_new(ARMP_TYPE_PARSER, NULL);
	cat_ref_anounce(result);
	ArmPParserPrivate *priv = armp_parser_get_instance_private(result);
	priv->scanner = cat_ref_ptr(scanner);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->model = cat_ref_ptr(model);
	priv->token_cash = cat_array_wo_new();
	priv->messages = cat_array_wo_new();
	return result;
}


/*GroAstSpec * */void armp_parser_run(ArmPParser *parser) {
	ArmPParserPrivate *priv = armp_parser_get_instance_private(parser);

//	GroAstSpec *result = NULL;

	GroRunIScanner *scanner = GRORUN_ISCANNER(parser);

//	ArmPAstActions *actions = armp_ast_actions_new();

	GroRunContext *context = grorun_context_new(priv->token_factory, priv->model, NULL);
	grorun_parser_do_parse(context, scanner);

	GroRunFork *fork = grorun_context_accepted_fork_first(context);
	if (fork==NULL) {
		if (grorun_context_error_fork_count(context)>0) {
			fork = grorun_context_error_fork_at(context, 0);
		}
	}
	cat_log_debug("#############################################################++++ fork=%O", fork);
	if (fork) {


		GroRunStackEntry *entry = grorun_fork_get_entry(fork, 1);
		if (entry) {
			GroRunIToken *main_lah = grorun_stack_entry_get_lookahead(entry);
			cat_log_debug("*********** %O", main_lah);

			GObject *val = grorun_itoken_get_value(main_lah);
			cat_log_debug("*********** %O", val);
//			if (GROAST_IS_SPEC(val)) {
//				result = (GroAstSpec *) val;
//				l_analyze_spec(parser, result);
//				priv->ast_specification = result;
//				cat_ref_ptr(result);
//
//			}

		}

		GroRunMessage *messages = grorun_fork_get_messages(fork);
		cat_log_debug("messages=%O", messages);
		if (messages) {
			while(messages) {
				cat_array_wo_append(priv->messages, (GObject *) messages);
				messages = grorun_message_get_parent_message(messages);
			}
		}


		if (cat_array_wo_size(priv->messages)>0) {
			cat_array_wo_sort(priv->messages, (GCompareFunc) l_message_compare);
		}
	}


//	while(TRUE) {
//		GroRunIToken *token = grorun_iscanner_next((GroRunIScanner *) priv->scanner);
//
//		if (token) {
//			if (gro_scanner_is_eof(priv->scanner, token)) {
//				cat_unref_ptr(token);
//				break;
//			}
//			cat_array_wo_append(priv->token_cash, (GObject *) token);
//			cat_unref_ptr(token);
//		} else {
//			break;
//		}
//	}
//
//	cat_unref_ptr(context);
//
//	return result;
}



CatArrayWo *armp_parser_get_token_cash(ArmPParser *parser) {
	ArmPParserPrivate *priv = armp_parser_get_instance_private(parser);
	return priv->token_cash;
}

CatArrayWo *armp_parser_get_messages(ArmPParser *parser) {
	ArmPParserPrivate *priv = armp_parser_get_instance_private(parser);
	return priv->messages;
}



/********************* start GroRunIScanner implementation *********************/


static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	ArmPParser *parser = ARMP_PARSER(self);
	ArmPParserPrivate *priv = armp_parser_get_instance_private(parser);

	GroRunIToken *token = NULL;
	while(TRUE) {
		token = grorun_iscanner_next((GroRunIScanner *) priv->scanner);

		if (token!=NULL) {
			cat_log_debug("token=%O", token);
			if (armp_scanner_is_eof(priv->scanner, token)) {
				break;
			}

			cat_array_wo_append(priv->token_cash, (GObject *) token);

			int sym_index = grorun_full_token_get_user_index((GroRunFullToken *) token);
			if ((sym_index==ARMP_SYM_EOL_COMMENT) || (sym_index==ARMP_SYM_FULL_COMMENT)) {
				cat_unref_ptr(token);
				continue;
			}
		}
		break;
	}
	return token;
}


static void l_scanner_iface_init(GroRunIScannerInterface *iface) {
	iface->next = l_scanner_next;
}

/********************* end GroRunIScanner implementation *********************/



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ArmPParser *instance = ARMP_PARSER(self);
	ArmPParserPrivate *priv = armp_parser_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
