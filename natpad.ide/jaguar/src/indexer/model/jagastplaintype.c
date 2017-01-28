/*
   File:    jagastplaintype.c
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

#include "jagastplaintype.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstPlainType"
#include <logging/catlog.h>

struct _JagAstPlainTypePrivate {
	CatStringWo *a_name;
	gboolean is_qualified_name;
};

G_DEFINE_TYPE (JagAstPlainType, jag_ast_plain_type, JAG_TYPE_AST_TYPE)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatStringWo *l_to_string(const JagAstType *ast_type) { return jag_ast_plain_type_to_string(JAG_AST_PLAIN_TYPE(ast_type)); }


#define DEF_PRIM(t) { \
	CatStringWo *bb = cat_string_wo_new_with(#t); \
	cat_ref_ignore(bb); \
	clazz->primitive_##t = jag_ast_plain_type_new(bb, TRUE); \
	cat_ref_ignore(clazz->primitive_##t); \
	} \


static void jag_ast_plain_type_class_init(JagAstPlainTypeClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstPlainTypePrivate));


	clazz->init_prims_once = 0;

	DEF_PRIM(boolean);
	DEF_PRIM(byte);
	DEF_PRIM(short);
	DEF_PRIM(char);
	DEF_PRIM(int);
	DEF_PRIM(long);
	DEF_PRIM(float);
	DEF_PRIM(double);

	JagAstTypeClass *type_class = JAG_AST_TYPE_CLASS(clazz);
	type_class->toString = l_to_string;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_plain_type_init(JagAstPlainType *instance) {
	JagAstPlainTypePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_AST_PLAIN_TYPE, JagAstPlainTypePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstPlainType *instance = JAG_AST_PLAIN_TYPE(object);
	JagAstPlainTypePrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_name);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstPlainType *jag_ast_plain_type_new(CatStringWo *a_name, gboolean isQualifiedName) {
	JagAstPlainType *result = g_object_new(JAG_TYPE_AST_PLAIN_TYPE, NULL);
	cat_ref_anounce(result);
	JagAstPlainTypePrivate *priv = result->priv;
	priv->a_name = cat_ref_ptr(a_name);
	priv->is_qualified_name = isQualifiedName;
	jag_ast_type_construct(JAG_AST_TYPE(result));
	return result;
}


CatStringWo *jag_ast_plain_type_get_name(const JagAstPlainType *plain_type) {
	return JAG_AST_PLAIN_TYPE_GET_PRIVATE(plain_type)->a_name;
}

gboolean jag_ast_plain_type_is_qualified(const JagAstPlainType *plain_type) {
	return JAG_AST_PLAIN_TYPE_GET_PRIVATE(plain_type)->is_qualified_name;
}

static JagAstPlainTypeClass *l_get_clazz_with_prims() {
//	JagAstPlainTypeClass *clazz = g_type_class_ref(JAG_TYPE_AST_PLAIN_TYPE);
//	if (g_once_init_enter(&(clazz->init_prims_once))) {
//		clazz->primitive_boolean = jag_ast_plain_type_new(cat_string_wo_new_with("boolean"), TRUE);
//		clazz->primitive_byte = jag_ast_plain_type_new(cat_string_wo_new_with("byte"), TRUE);
//		clazz->primitive_short = jag_ast_plain_type_new(cat_string_wo_new_with("short"), TRUE);
//		clazz->primitive_char = jag_ast_plain_type_new(cat_string_wo_new_with("char"), TRUE);
//		clazz->primitive_int = jag_ast_plain_type_new(cat_string_wo_new_with("int"), TRUE);
//		clazz->primitive_long = jag_ast_plain_type_new(cat_string_wo_new_with("long"), TRUE);
//		clazz->primitive_float = jag_ast_plain_type_new(cat_string_wo_new_with("float"), TRUE);
//		clazz->primitive_double = jag_ast_plain_type_new(cat_string_wo_new_with("double"), TRUE);
//	}
//	g_type_class_unref(clazz);
	/* well we just expect JagAstPlainTypeClass keeps on existing */
	JagAstPlainTypeClass *clazz = g_type_class_peek(JAG_TYPE_AST_PLAIN_TYPE);
	return clazz;
}


