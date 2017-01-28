/*
   File:    jagastgenericstype.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 23, 2012
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

#include "jagastgenericstype.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstGenericsType"
#include <logging/catlog.h>

struct _JagAstGenericsTypePrivate {
	JagAstType *base_type;
	CatArrayWo *e_generics;
};

G_DEFINE_TYPE(JagAstGenericsType, jag_ast_generics_type, JAG_TYPE_AST_TYPE)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatStringWo *l_to_string(const JagAstType *ast_type);

static void jag_ast_generics_type_class_init(JagAstGenericsTypeClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstGenericsTypePrivate));

	JagAstTypeClass *ast_type_class = JAG_AST_TYPE_CLASS(clazz);
	ast_type_class->toString = l_to_string;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_generics_type_init(JagAstGenericsType *instance) {
	JagAstGenericsTypePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_AST_GENERICS_TYPE, JagAstGenericsTypePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstGenericsType *instance = JAG_AST_GENERICS_TYPE(object);
	JagAstGenericsTypePrivate *priv = instance->priv;
	cat_unref_ptr(priv->base_type);
	cat_unref_ptr(priv->e_generics);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstGenericsType *jag_ast_generics_type_new(JagAstType *base_type, CatArrayWo *e_generics) {
	JagAstGenericsType *result = g_object_new(JAG_TYPE_AST_GENERICS_TYPE, NULL);
	cat_ref_anounce(result);
	JagAstGenericsTypePrivate *priv = result->priv;
	priv->base_type = cat_ref_ptr(base_type);
	priv->e_generics = cat_ref_ptr(e_generics);
	jag_ast_type_construct((JagAstType *) result);
	cat_log_debug("JagAstGenericsType=%p", result);
	return result;
}




static CatStringWo *l_to_string(const JagAstType *ast_type) {
	JagAstGenericsTypePrivate *priv = JAG_AST_GENERICS_TYPE_GET_PRIVATE(ast_type);

	CatStringWo *e_result = cat_string_wo_new_with("JagAstGenericsType[base=");
	if (priv->base_type) {
		cat_string_wo_append(e_result, JAG_AST_TYPE_GET_CLASS(priv->base_type)->toString(priv->base_type));
	} else {
		cat_string_wo_append_chars(e_result, "null");
	}

	if (priv->e_generics) {
		cat_string_wo_append_chars(e_result, ", generics<");
		CatIIterator *iter = cat_array_wo_iterator(priv->e_generics);
		gboolean is_first = TRUE;
		while(cat_iiterator_has_next(iter)) {
			JagAstType *ast_child_type = (JagAstType *) cat_iiterator_next(iter);
			if (is_first) {
				is_first = FALSE;
			} else {
				cat_string_wo_append_chars(e_result, ",");
			}
			if (ast_child_type) {
				cat_string_wo_append(e_result, JAG_AST_TYPE_GET_CLASS(ast_child_type)->toString(ast_child_type));
			} else {
				cat_string_wo_append_chars(e_result, "null");
			}
		}

		cat_string_wo_append_chars(e_result, ">");
	}

	cat_string_wo_append_chars(e_result, "]");
	return e_result;
}

