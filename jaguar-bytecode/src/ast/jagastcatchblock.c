/*
   File:    jagastcatchblock.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 6, 2012
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

#include "jagastcatchblock.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstCatchBlock"
#include <logging/catlog.h>

struct _JagAstCatchBlockPrivate {
	JagAstFieldDeclaration *field_declaration;
};

static void l_statement_iface_init(JagAstIStatementInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstCatchBlock, jag_ast_catch_block, JAG_AST_TYPE_BLOCK, {
		G_ADD_PRIVATE(JagAstCatchBlock)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ISTATEMENT, l_statement_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_catch_block_class_init(JagAstCatchBlockClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_catch_block_init(JagAstCatchBlock *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstCatchBlock *instance = JAG_AST_CATCH_BLOCK(object);
	JagAstCatchBlockPrivate *priv = jag_ast_catch_block_get_instance_private(instance);
	cat_unref_ptr(priv->field_declaration);
	G_OBJECT_CLASS(jag_ast_catch_block_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_catch_block_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstCatchBlock *jag_ast_catch_block_new(JagAstFieldDeclaration *field_declaration) {
	JagAstCatchBlock *result = g_object_new(JAG_AST_TYPE_CATCH_BLOCK, NULL);
	cat_ref_anounce(result);
	JagAstCatchBlockPrivate *priv = jag_ast_catch_block_get_instance_private(result);
	jag_ast_block_construct((JagAstBlock *) result);
	priv->field_declaration = cat_ref_ptr(field_declaration);
	return result;
}


/********************* start JagAstIStatement implementation *********************/

static void l_statement_write_statement(JagAstIStatement *self, JagAstWriter *out) {
	JagAstCatchBlock *instance = JAG_AST_CATCH_BLOCK(self);
	JagAstCatchBlockPrivate *priv = jag_ast_catch_block_get_instance_private(instance);

	jag_ast_writer_print(out, cat_string_wo_new_with(" catch("));
	jag_ast_field_declaration_write(priv->field_declaration, out);
	jag_ast_writer_print(out, cat_string_wo_new_with(") "));

	JagAstIStatementInterface *p_iface = g_type_interface_peek_parent(JAG_AST_ISTATEMENT_GET_INTERFACE(self));
	p_iface->writeStatement(self, out);
}

static void l_statement_iface_init(JagAstIStatementInterface *iface) {
	JagAstIStatementInterface *p_iface = g_type_interface_peek_parent(iface);

	iface->setAtLeastLineNumber = p_iface->setAtLeastLineNumber;
	iface->writeStatement = l_statement_write_statement;
}

/********************* end JagAstIStatement implementation *********************/
