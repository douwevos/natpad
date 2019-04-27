/*
   File:    jagastwriter.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 28, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "jagastwriter.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstWriter"
#include <logging/catlog.h>

struct _JagAstWriterPrivate {
	CatArrayWo *e_lines;
	CatStringWo *e_last_line;

	CatStringWo *a_last_index_text;
	CatStringWo *a_indent_text;
	int indent;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagAstWriter, jag_ast_writer, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void l_store_and_clear_line(JagAstWriter *writer);
static void l_rebuild_indent(JagAstWriter *writer);

static void jag_ast_writer_class_init(JagAstWriterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_writer_init(JagAstWriter *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstWriter *instance = JAG_AST_WRITER(object);
	JagAstWriterPrivate *priv = jag_ast_writer_get_instance_private(instance);
	cat_unref_ptr(priv->e_lines);
	cat_unref_ptr(priv->e_last_line);
	cat_unref_ptr(priv->a_last_index_text);
	cat_unref_ptr(priv->a_indent_text);
	G_OBJECT_CLASS(jag_ast_writer_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_writer_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstWriter *jag_ast_writer_new() {
	JagAstWriter *result = g_object_new(JAG_AST_TYPE_WRITER, NULL);
	cat_ref_anounce(result);
	JagAstWriterPrivate *priv = jag_ast_writer_get_instance_private(result);
	priv->e_lines = cat_array_wo_new();
	priv->e_last_line = cat_string_wo_new();

	priv->a_last_index_text = CAT_S(cat_s_empty_string);
	priv->a_indent_text = CAT_S(cat_s_empty_string);
	priv->indent = 0;
	return result;
}


void jag_ast_writer_set_at_least_line_nr(JagAstWriter *writer, int lineNumber) {
	JagAstWriterPrivate *priv = jag_ast_writer_get_instance_private(writer);
	if (lineNumber>=0) {
		while(cat_array_wo_size(priv->e_lines)<lineNumber) {
			l_store_and_clear_line(writer);
		}
	}
}


JagAstWriter *jag_ast_writer_print(JagAstWriter *writer, CatStringWo *a_text) {
	JagAstWriterPrivate *priv = jag_ast_writer_get_instance_private(writer);
	int lastIndex = 0;
	int idx;
	for(idx=0; idx<cat_string_wo_length(a_text); idx++) {
		gboolean lineBreak = cat_string_wo_char_at(a_text, idx)<32;
		if (lineBreak) {
			CatStringWo *sub = cat_string_wo_new_sub(a_text, lastIndex, idx-lastIndex);
			cat_string_wo_append(priv->e_last_line, cat_ref_sink_ptr(sub));
			cat_unref_ptr(sub);
			l_store_and_clear_line(writer);
			lastIndex = idx+1;
		}
	}
	CatStringWo *a_sub = cat_string_wo_new_sub(a_text, lastIndex, cat_string_wo_length(a_text)-lastIndex);
	cat_string_wo_append(priv->e_last_line, cat_ref_sink_ptr(a_sub));
	cat_unref_ptr(a_sub);
	return writer;
}

void jag_ast_writer_clear(JagAstWriter *writer) {
	JagAstWriterPrivate *priv = jag_ast_writer_get_instance_private(writer);
	cat_array_wo_clear(priv->e_lines);
	cat_string_wo_clear(priv->e_last_line);
	priv->indent = 0;
	cat_unref(priv->a_last_index_text);
	cat_unref(priv->a_indent_text);
	priv->a_last_index_text = CAT_S(cat_s_empty_string);
	priv->a_indent_text = CAT_S(cat_s_empty_string);
}

static void l_store_and_clear_line(JagAstWriter *writer) {
	JagAstWriterPrivate *priv = jag_ast_writer_get_instance_private(writer);

	CatStringWo *e_complete_line = cat_string_wo_clone(priv->a_last_index_text, CAT_CLONE_DEPTH_MAIN);
	cat_string_wo_append(e_complete_line, priv->e_last_line);
	e_complete_line = cat_string_wo_anchor(e_complete_line, 0);
	cat_array_wo_append(priv->e_lines, (GObject *) e_complete_line);
	cat_unref_ptr(e_complete_line);
	cat_string_wo_clear(priv->e_last_line);
	cat_ref_swap(priv->a_last_index_text, priv->a_indent_text);
	priv->e_last_line = cat_string_wo_new();
}

void jag_ast_writer_increase_indent(JagAstWriter *writer) {
	JagAstWriterPrivate *priv = jag_ast_writer_get_instance_private(writer);
	priv->indent++;
	l_rebuild_indent(writer);
}

void jag_ast_writer_decrease_indent(JagAstWriter *writer) {
	JagAstWriterPrivate *priv = jag_ast_writer_get_instance_private(writer);
	priv->indent--;
	l_rebuild_indent(writer);
}

CatStringWo *jag_ast_writer_to_string(JagAstWriter *writer) {
	JagAstWriterPrivate *priv = jag_ast_writer_get_instance_private(writer);
	CatStringWo *e_buf = cat_string_wo_new();
	int idx;
	for(idx=0; idx<cat_array_wo_size(priv->e_lines); idx++) {
		CatStringWo *a_line = (CatStringWo *) cat_array_wo_get(priv->e_lines, idx);
//			buf.append(""+idx+":").append(line).append("\n");
		cat_string_wo_append(e_buf, a_line);
		cat_string_wo_append_chars(e_buf, "\n");
	}
	if (cat_string_wo_length(priv->e_last_line)>0) {
		cat_string_wo_append(e_buf, priv->e_last_line);
	}

	return cat_string_wo_anchor(e_buf, 0);
}

static void l_rebuild_indent(JagAstWriter *writer) {
	JagAstWriterPrivate *priv = jag_ast_writer_get_instance_private(writer);
	CatStringWo *e_newIndent = cat_string_wo_new();
	int idx;
	for(idx=0; idx<priv->indent; idx++) {
		cat_string_wo_append_chars(e_newIndent, "    ");
	}

	CatStringWo *a_nIndentText = cat_string_wo_anchor(e_newIndent, TRUE);
	cat_ref_swap(priv->a_indent_text, a_nIndentText);
	cat_unref_ptr(a_nIndentText);
	if (cat_string_wo_length(priv->e_last_line)==0) {
		cat_ref_swap(priv->a_last_index_text, priv->a_indent_text);
	}
}
