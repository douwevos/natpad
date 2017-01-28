/*
   File:    jagastargumentlist.c
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



#include "jagastargumentlist.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstArgumentList"
#include <logging/catlog.h>

struct _JagAstArgumentListPrivate {
	CatArrayWo *e_arguments;
};

G_DEFINE_TYPE (JagAstArgumentList, jag_ast_argument_list, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_argument_list_class_init(JagAstArgumentListClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstArgumentListPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_argument_list_init(JagAstArgumentList *instance) {
	JagAstArgumentListPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_ARGUMENT_LIST, JagAstArgumentListPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstArgumentList *instance = JAG_AST_ARGUMENT_LIST(object);
	JagAstArgumentListPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_arguments);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstArgumentList *jag_ast_argument_list_new() {
	JagAstArgumentList *result = g_object_new(JAG_AST_TYPE_ARGUMENT_LIST, NULL);
	cat_ref_anounce(result);
	JagAstArgumentListPrivate *priv = result->priv;
	priv->e_arguments = cat_array_wo_new();
	return result;
}


JagAstArgument *jag_ast_argument_list_get_at(JagAstArgumentList *argument_list, int index) {
	JagAstArgumentListPrivate *priv = JAG_AST_ARGUMENT_LIST_GET_PRIVATE(argument_list);
	if (index>=0 && index<cat_array_wo_size(priv->e_arguments)) {
		return (JagAstArgument *) cat_array_wo_get(priv->e_arguments, index);
	}
	return NULL;
}

void jag_ast_argument_list_add(JagAstArgumentList *argument_list, JagAstArgument *argument) {
	JagAstArgumentListPrivate *priv = JAG_AST_ARGUMENT_LIST_GET_PRIVATE(argument_list);
	cat_array_wo_append(priv->e_arguments, (GObject *) argument);
}

int jag_ast_argument_list_count(JagAstArgumentList *argument_list) {
	JagAstArgumentListPrivate *priv = JAG_AST_ARGUMENT_LIST_GET_PRIVATE(argument_list);
	return cat_array_wo_size(priv->e_arguments);
}


gboolean jag_ast_argument_list_equal(JagAstArgumentList *argument_list_a, JagAstArgumentList *argument_list_b) {
	if (argument_list_a==argument_list_b) {
		return TRUE;
	}
	if (argument_list_a==NULL || argument_list_b==NULL) {
		return FALSE;
	}
	JagAstArgumentListPrivate *priv_a = JAG_AST_ARGUMENT_LIST_GET_PRIVATE(argument_list_a);
	JagAstArgumentListPrivate *priv_b = JAG_AST_ARGUMENT_LIST_GET_PRIVATE(argument_list_b);
	return cat_array_wo_equal(priv_a->e_arguments, priv_b->e_arguments, (GEqualFunc) jag_ast_argument_equal);
}


CatStringWo *jag_ast_argument_list_as_text(JagAstArgumentList *argument_list, CatStringWo *e_dump_buffer) {
	JagAstArgumentListPrivate *priv = JAG_AST_ARGUMENT_LIST_GET_PRIVATE(argument_list);
	if (e_dump_buffer==NULL) {
		e_dump_buffer = cat_string_wo_new();
	}
	int arg_count = cat_array_wo_size(priv->e_arguments);
	int arg_idx;
	for(arg_idx=0; arg_idx<arg_count; arg_idx++) {
		if (arg_idx!=0) {
			cat_string_wo_append_chars(e_dump_buffer, ", ");
		}
		JagAstArgument *argument = (JagAstArgument *) cat_array_wo_get(priv->e_arguments, arg_idx);
		jag_ast_argument_as_text(argument, e_dump_buffer);
	}
	return e_dump_buffer;
}
