/*
   File:    cusplainparser.c
   Project: cuscus
   Author:  Douwe Vos
   Date:    Mai 7, 2016
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2011 Douwe Vos.

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

#include "cusplainparser.h"

#include "cusparsercontextext.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CusPlainParser"
#include <logging/catlog.h>

struct _CusPlainParserPrivate {
	GroIScanner *delagate;
	DraKeywordPrinter *keyword_printer;
	DraKeywordPrinter *line_tag_printer;
	CatArrayWo *e_token_cash;
};

static void l_scanner_iface_init(GroIScannerInterface *iface);


G_DEFINE_TYPE_WITH_CODE(CusPlainParser, cus_plain_parser, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(CusPlainParser);
		G_IMPLEMENT_INTERFACE(GRO_TYPE_ISCANNER, l_scanner_iface_init);
});

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void cus_plain_parser_class_init(CusPlainParserClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void cus_plain_parser_init(CusPlainParser *instance) {
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CusPlainParser *instance = CUS_PLAIN_PARSER(object);
	CusPlainParserPrivate *priv = cus_plain_parser_get_instance_private(instance);
	cat_unref_ptr(priv->keyword_printer);
	cat_unref_ptr(priv->line_tag_printer);
	cat_unref_ptr(priv->delagate);
	cat_unref_ptr(priv->e_token_cash);
	G_OBJECT_CLASS(cus_plain_parser_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	G_OBJECT_CLASS(cus_plain_parser_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CusPlainParser *cus_plain_parser_new(GroIScanner *delagate, DraKeywordPrinter *token_printer, DraKeywordPrinter *line_tag_printer) {
	CusPlainParser *result = g_object_new(CUS_TYPE_PLAIN_PARSER, NULL);
	cat_ref_anounce(result);
	CusPlainParserPrivate *priv = cus_plain_parser_get_instance_private(result);
	priv->delagate = cat_ref_ptr(delagate);
	priv->keyword_printer = cat_ref_ptr(token_printer);
	priv->line_tag_printer = cat_ref_ptr(line_tag_printer);
	priv->e_token_cash = cat_array_wo_new_size(2048);
	return result;
}

static int l_map_symbol(GroToken *token);


void cus_plain_parser_run(CusPlainParser *parser) {
	CusPlainParserPrivate *priv = cus_plain_parser_get_instance_private(parser);

	GroIScanner *scanner = GRO_ISCANNER(parser);

	GroGlrParser *glr_parser = gro_glr_parser_new(&gro_glr_parser_tables_definitions, gro_glr_parser_actions_run_action);
	GroGlrParserContext *context = gro_glr_parser_context_new(scanner);
	gro_glr_parser_parse(glr_parser, context);
	GroForkEntry *fork_entry = gro_glr_parser_context_get_best_parse_result(context, TRUE);
	if (fork_entry) {
		GroContent *content = gro_fork_entry_content_at(fork_entry, 0);
		GroBnfSpec *spec = (GroBnfSpec *) gro_content_get_value(content);


		GroContent *message = gro_fork_entry_message_list_at(fork_entry,0);
		if (message) {
			CatArrayWo *errors_list = cat_array_wo_new();
			while(message) {
				cat_array_wo_append(errors_list, (GObject *) message);
				message = gro_content_get_linked(message);
			}
			cat_array_wo_reverse(errors_list);


			long long left_row, right_row;
			int left_column, right_column;
			CatIIterator *iter = cat_array_wo_iterator(errors_list);
			while(cat_iiterator_has_next(iter)) {
				GroContent *msg = cat_iiterator_next(iter);

				gro_content_get_left(msg, &left_row, &left_column);
				gro_content_get_right(msg, &right_row, &right_column);

				DraLineTagWo *line_tag = dra_line_tag_wo_new(left_row, DRA_TAG_TYPE_PARSER_ERROR);
				dra_line_tag_wo_set_color(line_tag, 0.2,0,0.7);
				dra_line_tag_wo_set_start_and_end_index(line_tag, left_column, right_column-1);
				dra_keyword_printer_print_line_tag(priv->line_tag_printer, line_tag);
			}
			cat_unref_ptr(iter);
		}
	}


	dra_keyword_printer_flush_line_tags(priv->line_tag_printer);
	dra_keyword_printer_reset(priv->line_tag_printer);


//	GroLrParser *lr_parser = gro_lr_parser_new(scanner);
//	GroParserContext *parser_context = gro_parser_context_new(scanner);
//	GroToken *token = gro_parser_base_parse((GroParserBase *) lr_parser, parser_context);
//	if (token!=NULL) {
//		GroBnfSpec *spec = (GroBnfSpec *) token->value;
////		GroModel *model = gro_bnf_spec_create_model(msg_handler, spec);
////		if (model!=NULL) {
//	}



	while(TRUE) {
		GroToken *token = gro_iscanner_next_token(priv->delagate);

		if (token) {
			if (token->sym==GRO_SYMBOL_TERM_EOF) {
				cat_unref_ptr(token);
				break;
			}
			cat_array_wo_append(priv->e_token_cash, (GObject *) token);
			cat_unref_ptr(token);
		} else {
			break;
		}
	}


	CatIIterator *iter = cat_array_wo_iterator(priv->e_token_cash);
	while(cat_iiterator_has_next(iter)) {
		GroToken *token = (GroToken *) cat_iiterator_next(iter);

		dra_keyword_printer_print_fg_color(priv->keyword_printer, token->left_row, token->left, token->right_row, token->right, l_map_symbol(token));

		if (token->is_error) {
			cat_log_error("error !!! row=%d, token->left=%d, token->right=%d", token->left_row, token->left, token->right);
			DraLineTagWo *line_tag = dra_line_tag_wo_new(token->left_row, DRA_TAG_TYPE_PARSER_ERROR);
			dra_line_tag_wo_set_color(line_tag, 1,0,0);
			dra_line_tag_wo_set_start_and_end_index(line_tag, token->left, token->right-1);
			dra_keyword_printer_print_line_tag(priv->line_tag_printer, line_tag);
		}
	}
	cat_unref_ptr(iter);



//	cat_unref_ptr(lr_parser);
//	cat_unref_ptr(parser_context);
}


//
// 5 licht paars
// 6 donker rose
// 7 donker groen
// 8 oranje
// 9 blue


static int l_map_symbol(GroToken *token) {
	switch(token->sym) {
		case GRO_SYMBOL_TERM_EOF : return 3;
		case GRO_SYMBOL_TERM_ERROR : return 3;
		case GRO_SYMBOL_TERM_PACKAGE : return 9;
		case GRO_SYMBOL_TERM_IMPORT : return 9;
		case GRO_SYMBOL_TERM_CODE : return 5;
		case GRO_SYMBOL_TERM_ACTION : return 6;
		case GRO_SYMBOL_TERM_PARSER : return 7;
		case GRO_SYMBOL_TERM_TERMINAL : return 9;
		case GRO_SYMBOL_TERM_NON : return 9;
		case GRO_SYMBOL_TERM_INIT : return 10;
		case GRO_SYMBOL_TERM_SCAN : return 11;
		case GRO_SYMBOL_TERM_WITH : return 1;
		case GRO_SYMBOL_TERM_START : return 2;
		case GRO_SYMBOL_TERM_SEMI : return 3;
		case GRO_SYMBOL_TERM_COMMA : return 4;
		case GRO_SYMBOL_TERM_STAR : return 5;
		case GRO_SYMBOL_TERM_DOT : return 6;
		case GRO_SYMBOL_TERM_COLON : return 7;
		case GRO_SYMBOL_TERM_COLON_COLON_EQUALS : return 8;
		case GRO_SYMBOL_TERM_BAR : return 9;
		case GRO_SYMBOL_TERM_PRECEDENCE : return 10;
		case GRO_SYMBOL_TERM_LEFT : return 11;
		case GRO_SYMBOL_TERM_RIGHT : return 12;
		case GRO_SYMBOL_TERM_NONASSOC : return 1;
		case GRO_SYMBOL_TERM_PERCENT_PREC : return 2;
		case GRO_SYMBOL_TERM_LBRACK : return 3;
		case GRO_SYMBOL_TERM_RBRACK : return 4;
		case GRO_SYMBOL_TERM_NONTERMINAL : return 5;
		case GRO_SYMBOL_TERM_ID : return 1;
		case GRO_SYMBOL_TERM_CODE_STRING : return 7;

		case GRO_SYMBOL_TERM_PLUS : return 9;
		case GRO_SYMBOL_TERM_EXCL : return 8;


		case GRO_SYMBOL_TERM_LINE_COMMENT : return 15;
		case GRO_SYMBOL_TERM_FULL_COMMENT : return 3;

		default : {} break;
	}
	return 1;
}




//static void l_print_token(CusPlainParser *parser, CusToken *token) {
//	CusPlainParserPrivate *priv = CUS_PLAIN_PARSER_GET_PRIVATE(parser);
//
//	cat_array_wo_append(priv->e_token_cash, token);
//
////
////	dra_keyword_printer_print_fg_color(priv->keyword_printer, token->left_row, token->left, token->right_row, token->right, l_map_symbol(token));
////
////	if (token->is_error) {
////		cat_log_debug("error !!! token->left=%d, token->right=%d", token->left, token->right);
////		DraLineTagWo *line_tag = dra_line_tag_wo_new(token->left_row);
////		dra_line_tag_wo_set_color(line_tag, 1,0,0);
////		dra_line_tag_wo_set_start_and_end_index(line_tag, token->left, token->right);
////		dra_keyword_printer_print_line_tag(priv->line_tag_printer, line_tag);
////	}
//
//}




static GroToken *l_scanner_next_token(GroIScanner *self) {
	CusPlainParser *parser = CUS_PLAIN_PARSER(self);
	CusPlainParserPrivate *priv = cus_plain_parser_get_instance_private(parser);


	GroToken *token = NULL;
	while(TRUE) {
		token = gro_iscanner_next_token(priv->delagate);

		if (token!=NULL) {
			cat_log_debug("token=%d", token->sym);
			if (token->sym==GRO_SYMBOL_TERM_EOF) {
				return token;
			}


			cat_array_wo_append(priv->e_token_cash, (GObject *) token);

			if (token->sym== GRO_SYMBOL_TERM_ERROR || token->sym == GRO_SYMBOL_TERM_FULL_COMMENT || token->sym == GRO_SYMBOL_TERM_LINE_COMMENT) {
				cat_unref_ptr(token);
				continue;
			}
		}
		break;
	}
	return token;
}

static void l_scanner_iface_init(GroIScannerInterface *iface) {
	iface->next_token = l_scanner_next_token;
}

