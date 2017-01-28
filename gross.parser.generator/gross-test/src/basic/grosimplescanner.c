
/*
   File:    grosimplescanner.c
   Project: gross-test
   Author:  Douwe Vos
   Date:    Jun 27, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "grosimplescanner.h"
#include "generated/grotbasymbol.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroSimpleScanner"
#include <logging/catlog.h>

struct _GroSimpleScannerPrivate {
	GroToken *buffered_token;
	CatIUtf8Scanner *utf8Scanner;
	CatStreamStatus streamStatus;
	gunichar lastChar;
	CatUnicharArray *buffer;
	int row, column, nextColumn, mark, mark_row;
	gboolean inFullComment;
	gboolean filter_non_parsable;

	gboolean create_ast_tokens;
};

static void i_scanner_iface_init(GroIScannerInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroSimpleScanner, gro_simple_scanner, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroSimpleScanner)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(GRO_TYPE_ISCANNER, i_scanner_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_simple_scanner_class_init(GroSimpleScannerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_simple_scanner_init(GroSimpleScanner *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	GroSimpleScanner *instance = GRO_SIMPLE_SCANNER(object);
//	GroSimpleScannerPrivate *priv = gro_simple_scanner_get_instance_private(instance);
	G_OBJECT_CLASS(gro_simple_scanner_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(gro_simple_scanner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_next_char(GroSimpleScanner *scanner);

GroSimpleScanner *gro_simple_scanner_new(CatIUtf8Scanner *scanner) {
	GroSimpleScanner *result = g_object_new(GRO_TYPE_SIMPLE_SCANNER, NULL);
	cat_ref_anounce(result);
	GroSimpleScannerPrivate *priv = gro_simple_scanner_get_instance_private(result);
	priv->utf8Scanner = cat_ref_ptr(scanner);
	priv->buffered_token = NULL;
	priv->mark = 0;
	priv->row = 0;
	priv->column = 0;
	priv->nextColumn = 0;
	priv->lastChar = 0;
	priv->streamStatus = CAT_STREAM_OK;
	priv->buffer = cat_ref_sink_ptr(cat_unichar_array_new());
	priv->inFullComment = FALSE;
	priv->filter_non_parsable = FALSE;
	priv->create_ast_tokens = FALSE;
	l_next_char(result);
	return result;
}


static void l_next_char(GroSimpleScanner *scanner) {
	GroSimpleScannerPrivate *priv = gro_simple_scanner_get_instance_private(scanner);
	if (priv->streamStatus==CAT_STREAM_OK) {
		gunichar nchar = cat_iutf8_scanner_next_char(priv->utf8Scanner, &(priv->streamStatus));
		priv->lastChar = nchar;
		priv->column = priv->nextColumn;
		if (nchar==0xa || nchar==0xd) {
			priv->row++;
			priv->nextColumn = 0;
		} else {
			priv->nextColumn++;
		}
	}
}

static GroToken *l_create_token(GroIScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val);

static GroToken *l_create_token_with_pos(GroSimpleScanner *scanner, int terminal_symbol, int left, int left_row, int right, int right_row) {
	return l_create_token(GRO_ISCANNER(scanner), terminal_symbol, -1, TRUE, FALSE, FALSE, left, left_row, right, right_row, NULL);
}


static GroToken *real_scanner_next_token(GroSimpleScanner *scanner) {
	GroSimpleScannerPrivate *priv = gro_simple_scanner_get_instance_private(scanner);
	GroToken *result = NULL;
	if (priv->buffered_token!=NULL) {
		result = priv->buffered_token;
		priv->buffered_token = NULL;
		return result;
	}
	cat_log_debug("scanning next");
	if (priv->streamStatus != CAT_STREAM_OK) {
		cat_log_debug("EOF");
		return l_create_token_with_pos(scanner, GROTBA_SYMBOL_TERM_EOF, 0, priv->row+1, 0, priv->row+1);
	}




	gboolean advance = FALSE;
	while(TRUE) {
		if (advance) {
			l_next_char(scanner);
			if (priv->streamStatus != CAT_STREAM_OK) {
				cat_log_debug("EOF");
				return l_create_token_with_pos(scanner, GROTBA_SYMBOL_TERM_EOF, 0, priv->row+1,0, priv->row+1);
			}
		}
		int row = priv->row;
		priv->mark = priv->column;
		priv->mark_row = priv->row;
		switch(priv->lastChar) {
			case 'A' :
			case 'a' : {
					result = l_create_token_with_pos(scanner, GROTBA_SYMBOL_TERM_A, priv->mark, priv->mark_row, priv->column+1, row);
					l_next_char(scanner);
					return result;
			}
			case 'B' :
			case 'b' : {
					result = l_create_token_with_pos(scanner, GROTBA_SYMBOL_TERM_B, priv->mark, priv->mark_row, priv->column+1, row);
					l_next_char(scanner);
					return result;
			}
			case 'C' :
			case 'c' : {
					result = l_create_token_with_pos(scanner, GROTBA_SYMBOL_TERM_C, priv->mark, priv->mark_row, priv->column+1, row);
					l_next_char(scanner);
					return result;
			}
			case 'D' :
			case 'd' : {
					result = l_create_token_with_pos(scanner, GROTBA_SYMBOL_TERM_D, priv->mark, priv->mark_row, priv->column+1, row);
					l_next_char(scanner);
					return result;
			}
			case 'E' :
			case 'e' : {
					result = l_create_token_with_pos(scanner, GROTBA_SYMBOL_TERM_E, priv->mark, priv->mark_row, priv->column+1, row);
					l_next_char(scanner);
					return result;
			}
			case 'F' :
			case 'f' : {
					result = l_create_token_with_pos(scanner, GROTBA_SYMBOL_TERM_F, priv->mark, priv->mark_row, priv->column+1, row);
					l_next_char(scanner);
					return result;
			}
			case 'G' :
			case 'g' : {
					result = l_create_token_with_pos(scanner, GROTBA_SYMBOL_TERM_G, priv->mark, priv->mark_row, priv->column+1, row);
					l_next_char(scanner);
					return result;
			}

			default: {
				advance = TRUE;
			} break;
		}
	}
	return result;
}


static GroToken *l_next_token(GroIScanner *self) {
	GroSimpleScanner *scanner = GRO_SIMPLE_SCANNER(self);
	GroToken *result = real_scanner_next_token(scanner);
//	if (priv->filter_non_parsable) {
//		if (result->sym==GROTBA_SYMBOL_TERM_FULL_COMMENT  || result->sym == GROTBA_SYMBOL_TERM_LINE_COMMENT) {
//			cat_unref_ptr(result);
//			return i_scanner_next_token(self);	// TODO remove recursion :(
//		}
//	}
	cat_log_debug("token[sym=%d, left=%d:%d, right=%d:%d]", result->sym, result->left, result->left_row, result->right, result->right_row);
	return result;
}


static GroToken *l_create_token(GroIScanner *self, int symbol, int parse_state, gboolean is_terminal, gboolean is_error, gboolean used_by_parser, int left, int left_row, int right, int right_row, GObject *val) {
	return gro_token_new_full_ext(symbol, parse_state, is_terminal, is_error, used_by_parser, left, left_row, right, right_row, val);
}


static void i_scanner_iface_init(GroIScannerInterface *iface) {
	iface->createToken = l_create_token;
	iface->next_token = l_next_token;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroSimpleScanner *instance = GRO_SIMPLE_SCANNER(self);
	GroSimpleScannerPrivate *priv = gro_simple_scanner_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
