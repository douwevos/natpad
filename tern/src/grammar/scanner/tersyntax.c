/*
   File:    tersyntax.c
   Project: tern
   Author:  Douwe Vos
   Date:    Jun 2, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#include "tersyntax.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerSyntax"
#include <logging/catlog.h>

struct _TerSyntaxPrivate {
	TerSyntaxMode mode;

	TerUnicharNode *keyword_node;
	CatHashSet *keyword_chars;

	CatUnicharArray *hex_prefix;

	CatUnicharArray *multi_line_comment_start;
	CatUnicharArray *multi_line_comment_end;

	CatUnicharArray *single_comment;
	int single_comment_col;

	CatUnicharArray *single_comment_alt;
	int single_comment_col_alt;

	CatHashSet *bracket_set;
	CatHashSet *operator_char_set;

	gunichar string_start, string_end;
};

G_DEFINE_TYPE_WITH_PRIVATE(TerSyntax, ter_syntax, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void ter_syntax_class_init(TerSyntaxClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_syntax_init(TerSyntax *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TerSyntax *instance = TER_SYNTAX(object);
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(instance);
	cat_unref_ptr(priv->keyword_node);
	cat_unref_ptr(priv->keyword_chars);
	cat_unref_ptr(priv->hex_prefix);
	cat_unref_ptr(priv->multi_line_comment_start);
	cat_unref_ptr(priv->multi_line_comment_end);
	cat_unref_ptr(priv->single_comment);
	cat_unref_ptr(priv->single_comment_alt);
	cat_unref_ptr(priv->bracket_set);
	cat_unref_ptr(priv->operator_char_set);
	G_OBJECT_CLASS(ter_syntax_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(ter_syntax_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

TerSyntax *ter_syntax_new() {
	TerSyntax *result = g_object_new(TER_TYPE_SYNTAX, NULL);
	cat_ref_anounce(result);
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(result);
	priv->keyword_node = ter_unichar_node_new((gunichar) 0, 0);
	priv->keyword_chars = cat_hash_set_new((GHashFunc) cat_unichar_hash, (GEqualFunc) cat_unichar_equal);
	priv->hex_prefix = NULL;
	priv->bracket_set = cat_hash_set_new((GHashFunc) cat_unichar_hash, (GEqualFunc) cat_unichar_equal);
	priv->operator_char_set = cat_hash_set_new((GHashFunc) cat_unichar_hash, (GEqualFunc) cat_unichar_equal);
	priv->mode = TER_MODE_TEXT;
	priv->single_comment = NULL;
	priv->single_comment_col = 0;
	priv->single_comment_alt = NULL;
	priv->single_comment_col_alt = 0;
	priv->multi_line_comment_start = NULL;
	priv->multi_line_comment_end = NULL;
	priv->string_start = 0;
	priv->string_end = 0;
	return result;
}

TerUnicharNode *ter_syntax_create_keyword_end_node(TerSyntax *syntax, CatUnicharArray *key_array) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	int count = cat_unichar_array_length(key_array);
	int idx;
	TerUnicharNode *node = priv->keyword_node;
	for(idx=0; idx<count; idx++) {
		gunichar key_char = cat_unichar_array_char_at(key_array, idx);
		cat_log_debug("idx:%d, key_char=%c, node=%p", idx, (char) key_char, node);
		node = ter_unichar_node_get_or_create(node, key_char);
		CatUnichar *uchobj = cat_unichar_new(key_char);
		cat_hash_set_add(priv->keyword_chars, (GObject *) uchobj);
		cat_unref_ptr(uchobj);
	}
	cat_log_debug("node=%p", node);
	return node;
}


TerUnicharNode *ter_syntax_get_keyword_node(TerSyntax *syntax) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return priv->keyword_node;
}

gboolean ter_syntax_is_keyword_char(TerSyntax *syntax, CatUnichar *uchobj) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return cat_hash_set_get(priv->keyword_chars, (GObject *) uchobj)!=NULL;
}


void ter_syntax_set_mode(TerSyntax *syntax, TerSyntaxMode new_mode) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	priv->mode = new_mode;
}

TerSyntaxMode ter_syntax_get_mode(TerSyntax *syntax) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return priv->mode;
}

CatHashSet *ter_syntax_get_bracket_set(TerSyntax *syntax) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return priv->bracket_set;
}

gboolean ter_syntax_is_bracket_char(TerSyntax *syntax, CatUnichar *uchobj) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return cat_hash_set_get(priv->bracket_set, (GObject *) uchobj)!=NULL;
}

CatHashSet *ter_syntax_get_operator_char_set(TerSyntax *syntax) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return priv->operator_char_set;
}

gboolean ter_syntax_is_operator_char(TerSyntax *syntax, CatUnichar *uchobj) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return cat_hash_set_get(priv->operator_char_set, (GObject *) uchobj)!=NULL;
}

void ter_syntax_set_hex_prefix(TerSyntax *syntax, CatUnicharArray *new_hex_prefix) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	cat_ref_sink_swap(priv->hex_prefix, new_hex_prefix);
}

CatUnicharArray *ter_syntax_get_hex_prefix(TerSyntax *syntax) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return priv->hex_prefix;
}

void ter_syntax_set_single_comment(TerSyntax *syntax, CatUnicharArray *new_single_comment) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	cat_ref_sink_swap(priv->single_comment, new_single_comment);
}

CatUnicharArray *ter_syntax_get_single_comment(TerSyntax *syntax) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return priv->single_comment;
}

void ter_syntax_set_single_comment_column(TerSyntax *syntax, int new_column) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	priv->single_comment_col = new_column;
}

int ter_syntax_get_single_comment_column(TerSyntax *syntax) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return priv->single_comment_col;
}

void ter_syntax_set_single_comment_alt(TerSyntax *syntax, CatUnicharArray *new_single_comment) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	cat_ref_sink_swap(priv->single_comment_alt, new_single_comment);
}

CatUnicharArray *ter_syntax_get_single_comment_alt(TerSyntax *syntax) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return priv->single_comment_alt;
}

void ter_syntax_set_single_comment_column_alt(TerSyntax *syntax, int new_column) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	priv->single_comment_col_alt = new_column;
}

int ter_syntax_get_single_comment_column_alt(TerSyntax *syntax) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return priv->single_comment_col_alt;
}


void ter_syntax_set_multi_line_comment_start(TerSyntax *syntax, CatUnicharArray *new_multi_line_comment_start) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	cat_ref_sink_swap(priv->multi_line_comment_start, new_multi_line_comment_start);
}

CatUnicharArray *ter_syntax_get_multi_line_comment_start(TerSyntax *syntax) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return priv->multi_line_comment_start;
}

void ter_syntax_set_multi_line_comment_end(TerSyntax *syntax, CatUnicharArray *new_multi_line_comment_end) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	cat_ref_sink_swap(priv->multi_line_comment_end, new_multi_line_comment_end);
}

CatUnicharArray *ter_syntax_get_multi_line_comment_end(TerSyntax *syntax) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return priv->multi_line_comment_end;
}


void ter_syntax_set_string_start(TerSyntax *syntax, gunichar string_start) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	priv->string_start = string_start;
}

gunichar ter_syntax_get_string_start(TerSyntax *syntax) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return priv->string_start;
}

void ter_syntax_set_string_end(TerSyntax *syntax, gunichar string_end) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	priv->string_end = string_end;
}

gunichar ter_syntax_get_string_end(TerSyntax *syntax) {
	TerSyntaxPrivate *priv = ter_syntax_get_instance_private(syntax);
	return priv->string_end;
}