gboolean jag_ast_plain_type_is_a_primitive(const JagAstPlainType *plain_type) {
	JagAstPlainTypePrivate *priv = JAG_AST_PLAIN_TYPE_GET_PRIVATE(plain_type);
	if (priv->is_qualified_name) {
		JagAstPlainTypeClass *clazz = l_get_clazz_with_prims();
		if ((clazz->primitive_boolean==plain_type)
			|| (clazz->primitive_byte==plain_type)
			|| (clazz->primitive_short==plain_type)
			|| (clazz->primitive_char==plain_type)
			|| (clazz->primitive_int==plain_type)
			|| (clazz->primitive_long==plain_type)
			|| (clazz->primitive_float==plain_type)
			|| (clazz->primitive_double==plain_type))
		{
			return TRUE;
		}
	}
	return FALSE;
}



JagAstPlainType *jag_ast_plain_type_get_boolean() {
	JagAstPlainTypeClass *clazz = l_get_clazz_with_prims();
	return clazz->primitive_boolean;
}

JagAstPlainType *jag_ast_plain_type_get_byte() {
	JagAstPlainTypeClass *clazz = l_get_clazz_with_prims();
	return clazz->primitive_byte;
}

JagAstPlainType *jag_ast_plain_type_get_short() {
	JagAstPlainTypeClass *clazz = l_get_clazz_with_prims();
	return clazz->primitive_short;
}

JagAstPlainType *jag_ast_plain_type_get_char() {
	JagAstPlainTypeClass *clazz = l_get_clazz_with_prims();
	return clazz->primitive_char;
}

JagAstPlainType *jag_ast_plain_type_get_int() {
	JagAstPlainTypeClass *clazz = l_get_clazz_with_prims();
	return clazz->primitive_int;
}

JagAstPlainType *jag_ast_plain_type_get_long() {
	JagAstPlainTypeClass *clazz = l_get_clazz_with_prims();
	return clazz->primitive_long;
}

JagAstPlainType *jag_ast_plain_type_get_float() {
	JagAstPlainTypeClass *clazz = l_get_clazz_with_prims();
	return clazz->primitive_float;
}

JagAstPlainType *jag_ast_plain_type_get_double() {
	JagAstPlainTypeClass *clazz = l_get_clazz_with_prims();
	return clazz->primitive_double;
}



gboolean jag_ast_plain_type_equal(const JagAstPlainType *type_a, const JagAstPlainType *type_b) {
	if (type_a==type_b) {
		return TRUE;
	}
	if (type_a==NULL || type_b==NULL) {
		return TRUE;
	}
	JagAstPlainTypePrivate *priv_a = JAG_AST_PLAIN_TYPE_GET_PRIVATE(type_a);
	JagAstPlainTypePrivate *priv_b = JAG_AST_PLAIN_TYPE_GET_PRIVATE(type_b);
	if (priv_a->is_qualified_name!=priv_b->is_qualified_name) {
		return FALSE;
	}
	return cat_string_wo_equal(priv_a->a_name, priv_b->a_name);
}


CatStringWo *jag_ast_plain_type_to_string(const JagAstPlainType *plain_type) {
	JagAstPlainTypePrivate *priv = JAG_AST_PLAIN_TYPE_GET_PRIVATE(plain_type);
	CatStringWo *result = cat_string_wo_new_with("PlainType[");
	cat_string_wo_append(result, priv->a_name);
	if (!priv->is_qualified_name) {
		cat_string_wo_append_chars(result, ", not-qualified");
	}
	cat_string_wo_append_chars(result, "]");
	return result;
}


