/*
   File:    jagastargument.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 30, 2012
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

#include "jagastargument.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstArgument"
#include <logging/catlog.h>

struct _JagAstArgumentPrivate {
	JagAstDeclarationType *declaration_type;
	CatStringWo *a_name;
	JagAstModifiers *modifiers;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagAstArgument, jag_ast_argument, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_argument_class_init(JagAstArgumentClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_argument_init(JagAstArgument *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstArgument *instance = JAG_AST_ARGUMENT(object);
	JagAstArgumentPrivate *priv = jag_ast_argument_get_instance_private(instance);
	cat_unref_ptr(priv->declaration_type);
	cat_unref_ptr(priv->a_name);
	cat_unref_ptr(priv->modifiers);
	G_OBJECT_CLASS(jag_ast_argument_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_argument_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstArgument *jag_ast_argument_new(JagAstDeclarationType *declaration_type, CatStringWo *a_name) {
	JagAstArgument *result = g_object_new(JAG_AST_TYPE_ARGUMENT, NULL);
	cat_ref_anounce(result);
	JagAstArgumentPrivate *priv = jag_ast_argument_get_instance_private(result);
	priv->declaration_type = cat_ref_ptr(declaration_type);
	priv->a_name = cat_ref_ptr(a_name);
	priv->modifiers = jag_ast_modifiers_new(0);
	return result;
}


CatStringWo *jag_ast_argument_get_name(JagAstArgument *argument) {
	JagAstArgumentPrivate *priv = jag_ast_argument_get_instance_private(argument);
	return priv->a_name;
}


JagAstDeclarationType *jag_ast_argument_get_declaration_type(JagAstArgument *argument) {
	JagAstArgumentPrivate *priv = jag_ast_argument_get_instance_private(argument);
	return priv->declaration_type;
}

JagAstModifiers *jag_ast_argument_get_modifiers(JagAstArgument *argument) {
	JagAstArgumentPrivate *priv = jag_ast_argument_get_instance_private(argument);
	return priv->modifiers;
}


gboolean jag_ast_argument_equal(JagAstArgument *argument_a, JagAstArgument *argument_b) {
	if (argument_a==argument_b) {
		return TRUE;
	}
	if (argument_a==NULL || argument_b==NULL) {
		return FALSE;
	}
	JagAstArgumentPrivate *priv_a = jag_ast_argument_get_instance_private(argument_a);
	JagAstArgumentPrivate *priv_b = jag_ast_argument_get_instance_private(argument_b);
	return cat_string_wo_equal(priv_a->a_name, priv_b->a_name) &&
			jag_ast_declaration_type_equal(priv_a->declaration_type, priv_b->declaration_type) &&
			jag_ast_modifiers_equal(priv_a->modifiers, priv_b->modifiers);
}

CatStringWo *jag_ast_argument_as_text(JagAstArgument *argument, CatStringWo *e_dump_buffer) {
	JagAstArgumentPrivate *priv = jag_ast_argument_get_instance_private(argument);
	if (e_dump_buffer==NULL) {
		e_dump_buffer = cat_string_wo_new();
	}
	cat_string_wo_append_chars(e_dump_buffer, "JagAstArgument[");
	cat_string_wo_append_chars(e_dump_buffer, cat_string_wo_getchars(priv->a_name));
	cat_string_wo_append_chars(e_dump_buffer, ", ");
	jag_ast_declaration_type_as_text(priv->declaration_type, e_dump_buffer);
	cat_string_wo_append_char(e_dump_buffer, ']');
	return e_dump_buffer;
}
