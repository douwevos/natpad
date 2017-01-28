/*
   File:    grorunscannerbase.c
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Nov 11, 2016
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

#include "grorunscannerbase.h"
#include "../full/grorunfulltoken.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunScannerBase"
#include <logging/catlog.h>

struct _GroRunScannerBasePrivate {
	CatHashMapWo *keyword_mapping;
	CatArrayWo *connected_symbols;
	GroRunITokenFactory *token_factory;
	CatIUtf8Scanner *input;
	CatStreamStatus input_status;
	CatUnicharArray *lookahead_sec;
	int lookahead_sec_offset;
	gunichar *lookahead;
	int lookahead_max;
	gunichar next_char;
	int mark_row, mark_column;
	int row, left_column, right_column;
	int index_sequence;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunScannerBase, grorun_scanner_base, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunScannerBase)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gunichar *l_get_lookahead_buffer(GroRunScannerBase *scanner);
static GroRunSymbol *l_get_connected_symbol(GroRunScannerBase *scanner, int sym_conn_id);

static gunichar l_advance(GroRunScannerBase *scanner, int advance_flags);
static GroRunIToken *l_create_token(GroRunScannerBase *scanner, int conn_sym_id, long long end_row, int end_column, void *c_value);
static GroRunIToken *l_create_token_basic(GroRunScannerBase *scanner, int conn_sym_id, int nr_of_characters);
static GroRunIToken *l_scan_for_id_or_keyword(GroRunScannerBase *scanner, int conn_sym_id_for_identifier, gboolean case_sensitive);
static GroRunIToken *l_scan_for_end_of_line(GroRunScannerBase *scanner, int conn_sym_id_for_identifier, gboolean get_text);
static GroRunIToken *l_scan_for_quoted_string(GroRunScannerBase *scanner, int conn_sym_id_for_string);

static void l_markLocation(GroRunScannerBase *scanner);
static int l_getColumn(GroRunScannerBase *scanner);
static long long l_getRow(GroRunScannerBase *scanner);
static int l_getMarkedColumn(GroRunScannerBase *scanner);
static long long l_getMarkedRow(GroRunScannerBase *scanner);


static void grorun_scanner_base_class_init(GroRunScannerBaseClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	clazz->getLookaheadBuffer = l_get_lookahead_buffer;
	clazz->getConnectedSymbol = l_get_connected_symbol;
	clazz->advance = l_advance;
	clazz->createToken = l_create_token;
	clazz->createTokenBasic = l_create_token_basic;
	clazz->scanForIdOrKeyword = l_scan_for_id_or_keyword;
	clazz->scanForEndOfLine = l_scan_for_end_of_line;
	clazz->scanForQuotedString = l_scan_for_quoted_string;

	clazz->markLocation = l_markLocation;
	clazz->getColumn = l_getColumn;
	clazz->getRow = l_getRow;
	clazz->getMarkedColumn = l_getMarkedColumn;
	clazz->getMarkedRow = l_getMarkedRow;

}

static void grorun_scanner_base_init(GroRunScannerBase *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroRunScannerBase *instance = GRORUN_SCANNER_BASE(object);
	GroRunScannerBasePrivate *priv = grorun_scanner_base_get_instance_private(instance);
	cat_unref_ptr(priv->connected_symbols);
	cat_unref_ptr(priv->keyword_mapping);
	cat_unref_ptr(priv->input);
	cat_unref_ptr(priv->lookahead_sec);
	cat_unref_ptr(priv->token_factory);
	G_OBJECT_CLASS(grorun_scanner_base_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_scanner_base_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void grorun_scanner_base_construct(GroRunScannerBase *scanner_base, GroRunITokenFactory *token_factory, CatIUtf8Scanner *scanner, CatHashMapWo *keyword_mapping, CatArrayWo *connected_symbols, int lookahead_max) {
	GroRunScannerBasePrivate *priv = grorun_scanner_base_get_instance_private(scanner_base);


	priv->connected_symbols = cat_ref_ptr(connected_symbols);
	priv->keyword_mapping = cat_ref_ptr(keyword_mapping);

	priv->input_status = CAT_STREAM_OK;
	priv->input = cat_ref_ptr(scanner);
	priv->token_factory = cat_ref_ptr(token_factory);
	priv->lookahead_sec = NULL;

	priv->lookahead_max = lookahead_max;
	priv->lookahead = g_malloc(sizeof(gunichar)*lookahead_max);

	int idx;
	for(idx=1; idx<lookahead_max; idx++) {
		priv->lookahead[idx] = cat_iutf8_scanner_next_char(priv->input, &(priv->input_status));
	}
	priv->next_char = 0;
	priv->row = 0;
	priv->left_column = 0;
	priv->right_column = 0;
	priv->mark_row = 0;
	priv->mark_column = -1;
	priv->index_sequence = 0;
	l_advance(scanner_base, 0);
}


static void l_scan_one_unichar(GroRunScannerBasePrivate *priv) {
	int idx;
	for(idx=1; idx<priv->lookahead_max; idx++) {
		priv->lookahead[idx-1] = priv->lookahead[idx];
	}
	if (priv->lookahead_sec!=NULL) {
		priv->lookahead[priv->lookahead_max-1] = cat_unichar_array_char_at(priv->lookahead_sec, priv->lookahead_sec_offset++);
		if (priv->lookahead_sec_offset>cat_unichar_array_length(priv->lookahead_sec)) {
			cat_unref_ptr(priv->lookahead_sec);
			priv->lookahead_sec_offset = 0;
		}
	} else {
		priv->lookahead[priv->lookahead_max-1] = cat_iutf8_scanner_next_char(priv->input, &(priv->input_status));
	}
}


static gunichar l_advance_raw(GroRunScannerBasePrivate *priv) {
	l_scan_one_unichar(priv);

	priv->left_column = priv->right_column;
	gunichar result = priv->lookahead[0];
	if (result==0xa) {
		if (priv->lookahead[1]==0xd) {
			l_scan_one_unichar(priv);
		}
		priv->row++;
		priv->right_column = 0;
	} else 	if (result==0xd) {
		if (priv->lookahead[1]==0xa) {
			l_scan_one_unichar(priv);
		}
		priv->row++;
		priv->right_column = -1;
	} else if (result==-1) {
		return -1;
	} else {
		if (result < 0x80) {
			priv->right_column++;
		} else if (result< (1<<12)) {
			priv->right_column+= 2;
		} else if (result< (1<<18)) {
			priv->right_column+= 3;
		} else if (result< (1<<24)) {
			priv->right_column+= 4;
		} else if (result< (1<<30)) {
			priv->right_column+= 5;
		} else {
			priv->right_column+= 6;
		}
	}
	priv->next_char = result;
	return result;
}

static gunichar l_advance(GroRunScannerBase *scanner, int advance_flags) {
	GroRunScannerBasePrivate *priv = grorun_scanner_base_get_instance_private(scanner);
	if (advance_flags==0) {
		return l_advance_raw(priv);
	}
	while(TRUE) {
		gunichar fch = l_advance_raw(priv);
		if ((fch==0xa || fch==0xd) && ((advance_flags & GRORUN_ADVANCE_STRIP_LINE_BREAKS) !=0)) {
			continue;
		} else if ((fch==' ' || fch=='\t') && ((advance_flags & GRORUN_ADVANCE_STRIP_WHITE_SPACES) !=0)) {
			continue;
		}
		return fch;
	}
}


static gunichar *l_get_lookahead_buffer(GroRunScannerBase *scanner) {
	GroRunScannerBasePrivate *priv = grorun_scanner_base_get_instance_private(scanner);
	return priv->lookahead;
}

static GroRunSymbol *l_get_connected_symbol(GroRunScannerBase *scanner, int sym_conn_id) {
	GroRunScannerBasePrivate *priv = grorun_scanner_base_get_instance_private(scanner);
	return (GroRunSymbol *) cat_array_wo_get(priv->connected_symbols, sym_conn_id);
}


static GroRunIToken *l_create_token(GroRunScannerBase *scanner, int conn_sym_id, long long end_row, int end_column, void *c_value) {
	GroRunScannerBasePrivate *priv = grorun_scanner_base_get_instance_private(scanner);
	GroRunSymbol *symbol = (GroRunSymbol *) cat_array_wo_get(priv->connected_symbols, conn_sym_id);
	cat_log_debug("conn_sym_id=%d, symbol=%O", conn_sym_id, symbol);

	GroRunLocation *location = grorun_location_new(priv->mark_column, priv->mark_row, end_column, end_row);
	GroRunLeftRight lr;
	lr.left = priv->index_sequence++;
	lr.right = lr.left;
	GroRunIToken *result = grorun_itoken_factory_create_token(priv->token_factory, lr, symbol, (GObject *) c_value, location, NULL);
	if (GRORUN_IS_FULL_TOKEN(result)) {
		grorun_full_token_set_user_index((GroRunFullToken *) result, conn_sym_id);
	}
	cat_unref_ptr(location);
	cat_unref_ptr(c_value);
	return result;
}

static GroRunIToken *l_create_token_basic(GroRunScannerBase *scanner, int conn_sym_id, int nr_of_characters) {
	GroRunScannerBasePrivate *priv = grorun_scanner_base_get_instance_private(scanner);

	while(nr_of_characters>1) {
		l_advance_raw(priv);
		nr_of_characters--;
	}
	long long end_row = priv->row;
	int end_column = priv->right_column;
	l_advance_raw(priv);

	return l_create_token(scanner, conn_sym_id, end_row, end_column, NULL);
}


static GroRunIToken *l_scan_for_id_or_keyword(GroRunScannerBase *scanner, int conn_sym_id_for_identifier, gboolean case_sensitive) {
	GroRunScannerBasePrivate *priv = grorun_scanner_base_get_instance_private(scanner);

//	int lastCol = priv->column+1;
	CatStringWo *buf = cat_string_wo_new();
	while(g_unichar_isalpha(priv->lookahead[0]) || priv->lookahead[0]=='_' || g_unichar_isdigit(priv->lookahead[0])) {
		cat_string_wo_append_unichar(buf, priv->lookahead[0]);
//		lastCol = priv->column+1;
		l_advance(scanner, 0);
		if (priv->row!=priv->mark_row) {
			break;
		}
	}

	int connSymId = conn_sym_id_for_identifier;
	CatStringWo *keyword = NULL;
	if (!case_sensitive) {
		keyword = cat_string_wo_clone(buf, CAT_CLONE_DEPTH_FULL);
		cat_string_wo_to_lowercase(keyword);
	} else {
		keyword = cat_ref_ptr(buf);
	}
	cat_log_debug("keyword='%O'", keyword);
	CatInteger *ci = (CatInteger *) cat_hash_map_wo_get(priv->keyword_mapping, keyword);
	cat_unref_ptr(keyword);
	if (ci!=NULL) {
		connSymId = cat_integer_value(ci);
	}

	return l_create_token(scanner, connSymId, priv->mark_row, priv->left_column, buf);
}

static GroRunIToken *l_scan_for_end_of_line(GroRunScannerBase *scanner, int conn_sym_id_for_identifier, gboolean get_text) {
	GroRunScannerBasePrivate *priv = grorun_scanner_base_get_instance_private(scanner);

	CatStringWo *val = NULL;
	if (get_text) {
		val = cat_string_wo_new();
	}
	GroRunIToken *result = NULL;

	while(TRUE) {
		if (val) {
			cat_string_wo_append_unichar(val, priv->lookahead[0]);
		}
		gunichar ch = priv->lookahead[1];
		if (ch==-1 || ch==0xd || ch==0xa) {
			result = l_create_token(scanner, conn_sym_id_for_identifier, priv->mark_row, priv->right_column, val);
			l_advance(scanner, 0);
			break;
		}
		l_advance(scanner, 0);
	}

	return result;
}


static GroRunIToken *l_scan_for_quoted_string(GroRunScannerBase *scanner, int conn_sym_id_for_string) {
	GroRunScannerBasePrivate *priv = grorun_scanner_base_get_instance_private(scanner);

	GroRunIToken *result = NULL;

	gunichar mark_ch = priv->lookahead[0];
	int end_column = priv->right_column;
	gunichar ch = l_advance(scanner, 0);


	gboolean escaped = FALSE;

	while(TRUE) {
		gboolean is_mark_char = (ch==mark_ch && !escaped);
		if (ch==-1 || ch==0xd || ch==0xa || is_mark_char) {
			if (is_mark_char) {
				end_column = priv->right_column;
			}
			result = l_create_token(scanner, conn_sym_id_for_string, priv->mark_row, end_column, NULL);
			l_advance(scanner, 0);
			break;
		} else if (ch=='\\') {
			escaped = !escaped;
		} else {
			escaped = FALSE;
		}
		end_column = priv->right_column;
		ch = l_advance(scanner, 0);
	}

	return result;

}



static void l_markLocation(GroRunScannerBase *scanner) {
	GroRunScannerBasePrivate *priv = grorun_scanner_base_get_instance_private(scanner);
	priv->mark_column = priv->left_column;
	priv->mark_row = priv->row;
}

static int l_getColumn(GroRunScannerBase *scanner) {
	GroRunScannerBasePrivate *priv = grorun_scanner_base_get_instance_private(scanner);
	return priv->right_column;
}


static long long l_getRow(GroRunScannerBase *scanner) {
	GroRunScannerBasePrivate *priv = grorun_scanner_base_get_instance_private(scanner);
	return priv->row;
}


static int l_getMarkedColumn(GroRunScannerBase *scanner) {
	GroRunScannerBasePrivate *priv = grorun_scanner_base_get_instance_private(scanner);
	return priv->mark_column;
}


static long long l_getMarkedRow(GroRunScannerBase *scanner) {
	GroRunScannerBasePrivate *priv = grorun_scanner_base_get_instance_private(scanner);
	return priv->mark_row;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
