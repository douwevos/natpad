/*
   File:    mulvhdlplainparser.c
   Project: mule
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

#include <grossruntime.h>

#include <logging/catlogdefs.h>

#include "mulvhdlplainparser.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulVhdlPlainParser"
#include <logging/catlog.h>

struct _MulVhdlPlainParserPrivate {
	MulVhdlScanner *token_scanner;
	CatArrayWo *token_cash;
	GroRunITokenFactory *token_factory;
	GroRunModel *model;
	DraKeywordPrinter *token_printer;
	DraKeywordPrinter *line_tag_printer;
};

static void l_scanner_iface_init(GroRunIScannerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulVhdlPlainParser, mul_vhdl_plain_parser, G_TYPE_OBJECT,
		G_ADD_PRIVATE(MulVhdlPlainParser)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void mul_vhdl_plain_parser_class_init(MulVhdlPlainParserClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void mul_vhdl_plain_parser_init(MulVhdlPlainParser *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MulVhdlPlainParser *instance = MUL_VHDL_PLAIN_PARSER(object);
	MulVhdlPlainParserPrivate *priv = mul_vhdl_plain_parser_get_instance_private(instance);
	cat_unref_ptr(priv->token_scanner);
	cat_unref_ptr(priv->token_factory);
	cat_unref_ptr(priv->token_printer);
	cat_unref_ptr(priv->line_tag_printer);
	cat_unref_ptr(priv->model);
	cat_unref_ptr(priv->token_cash);
	G_OBJECT_CLASS(mul_vhdl_plain_parser_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_vhdl_plain_parser_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

MulVhdlPlainParser *mul_vhdl_plain_parser_new(MulVhdlScanner *token_scanner, GroRunITokenFactory *token_factory, GroRunModel *model, DraKeywordPrinter *token_printer, DraKeywordPrinter *line_tag_printer) {
	MulVhdlPlainParser *result = g_object_new(MUL_VHDL_TYPE_PLAIN_PARSER, NULL);
	cat_ref_anounce(result);
	MulVhdlPlainParserPrivate *priv = mul_vhdl_plain_parser_get_instance_private(result);
	priv->token_scanner = cat_ref_ptr(token_scanner);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->model = cat_ref_ptr(model);
	priv->token_cash = cat_array_wo_new();
	priv->token_printer = cat_ref_ptr(token_printer);
	priv->line_tag_printer = cat_ref_ptr(line_tag_printer);
	return result;
}



static int l_map_symbol(MulVhdlPlainParserPrivate *priv, GroRunIToken *token);


void mul_vhdl_plain_parser_run(MulVhdlPlainParser *parser) {
	MulVhdlPlainParserPrivate *priv = mul_vhdl_plain_parser_get_instance_private(parser);

	GroRunIScanner *scanner = GRORUN_ISCANNER(parser);

	GroRunContext *context = grorun_context_new(priv->token_factory, priv->model, NULL);
	grorun_parser_do_parse(context, scanner);


	GroRunFork *fork = grorun_context_accepted_fork_first(context);
	if (fork==NULL) {
		if (grorun_context_error_fork_count(context)>0) {
			fork = grorun_context_error_fork_at(context, 0);
		}
	}
	cat_log_error("#############################################################++++ fork=%O", fork);
	if (fork) {

		GroRunMessage *messages = grorun_fork_get_messages(fork);
		cat_log_error("messages=%O", messages);
		if (messages) {
			CatArrayWo *errors_list = cat_array_wo_new();
			while(messages) {
				cat_array_wo_append(errors_list, (GObject *) messages);
				messages = grorun_message_get_parent_message(messages);
			}
			cat_array_wo_reverse(errors_list);


			long left_row, right_row;
			int left_column, right_column;
			CatIIterator *iter = cat_array_wo_iterator(errors_list);
			while(cat_iiterator_has_next(iter)) {
				GroRunMessage *msg = cat_iiterator_next(iter);

				GroRunLocation *location = grorun_message_get_location(msg);
				if (location) {
					grorun_location_get_all(location, &left_column, &left_row, &right_column, &right_row);
				}

				DraLineTagWo *line_tag = dra_line_tag_wo_new(left_row, DRA_TAG_TYPE_PARSER_ERROR);
				dra_line_tag_wo_set_color(line_tag, 0.2,0,0.7);
				dra_line_tag_wo_set_start_and_end_index(line_tag, left_column, right_column);
				dra_keyword_printer_print_line_tag(priv->line_tag_printer, line_tag);
			}
			cat_unref_ptr(iter);
			cat_unref_ptr(errors_list);
		}
	}


	dra_keyword_printer_flush_line_tags(priv->line_tag_printer);
	dra_keyword_printer_reset(priv->line_tag_printer);

	while(TRUE) {
		GroRunIToken *token = grorun_iscanner_next((GroRunIScanner *) priv->token_scanner);

		if (token) {
			if (mul_vhdl_scanner_is_eof(priv->token_scanner, token)) {
				cat_unref_ptr(token);
				break;
			}
			cat_array_wo_append(priv->token_cash, (GObject *) token);
			cat_unref_ptr(token);
		} else {
			break;
		}
	}


	CatIIterator *iter = cat_array_wo_iterator(priv->token_cash);
	while(cat_iiterator_has_next(iter)) {
		GroRunFullToken *token = (GroRunFullToken *) cat_iiterator_next(iter);

		cat_log_debug("token=%O", token);

		GroRunLocation *location = grorun_full_token_get_location(token);

		int left_column, right_column;
		long left_row, right_row;
		grorun_location_get_all(location, &left_column, &left_row, &right_column, &right_row);

		dra_keyword_printer_print_fg_color(priv->token_printer, left_row, left_column, right_row, right_column, l_map_symbol(priv, (GroRunIToken *) token));
//
//		if (token->is_error) {
//			cat_log_error("error !!! row=%d, token->left=%d, token->right=%d", token->left_row, token->left, token->right);
//			DraLineTagWo *line_tag = dra_line_tag_wo_new(token->left_row);
//			dra_line_tag_wo_set_color(line_tag, 1,0,0);
//			dra_line_tag_wo_set_start_and_end_index(line_tag, token->left, token->right-1);
//			dra_keyword_printer_print_line_tag(priv->line_tag_printer, line_tag);
//		}
	}
	cat_unref_ptr(iter);
	cat_unref_ptr(context);

}


//
// 5 licht paars
// 6 donker rose
// 7 donker groen
// 8 oranje
// 9 blue


static int l_map_symbol(MulVhdlPlainParserPrivate *priv, GroRunIToken *token) {
	if (GRORUN_IS_FULL_TOKEN(token)) {
		int sym_index = grorun_full_token_get_user_index((GroRunFullToken *) token);
		switch(sym_index) {

			case MUL_VHDL_SYM_KWABS :
			case MUL_VHDL_SYM_KWACCESS :
			case MUL_VHDL_SYM_KWAFTER :
			case MUL_VHDL_SYM_KWALIAS :
			case MUL_VHDL_SYM_KWALL :
			case MUL_VHDL_SYM_KWAND :
			case MUL_VHDL_SYM_KWARCHITECTURE :
			case MUL_VHDL_SYM_KWARRAY :
			case MUL_VHDL_SYM_KWASSERT :
			case MUL_VHDL_SYM_KWATTRIBUTE :
			case MUL_VHDL_SYM_KWBEGIN :
			case MUL_VHDL_SYM_KWBLOCK :
			case MUL_VHDL_SYM_KWBODY :
			case MUL_VHDL_SYM_KWBUFFER :
			case MUL_VHDL_SYM_KWBUS :
			case MUL_VHDL_SYM_KWCASE :
			case MUL_VHDL_SYM_KWCOMPONENT :
			case MUL_VHDL_SYM_KWCONFIGURATION :
			case MUL_VHDL_SYM_KWCONSTANT :
			case MUL_VHDL_SYM_KWDISCONNECT :
			case MUL_VHDL_SYM_KWDOWNTO :
			case MUL_VHDL_SYM_KWELSE :
			case MUL_VHDL_SYM_KWELSIF :
			case MUL_VHDL_SYM_KWEND :
			case MUL_VHDL_SYM_KWENTITY :
			case MUL_VHDL_SYM_KWEXIT :
	//		case MUL_VHDL_SYM_KWFILE :
			case MUL_VHDL_SYM_KWFOR :
			case MUL_VHDL_SYM_KWFUNCTION :
			case MUL_VHDL_SYM_KWGENERATE :
			case MUL_VHDL_SYM_KWGENERIC :
			case MUL_VHDL_SYM_KWGUARDED :
			case MUL_VHDL_SYM_KWIF :
			case MUL_VHDL_SYM_KWIN :
			case MUL_VHDL_SYM_KWINOUT :
			case MUL_VHDL_SYM_KWIS :
			case MUL_VHDL_SYM_KWLABEL :
			case MUL_VHDL_SYM_KWLIBRARY :
			case MUL_VHDL_SYM_KWLINKAGE :
			case MUL_VHDL_SYM_KWLOOP :
			case MUL_VHDL_SYM_KWMAP :
			case MUL_VHDL_SYM_KWMOD :
			case MUL_VHDL_SYM_KWNAND :
			case MUL_VHDL_SYM_KWNEW :
			case MUL_VHDL_SYM_KWNEXT :
			case MUL_VHDL_SYM_KWNOR :
			case MUL_VHDL_SYM_KWOF :
			case MUL_VHDL_SYM_KWON :
			case MUL_VHDL_SYM_KWOPEN :
			case MUL_VHDL_SYM_KWOR :
			case MUL_VHDL_SYM_KWOTHERS :
			case MUL_VHDL_SYM_KWOUT :
			case MUL_VHDL_SYM_KWPACKAGE :
			case MUL_VHDL_SYM_KWPORT :
			case MUL_VHDL_SYM_KWPROCEDURE :
			case MUL_VHDL_SYM_KWPROCESS :
			case MUL_VHDL_SYM_KWRANGE :
			case MUL_VHDL_SYM_KWRECORD :
			case MUL_VHDL_SYM_KWREGISTER :
			case MUL_VHDL_SYM_KWREM :
			case MUL_VHDL_SYM_KWREPORT :
			case MUL_VHDL_SYM_KWRETURN :
			case MUL_VHDL_SYM_KWSELECT :
			case MUL_VHDL_SYM_KWSEVERITY :
			case MUL_VHDL_SYM_KWSIGNAL :
			case MUL_VHDL_SYM_KWSUBTYPE :
			case MUL_VHDL_SYM_KWTHEN :
			case MUL_VHDL_SYM_KWTO :
			case MUL_VHDL_SYM_KWTRANSPORT :
			case MUL_VHDL_SYM_KWTYPE :
			case MUL_VHDL_SYM_KWUNITS :
			case MUL_VHDL_SYM_KWUNTIL :
			case MUL_VHDL_SYM_KWUSE :
			case MUL_VHDL_SYM_KWVARIABLE :
			case MUL_VHDL_SYM_KWWAIT :
			case MUL_VHDL_SYM_KWWHEN :
			case MUL_VHDL_SYM_KWWHILE :
			case MUL_VHDL_SYM_KWWITH :
			case MUL_VHDL_SYM_KWXOR : return 12;
			case MUL_VHDL_SYM_NUMBERLITERAL : return 4;
			case MUL_VHDL_SYM_STRINGLITERAL : return 5;
	//

			case MUL_VHDL_SYM_EOL_COMMENT : return 3;

			default : {} break;
		}
	}
	return 1;
}





/********************* start GroRunIScanner implementation *********************/


static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	MulVhdlPlainParser *plain_parser = MUL_VHDL_PLAIN_PARSER(self);
	MulVhdlPlainParserPrivate *priv = mul_vhdl_plain_parser_get_instance_private(plain_parser);

	GroRunIToken *token = NULL;
	while(TRUE) {
		token = grorun_iscanner_next((GroRunIScanner *) priv->token_scanner);

		if (token!=NULL) {
			cat_log_debug("token=%O", token);
			if (mul_vhdl_scanner_is_eof(priv->token_scanner, token)) {
				break;
			}

			cat_array_wo_append(priv->token_cash, (GObject *) token);

			int user_index = grorun_full_token_get_user_index((GroRunFullToken *) token);
			cat_log_detail("token=%O, user_index=%d", token, user_index);
			if ((user_index==MUL_VHDL_SYM_EOL_COMMENT)) {
					cat_unref_ptr(token);
					continue;
			}
//			if (token->sym== GRO_SYMBOL_TERM_ERROR || token->sym == GRO_SYMBOL_TERM_FULL_COMMENT || token->sym == GRO_SYMBOL_TERM_LINE_COMMENT) {
//				cat_unref_ptr(token);
//				continue;
//			}
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
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/


