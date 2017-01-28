/*
   File:    jagasttype.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 24, 2012
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

#include "jagasttype.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstType"
#include <logging/catlog.h>

struct _JagAstTypePrivate {
	void *dummy;
};

G_DEFINE_TYPE (JagAstType, jag_ast_type, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
CatStringWo *l_to_string(const JagAstType *ast_type) {
	CatStringWo *e_result = cat_string_wo_new_with("toString not implemented for class:");
	cat_string_wo_append_chars(e_result, ast_type==NULL ? "null" : g_type_name_from_instance((GTypeInstance *) ast_type));
	return e_result;
}

static void jag_ast_type_class_init(JagAstTypeClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstTypePrivate));


	clazz->toString = l_to_string;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

}

static void jag_ast_type_init(JagAstType *instance) {
	JagAstTypePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_AST_TYPE, JagAstTypePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagAstType *instance = JAG_AST_TYPE(object);
//	JagAstTypePrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void jag_ast_type_construct(JagAstType *ast_type) {
//	JagAstTypePrivate *priv = result->priv;
}




