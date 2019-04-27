/*
   File:    jagastmethod.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 8, 2012
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

#include "jagastmethod.h"
#include "jagastistatement.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstMethod"
#include <logging/catlog.h>

struct _JagAstMethodPrivate {
	JagAstModifiers *modifiers;
	JagAstMethodHeader *method_header;
	JagAstBlock *statement_block;
	int statement_line_nr;
};

static void l_statement_iface_init(JagAstIStatementInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstMethod, jag_ast_method, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstMethod)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ISTATEMENT, l_statement_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_method_class_init(JagAstMethodClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_method_init(JagAstMethod *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstMethod *instance = JAG_AST_METHOD(object);
	JagAstMethodPrivate *priv = jag_ast_method_get_instance_private(instance);
	cat_unref_ptr(priv->method_header);
	cat_unref_ptr(priv->modifiers);
	cat_unref_ptr(priv->statement_block);
	G_OBJECT_CLASS(jag_ast_method_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_method_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstMethod *jag_ast_method_new(JagAstModifiers *modifiers, JagAstMethodHeader *method_header, JagAstBlock *statement_block) {
	JagAstMethod *result = g_object_new(JAG_AST_TYPE_METHOD, NULL);
	cat_ref_anounce(result);
	JagAstMethodPrivate *priv = jag_ast_method_get_instance_private(result);
	priv->modifiers = cat_ref_ptr(modifiers);
	priv->method_header = cat_ref_ptr(method_header);
	priv->statement_block = cat_ref_ptr(statement_block);
	priv->statement_line_nr = -1;
	return result;
}


/********************* start JagAstIStatement implementation *********************/

static void l_statement_set_at_least_line_nr(JagAstIStatement *self, int at_least_line_nr) {
	JagAstMethod *instance = JAG_AST_METHOD(self);
	JagAstMethodPrivate *priv = jag_ast_method_get_instance_private(instance);
	priv->statement_line_nr = at_least_line_nr;
}

static void l_statement_write_statement(JagAstIStatement *self, JagAstWriter *out) {
	JagAstMethod *instance = JAG_AST_METHOD(self);
	JagAstMethodPrivate *priv = jag_ast_method_get_instance_private(instance);
	jag_ast_writer_set_at_least_line_nr(out, priv->statement_line_nr);

	if (priv->modifiers) {
		jag_ast_writer_print(out, jag_ast_modifiers_as_text(priv->modifiers));
	}
	jag_ast_method_header_write(priv->method_header, out);
	jag_ast_writer_print(out, cat_string_wo_new_with(" "));
	jag_ast_istatement_write_statement((JagAstIStatement *) priv->statement_block, out);
}

static void l_statement_iface_init(JagAstIStatementInterface *iface) {
	iface->setAtLeastLineNumber = l_statement_set_at_least_line_nr;
	iface->writeStatement = l_statement_write_statement;
}

/********************* end JagAstIStatement implementation *********************/
