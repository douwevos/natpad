/*
   File:    spiplainparser.c
   Project: spider
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

#include "spiplainparser.h"
#include "../editor/spiscannernew.h"
#include <grossruntime.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "SpiPlainParser"
#include <logging/catlog.h>

struct _SpiPlainParserPrivate {
	SpiScannerNew *token_scanner;
	CatArrayWo *token_cash;
	GroRunITokenFactory *token_factory;
	GroRunModel *model;
	DraKeywordPrinter *token_printer;
	DraKeywordPrinter *line_tag_printer;
};

static void l_scanner_iface_init(GroRunIScannerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(SpiPlainParser, spi_plain_parser, G_TYPE_OBJECT,
		G_ADD_PRIVATE(SpiPlainParser)
		G_IMPLEMENT_INTERFACE(GRORUN_TYPE_ISCANNER, l_scanner_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void spi_plain_parser_class_init(SpiPlainParserClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void spi_plain_parser_init(SpiPlainParser *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	SpiPlainParser *instance = SPI_PLAIN_PARSER(object);
	SpiPlainParserPrivate *priv = spi_plain_parser_get_instance_private(instance);
	cat_unref_ptr(priv->token_scanner);
	cat_unref_ptr(priv->token_factory);
	cat_unref_ptr(priv->token_printer);
	cat_unref_ptr(priv->line_tag_printer);
	cat_unref_ptr(priv->model);
	cat_unref_ptr(priv->token_cash);
	G_OBJECT_CLASS(spi_plain_parser_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(spi_plain_parser_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

SpiPlainParser *spi_plain_parser_new(SpiScannerNew *token_scanner, GroRunITokenFactory *token_factory, GroRunModel *model, DraKeywordPrinter *token_printer, DraKeywordPrinter *line_tag_printer) {
	SpiPlainParser *result = g_object_new(SPI_TYPE_PLAIN_PARSER, NULL);
	cat_ref_anounce(result);
	SpiPlainParserPrivate *priv = spi_plain_parser_get_instance_private(result);
	priv->token_scanner = cat_ref_ptr(token_scanner);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->model = cat_ref_ptr(model);
	priv->token_cash = cat_array_wo_new();
	priv->token_printer = cat_ref_ptr(token_printer);
	priv->line_tag_printer = cat_ref_ptr(line_tag_printer);
	return result;
}



static int l_map_symbol(SpiPlainParserPrivate *priv, GroRunIToken *token);


void spi_plain_parser_run(SpiPlainParser *parser) {
	SpiPlainParserPrivate *priv = spi_plain_parser_get_instance_private(parser);

	GroRunIScanner *scanner = GRORUN_ISCANNER(parser);

	GroRunContext *context = grorun_context_new(priv->token_factory, priv->model, NULL);
	grorun_parser_do_parse(context, scanner);


	GroRunFork *fork = grorun_context_accepted_fork_first(context);
	if (fork==NULL) {
		if (grorun_context_error_fork_count(context)>0) {
			fork = grorun_context_error_fork_at(context, 0);
		}
	}
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
			if (spi_scanner_new_is_eof(priv->token_scanner, token)) {
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


//	cat_unref_ptr(lr_parser);
//	cat_unref_ptr(parser_context);
}


static int l_map_symbol(SpiPlainParserPrivate *priv, GroRunIToken *token) {
	if (GRORUN_IS_FULL_TOKEN(token)) {
		int sym_index = grorun_full_token_get_user_index((GroRunFullToken *) token);
		switch(sym_index) {
			case SPI_SYM_ID : return 8;
			case SPI_SYM_NUMBER :
			case SPI_SYM_STRING :
			case SPI_SYM_LPAREN :
			case SPI_SYM_RPAREN :
			case SPI_SYM_LBRACE :
			case SPI_SYM_RBRACE :
			case SPI_SYM_SEMI :
			case SPI_SYM_COMMA :
			case SPI_SYM_ASS_IS :
			case SPI_SYM_QUESTION :
			case SPI_SYM_COLON :
			case SPI_SYM_OP_LOR :
			case SPI_SYM_OP_LAND :
			case SPI_SYM_OP_BOR :
			case SPI_SYM_OP_BXOR :
			case SPI_SYM_OP_BAND :
			case SPI_SYM_KW_DELETE :
			case SPI_SYM_KW_VOID :
			case SPI_SYM_KW_TYPEOF :
			case SPI_SYM_OP_INC :
			case SPI_SYM_OP_DEC :
			case SPI_SYM_OP_ADD :
			case SPI_SYM_OP_SUB :
			case SPI_SYM_OP_NEG :
			case SPI_SYM_OP_NOT :
			case SPI_SYM_OP_MUL :
			case SPI_SYM_OP_DIV :
			case SPI_SYM_OP_MOD :
			case SPI_SYM_OP_SHL :
			case SPI_SYM_OP_SHR :
			case SPI_SYM_OP_SHRR :
			case SPI_SYM_OP_LT :
			case SPI_SYM_OP_GT :
			case SPI_SYM_OP_LE :
			case SPI_SYM_OP_GE :
			case SPI_SYM_OP_EQ :
			case SPI_SYM_OP_NE :
			case SPI_SYM_OP_TEQ :
			case SPI_SYM_OP_TNE :
			case SPI_SYM_OP_ASS_MUL :
			case SPI_SYM_OP_ASS_DIV :
			case SPI_SYM_OP_ASS_MOD :
			case SPI_SYM_OP_ASS_ADD :
			case SPI_SYM_OP_ASS_SUB :
			case SPI_SYM_OP_ASS_SHL :
			case SPI_SYM_OP_ASS_SHR :
			case SPI_SYM_OP_ASS_SHRR :
			case SPI_SYM_OP_ASS_AND :
			case SPI_SYM_OP_ASS_XOR :
			case SPI_SYM_OP_ASS_OR :
			case SPI_SYM_LBRACK :
			case SPI_SYM_RBRACK :
			case SPI_SYM_DOT :
			case SPI_SYM_REGULAR_EXPRESSION : return 1;
			case SPI_SYM_KW_FUNCTION :
			case SPI_SYM_KW_VAR :
			case SPI_SYM_KW_RETURN :
			case SPI_SYM_KW_IF :
			case SPI_SYM_KW_ELSE :
			case SPI_SYM_KW_THIS :
			case SPI_SYM_KW_FOR :
			case SPI_SYM_KW_NULL :
			case SPI_SYM_KW_TRUE :
			case SPI_SYM_KW_FALSE :
			case SPI_SYM_KW_INSTANCE_OF :
			case SPI_SYM_KW_IN :
			case SPI_SYM_KW_NEW :
			case SPI_SYM_KW_SWITCH :
			case SPI_SYM_KW_CASE :
			case SPI_SYM_KW_DEFAULT :
			case SPI_SYM_KW_DO :
			case SPI_SYM_KW_WHILE :
			case SPI_SYM_KW_WITH :
			case SPI_SYM_KW_CONTINUE :
			case SPI_SYM_KW_BREAK :
			case SPI_SYM_KW_THROW :
			case SPI_SYM_KW_TRY :
			case SPI_SYM_KW_CATCH :
			case SPI_SYM_KW_FINALLY : return 9;
			case SPI_SYM_LINE_COMMENT :
			case SPI_SYM_FULL_COMMENT : return 7;
			default : {} break;
		}
	}
	return 1;
}





/********************* start GroRunIScanner implementation *********************/


static GroRunIToken *l_scanner_next(GroRunIScanner *self) {
	SpiPlainParser *plain_parser = SPI_PLAIN_PARSER(self);
	SpiPlainParserPrivate *priv = spi_plain_parser_get_instance_private(plain_parser);

	GroRunIToken *token = NULL;
	while(TRUE) {
		token = grorun_iscanner_next((GroRunIScanner *) priv->token_scanner);

		if (token!=NULL) {
			cat_log_debug("token=%O", token);
			if (spi_scanner_new_is_eof(priv->token_scanner, token)) {
				break;
			}

			cat_array_wo_append(priv->token_cash, (GObject *) token);

			int user_index = grorun_full_token_get_user_index((GroRunFullToken *) token);
			cat_log_detail("token=%O, user_index=%d", token, user_index);
			if ((user_index==SPI_SYM_LINE_COMMENT) || (user_index==SPI_SYM_FULL_COMMENT)) {
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
	SpiPlainParser *instance = SPI_PLAIN_PARSER(self);
	SpiPlainParserPrivate *priv = spi_plain_parser_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, model=%O]", iname, self, priv->model);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
