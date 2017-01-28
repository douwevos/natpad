/*
   File:    jagastfinallyblock.c
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

#include "jagastfinallyblock.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstFinallyBlock"
#include <logging/catlog.h>

struct _JagAstFinallyBlockPrivate {
	void *dummy;
};

static void l_statement_iface_init(JagAstIStatementInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstFinallyBlock, jag_ast_finally_block, JAG_AST_TYPE_BLOCK, {
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ISTATEMENT, l_statement_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_finally_block_class_init(JagAstFinallyBlockClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstFinallyBlockPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_finally_block_init(JagAstFinallyBlock *instance) {
	JagAstFinallyBlockPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_FINALLY_BLOCK, JagAstFinallyBlockPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagAstFinallyBlock *instance = JAG_AST_FINALLY_BLOCK(object);
//	JagAstFinallyBlockPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstFinallyBlock *jag_ast_finally_block_new() {
	JagAstFinallyBlock *result = g_object_new(JAG_AST_TYPE_FINALLY_BLOCK, NULL);
	cat_ref_anounce(result);
//	JagAstFinallyBlockPrivate *priv = result->priv;
	jag_ast_block_construct((JagAstBlock *) result);
	return result;
}








/********************* start JagAstIStatement implementation *********************/

static void l_statement_write_statement(JagAstIStatement *self, JagAstWriter *out) {

	jag_ast_writer_print(out, cat_string_wo_new_with(" finally "));

	JagAstIStatementInterface *p_iface = g_type_interface_peek_parent(JAG_AST_ISTATEMENT_GET_INTERFACE(self));
	p_iface->writeStatement(self, out);

}

static void l_statement_iface_init(JagAstIStatementInterface *iface) {
	JagAstIStatementInterface *p_iface = g_type_interface_peek_parent(iface);

	iface->setAtLeastLineNumber = p_iface->setAtLeastLineNumber;
	iface->writeStatement = l_statement_write_statement;
}

/********************* end JagAstIStatement implementation *********************/
