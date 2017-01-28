/*
   File:    jagastclazz.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 20, 2012
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

#include "jagastclazz.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstClazz"
#include <logging/catlog.h>

struct _JagAstClazzPrivate {
	JagNameType *name_type;
	CatArrayWo *e_field_declarations;
};

G_DEFINE_TYPE (JagAstClazz, jag_ast_clazz, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_clazz_class_init(JagAstClazzClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstClazzPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_clazz_init(JagAstClazz *instance) {
	JagAstClazzPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_AST_CLAZZ, JagAstClazzPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstClazz *instance = JAG_AST_CLAZZ(object);
	JagAstClazzPrivate *priv = instance->priv;
	cat_unref_ptr(priv->name_type);
	cat_unref_ptr(priv->e_field_declarations);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void jag_ast_clazz_construct(JagAstClazz *ast_clazz, JagNameType *name_type) {
	JagAstClazzPrivate *priv = JAG_AST_CLAZZ_GET_PRIVATE(ast_clazz);
	priv->name_type = cat_ref_ptr(name_type);
	priv->e_field_declarations = cat_array_wo_new();
	jag_name_type_set_implementation(name_type, (GObject *) ast_clazz);

	cat_log_debug("name=%s", cat_string_wo_getchars(jag_ast_clazz_get_name(ast_clazz)));
}


CatStringWo *jag_ast_clazz_get_name(JagAstClazz *ast_clazz) {
	JagAstClazzPrivate *priv = JAG_AST_CLAZZ_GET_PRIVATE(ast_clazz);
	JagNameType *name_type = priv->name_type;
	return name_type!=NULL ? jag_name_type_get_name(name_type) : NULL;
}


void jag_ast_clazz_add_field(JagAstClazz *ast_clazz, JagAstDeclarator *field_declarator) {
	JagAstClazzPrivate *priv = JAG_AST_CLAZZ_GET_PRIVATE(ast_clazz);
	cat_array_wo_append(priv->e_field_declarations, (GObject *) field_declarator);
}

CatArrayWo *jag_ast_clazz_enlist_fields(JagAstClazz *ast_clazz) {
	JagAstClazzPrivate *priv = JAG_AST_CLAZZ_GET_PRIVATE(ast_clazz);
	return cat_array_wo_clone(priv->e_field_declarations, CAT_CLONE_DEPTH_MAIN);
}
