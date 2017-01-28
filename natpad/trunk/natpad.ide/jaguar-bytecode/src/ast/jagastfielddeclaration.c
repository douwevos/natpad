/*
   File:    jagastfielddeclaration.c
   Project: jaguar-decompiler
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

#include "jagastfielddeclaration.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstFieldDeclaration"
#include <logging/catlog.h>

struct _JagAstFieldDeclarationPrivate {
	JagAstModifiers *modifiers;
	JagAstDeclarationType *fieldDescriptor;
	JagAstIdentifier *variableName;
	JagAstIExpression *value;
};

G_DEFINE_TYPE (JagAstFieldDeclaration, jag_ast_field_declaration, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_field_declaration_class_init(JagAstFieldDeclarationClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstFieldDeclarationPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_field_declaration_init(JagAstFieldDeclaration *instance) {
	JagAstFieldDeclarationPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_FIELD_DECLARATION, JagAstFieldDeclarationPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstFieldDeclaration *instance = JAG_AST_FIELD_DECLARATION(object);
	JagAstFieldDeclarationPrivate *priv = instance->priv;
	cat_unref_ptr(priv->modifiers);
	cat_unref_ptr(priv->fieldDescriptor);
	cat_unref_ptr(priv->variableName);
	cat_unref_ptr(priv->value);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstFieldDeclaration *jag_ast_field_declaration_new(JagAstModifiers *modifiers, JagAstDeclarationType *fieldDescriptor, JagAstIdentifier *variableName, JagAstIExpression *value) {
	JagAstFieldDeclaration *result = g_object_new(JAG_AST_TYPE_FIELD_DECLARATION, NULL);
	cat_ref_anounce(result);
	JagAstFieldDeclarationPrivate *priv = result->priv;
	priv->modifiers = cat_ref_ptr(modifiers);
	priv->fieldDescriptor = cat_ref_ptr(fieldDescriptor);
	priv->variableName = cat_ref_ptr(variableName);
	priv->value = cat_ref_ptr(value);
	return result;
}


JagAstIdentifier *jag_ast_field_declaration_get_variable(JagAstFieldDeclaration *field_declaration) {
	JagAstFieldDeclarationPrivate *priv = JAG_AST_FIELD_DECLARATION_GET_PRIVATE(field_declaration);
	return priv->variableName;
}



void jag_ast_field_declaration_write(JagAstFieldDeclaration *field_declaration, JagAstWriter *out) {
	JagAstFieldDeclarationPrivate *priv = JAG_AST_FIELD_DECLARATION_GET_PRIVATE(field_declaration);
////		if (modifiers!=null) {
////			for(Modifier modifier : modifiers) {
////				out.print(modifier.text);
////				out.print(" ");
////			}
////		}
	jag_ast_writer_print(out, jag_ast_modifiers_as_text(priv->modifiers));
	jag_ast_declaration_type_write(priv->fieldDescriptor, out);
	jag_ast_writer_print(out, cat_string_wo_new_with(" "));
	jag_ast_iexpression_write((JagAstIExpression *) priv->variableName, out);
	if (priv->value!=NULL) {
		jag_ast_writer_print(out, cat_string_wo_new_with(" = "));
		jag_ast_iexpression_write(priv->value, out);
	}
}

