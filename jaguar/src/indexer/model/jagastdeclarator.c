/*
   File:    jagastdeclarator.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 25, 2012
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

#include "jagastdeclarator.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstDeclarator"
#include <logging/catlog.h>

struct _JagAstDeclaratorPrivate {
	CatStringWo *a_declarator_id;
	int dimension_count;
	JagAstType *base_type;
	JagAstIndexerModifiers *modifiers;
};

G_DEFINE_TYPE (JagAstDeclarator, jag_ast_declarator, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_declarator_class_init(JagAstDeclaratorClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstDeclaratorPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_declarator_init(JagAstDeclarator *instance) {
	JagAstDeclaratorPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_AST_DECLARATOR, JagAstDeclaratorPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstDeclarator *instance = JAG_AST_DECLARATOR(object);
	JagAstDeclaratorPrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_declarator_id);
	cat_unref_ptr(priv->base_type);
	cat_unref_ptr(priv->modifiers);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstDeclarator *jag_ast_declarator_new(CatStringWo *id) {
	JagAstDeclarator *result = g_object_new(JAG_TYPE_AST_DECLARATOR, NULL);
	cat_ref_anounce(result);
	JagAstDeclaratorPrivate *priv = result->priv;
	priv->a_declarator_id = cat_ref_ptr(id);
	priv->dimension_count = 0;
	priv->base_type = NULL;
	priv->modifiers = NULL;
	return result;
}


void jag_ast_declarator_add_dimension(JagAstDeclarator *declarator) {
	JagAstDeclaratorPrivate *priv = JAG_AST_DECLARATOR_GET_PRIVATE(declarator);
	priv->dimension_count++;
}


void jag_ast_declarator_set_base_type(JagAstDeclarator *declarator, JagAstType *base_type) {
	JagAstDeclaratorPrivate *priv = JAG_AST_DECLARATOR_GET_PRIVATE(declarator);
	cat_ref_swap(priv->base_type, base_type);
}

CatStringWo *jag_ast_declarator_get_id(JagAstDeclarator *declarator) {
	return JAG_AST_DECLARATOR_GET_PRIVATE(declarator)->a_declarator_id;
}



void jag_ast_declarator_set_modifiers(JagAstDeclarator *declarator, JagAstIndexerModifiers *modifiers) {
	JagAstDeclaratorPrivate *priv = JAG_AST_DECLARATOR_GET_PRIVATE(declarator);
	cat_ref_swap(priv->modifiers, modifiers);
}

JagAstIndexerModifiers *jag_ast_declarator_get_modifiers(JagAstDeclarator *declarator) {
	return JAG_AST_DECLARATOR_GET_PRIVATE(declarator)->modifiers;
}


