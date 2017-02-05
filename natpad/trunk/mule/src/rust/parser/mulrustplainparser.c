/*
   File:    mulrustplainparser.c
   Project: mule
   Author:  Douwe Vos
   Date:    Jan 9, 2017
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

#include "mulrustplainparser.h"
#include "gross/mulrustactions.h"
#include <grossruntime.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MulRustPlainParser"
#include <logging/catlog.h>

struct _MulRustPlainParserPrivate {
	MulRustScanner *token_scanner;
	CatArrayWo *token_cash;
	GroRunITokenFactory *token_factory;
	GroRunModel *model;
	DraKeywordPrinter *token_printer;
	DraKeywordPrinter *line_tag_printer;
};

static void l_scanner_iface_init(GroRunIScannerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MulRustPlainParser, mul_rust_plain_parser, G_TYPE_OBJECT,
		G_ADD_PRIVATE(MulRustPlainParser)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void mul_rust_plain_parser_class_init(MulRustPlainParserClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void mul_rust_plain_parser_init(MulRustPlainParser *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MulRustPlainParser *instance = MUL_RUST_PLAIN_PARSER(object);
	MulRustPlainParserPrivate *priv = mul_rust_plain_parser_get_instance_private(instance);
	cat_unref_ptr(priv->token_scanner);
	cat_unref_ptr(priv->token_factory);
	cat_unref_ptr(priv->token_printer);
	cat_unref_ptr(priv->line_tag_printer);
	cat_unref_ptr(priv->model);
	cat_unref_ptr(priv->token_cash);
	G_OBJECT_CLASS(mul_rust_plain_parser_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(mul_rust_plain_parser_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MulRustPlainParser *mul_rust_plain_parser_new(MulRustScanner *token_scanner, GroRunITokenFactory *token_factory, GroRunModel *model,
		DraKeywordPrinter *token_printer, DraKeywordPrinter *line_tag_printer) {
	MulRustPlainParser *result = g_object_new(MUL_TYPE_RUST_PLAIN_PARSER, NULL);
	cat_ref_anounce(result);
	MulRustPlainParserPrivate *priv = mul_rust_plain_parser_get_instance_private(result);
	priv->token_scanner = cat_ref_ptr(token_scanner);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->model = cat_ref_ptr(model);
	priv->token_cash = cat_array_wo_new();
	priv->token_printer = cat_ref_ptr(token_printer);
	priv->line_tag_printer = cat_ref_ptr(line_tag_printer);
	return result;
}

static int l_map_symbol(MulRustPlainParserPrivate *priv, GroRunIToken *token);

void mul_rust_plain_parser_run(MulRustPlainParser *parser) {
	MulRustPlainParserPrivate *priv = mul_rust_plain_parser_get_instance_private(parser);

	GroRunIScanner *scanner = GRORUN_ISCANNER(parser);

	MulRustActions *rust_actions = mulrust_actions_new();

	GroRunContext *context = grorun_context_new(priv->token_factory, priv->model, rust_actions);
	cat_unref_ptr(rust_actions);
	grorun_parser_do_parse(context, scanner);


	GroRunFork *fork = grorun_context_accepted_fork_first(context);
	if (fork==NULL) {
		if (grorun_context_error_fork_count(context)>0) {
			fork = grorun_context_error_fork_at(context, 0);
		}
	}
	cat_log_debug("#############################################################++++ fork=%O", fork);
	if (fork) {

		GroRunMessage *messages = grorun_fork_get_messages(fork);
		cat_log_debug("messages=%O", messages);
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
				CatStringWo *t = grorun_message_get_text(msg);
				dra_line_tag_wo_set_text(line_tag, t);
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
			if (mul_rust_scanner_is_eof(priv->token_scanner, token)) {
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

		int r = l_map_symbol(priv, (GroRunIToken *) token);
		if (r>=0) {
			dra_keyword_printer_print_fg_color(priv->token_printer, left_row, left_column, right_row, right_column, r);
		}
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


static int l_map_symbol(MulRustPlainParserPrivate *priv, GroRunIToken *token) {
	if (GRORUN_IS_FULL_TOKEN(token)) {
		int sym_index = grorun_full_token_get_user_index((GroRunFullToken *) token);
		switch(sym_index) {

			case MUL_RUST_SYM_END_OF_INPUT               :
			case MUL_RUST_SYM_ERROR                      :
			case MUL_RUST_SYM_SHL                        :
			case MUL_RUST_SYM_SHR                        :
			case MUL_RUST_SYM_LE                         :
			case MUL_RUST_SYM_EQEQ                       :
			case MUL_RUST_SYM_NE                         :
			case MUL_RUST_SYM_GE                         :
			case MUL_RUST_SYM_ANDAND                     :
			case MUL_RUST_SYM_OROR                       :
			case MUL_RUST_SYM_SHLEQ                      :
			case MUL_RUST_SYM_SHREQ                      :
			case MUL_RUST_SYM_MINUSEQ                    :
			case MUL_RUST_SYM_ANDEQ                      :
			case MUL_RUST_SYM_OREQ                       :
			case MUL_RUST_SYM_PLUSEQ                     :
			case MUL_RUST_SYM_STAREQ                     :
			case MUL_RUST_SYM_SLASHEQ                    :
			case MUL_RUST_SYM_CARETEQ                    :
			case MUL_RUST_SYM_PERCENTEQ                  :
			case MUL_RUST_SYM_DOTDOT                     :
			case MUL_RUST_SYM_DOTDOTDOT                  :
			case MUL_RUST_SYM_MOD_SEP                    :
			case MUL_RUST_SYM_RARROW                     :
			case MUL_RUST_SYM_LARROW                     : return 7;
			case MUL_RUST_SYM_FAT_ARROW                  :
			case MUL_RUST_SYM_LIT_BYTE                   :
			case MUL_RUST_SYM_LIT_CHAR                   :
			case MUL_RUST_SYM_LIT_INTEGER                :
			case MUL_RUST_SYM_LIT_FLOAT                  :
			case MUL_RUST_SYM_LIT_STR                    :
			case MUL_RUST_SYM_LIT_STR_RAW                :
			case MUL_RUST_SYM_LIT_BYTE_STR               :
			case MUL_RUST_SYM_LIT_BYTE_STR_RAW           :
			case MUL_RUST_SYM_UNDERSCORE                 :
			case MUL_RUST_SYM_LIFETIME                   :
			case MUL_RUST_SYM_SELF                       :
			case MUL_RUST_SYM_STATIC                     :
			case MUL_RUST_SYM_AS                         :
			case MUL_RUST_SYM_BREAK                      :
			case MUL_RUST_SYM_CRATE                      :
			case MUL_RUST_SYM_ELSE                       :
			case MUL_RUST_SYM_ENUM                       :
			case MUL_RUST_SYM_EXTERN                     :
			case MUL_RUST_SYM_FALSE                      :
			case MUL_RUST_SYM_FN                         :
			case MUL_RUST_SYM_FOR                        :
			case MUL_RUST_SYM_IF                         :
			case MUL_RUST_SYM_IMPL                       :
			case MUL_RUST_SYM_IN                         :
			case MUL_RUST_SYM_LET                        :
			case MUL_RUST_SYM_LOOP                       :
			case MUL_RUST_SYM_MATCH                      :
			case MUL_RUST_SYM_MOD                        :
			case MUL_RUST_SYM_MOVE                       :
			case MUL_RUST_SYM_MUT                        :
			case MUL_RUST_SYM_PRIV                       :
			case MUL_RUST_SYM_PUB                        :
			case MUL_RUST_SYM_REF                        :
			case MUL_RUST_SYM_RETURN                     :
			case MUL_RUST_SYM_STRUCT                     :
			case MUL_RUST_SYM_TRUE                       :
			case MUL_RUST_SYM_TRAIT                      :
			case MUL_RUST_SYM_TYPE                       :
			case MUL_RUST_SYM_UNSAFE                     :
			case MUL_RUST_SYM_USE                        :
			case MUL_RUST_SYM_WHILE                      :
			case MUL_RUST_SYM_CONTINUE                   :
			case MUL_RUST_SYM_PROC                       :
			case MUL_RUST_SYM_BOX                        :
			case MUL_RUST_SYM_CONST                      :
			case MUL_RUST_SYM_WHERE                      :
			case MUL_RUST_SYM_TYPEOF                     :
			case MUL_RUST_SYM_INNER_DOC_COMMENT          :
			case MUL_RUST_SYM_OUTER_DOC_COMMENT          :
			case MUL_RUST_SYM_SHEBANG                    :
			case MUL_RUST_SYM_SHEBANG_LINE               :
			case MUL_RUST_SYM_STATIC_LIFETIME            : return 2;

			case MUL_RUST_SYM_L_LSQBRACE                 :
			case MUL_RUST_SYM_L_RSQBRACE                 :
			case MUL_RUST_SYM_L_LPAREN                   :
			case MUL_RUST_SYM_L_RPAREN                   :
			case MUL_RUST_SYM_L_LCUBRACE                 :
			case MUL_RUST_SYM_L_RCUBRACE                 : return 9;

			case MUL_RUST_SYM_L_LT                       :
			case MUL_RUST_SYM_L_GT                       :
			case MUL_RUST_SYM_L_COMMA                    :
			case MUL_RUST_SYM_L_DOT                      :
			case MUL_RUST_SYM_L_SEMI                     :
			case MUL_RUST_SYM_L_AMPERSAND                :
			case MUL_RUST_SYM_L_COLON                    :
			case MUL_RUST_SYM_L_HASH                     :
			case MUL_RUST_SYM_L_AT                       :
			case MUL_RUST_SYM_L_NEG                      :
			case MUL_RUST_SYM_L_QUESTIONMARK             :
			case MUL_RUST_SYM_L_EXCLAMATION              :
			case MUL_RUST_SYM_L_VERTICAL_LINE            :
			case MUL_RUST_SYM_L_CIRCUMFLEX               :
			case MUL_RUST_SYM_L_PLUS                     :
			case MUL_RUST_SYM_L_MINUS                    :
			case MUL_RUST_SYM_L_MUL                      :
			case MUL_RUST_SYM_L_DIV                      :
			case MUL_RUST_SYM_L_IS                       :
			case MUL_RUST_SYM_L_MOD                      : return 12;

			case MUL_RUST_SYM_IDENT                      : return 1;

			default : {} break;
		}
	}
	return 1;
}


/********************* start GroRunIScanner implementation *********************/


static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	MulRustPlainParser *plain_parser = MUL_RUST_PLAIN_PARSER(self);
	MulRustPlainParserPrivate *priv = mul_rust_plain_parser_get_instance_private(plain_parser);

	GroRunIToken *token = NULL;
	while(TRUE) {
		token = grorun_iscanner_next((GroRunIScanner *) priv->token_scanner);

		if (token!=NULL) {
			cat_log_debug("token=%O", token);
			if (mul_rust_scanner_is_eof(priv->token_scanner, token)) {
				break;
			}

			cat_array_wo_append(priv->token_cash, (GObject *) token);

			int user_index = grorun_full_token_get_user_index((GroRunFullToken *) token);
			cat_log_detail("token=%O, user_index=%d", token, user_index);
			if ((user_index==MUL_RUST_SYM_BLOCK_COMMENT)) {
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
	MulRustPlainParser *instance = MUL_RUST_PLAIN_PARSER(self);
	MulRustPlainParserPrivate *priv = mul_rust_plain_parser_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
