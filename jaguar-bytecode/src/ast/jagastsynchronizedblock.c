/*
   File:    jagastsynchronizedblock.c
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

#include "jagastsynchronizedblock.h"
#include "jagastistatement.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstSynchronizedBlock"
#include <logging/catlog.h>

struct _JagAstSynchronizedBlockPrivate {
	JagAstIExpression *sync_expression;
};

static void l_statement_iface_init(JagAstIStatementInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstSynchronizedBlock, jag_ast_synchronized_block, JAG_AST_TYPE_BLOCK, {
		G_ADD_PRIVATE(JagAstSynchronizedBlock)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ISTATEMENT, l_statement_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_synchronized_block_class_init(JagAstSynchronizedBlockClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_synchronized_block_init(JagAstSynchronizedBlock *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstSynchronizedBlock *instance = JAG_AST_SYNCHRONIZED_BLOCK(object);
	JagAstSynchronizedBlockPrivate *priv = jag_ast_synchronized_block_get_instance_private(instance);
	cat_unref_ptr(priv->sync_expression);
	G_OBJECT_CLASS(jag_ast_synchronized_block_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_synchronized_block_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstSynchronizedBlock *jag_ast_synchronized_block_new(JagAstIExpression *sync_expression) {
	JagAstSynchronizedBlock *result = g_object_new(JAG_AST_TYPE_SYNCHRONIZED_BLOCK, NULL);
	cat_ref_anounce(result);
	JagAstSynchronizedBlockPrivate *priv = jag_ast_synchronized_block_get_instance_private(result);
	jag_ast_block_construct((JagAstBlock *) result);
	priv->sync_expression = cat_ref_ptr(sync_expression);
	return result;
}


/********************* start JagAstIStatement implementation *********************/

static void l_statement_write_statement(JagAstIStatement *self, JagAstWriter *out) {
	JagAstSynchronizedBlock *instance = JAG_AST_SYNCHRONIZED_BLOCK(self);
	JagAstSynchronizedBlockPrivate *priv = jag_ast_synchronized_block_get_instance_private(instance);

	int slnr = jag_ast_block_get_statement_line_nr((JagAstBlock *) self);
	jag_ast_writer_set_at_least_line_nr(out, slnr);

	jag_ast_writer_print(out, cat_string_wo_new_with("synchronized("));
	jag_ast_iexpression_write(priv->sync_expression, out);
	jag_ast_writer_print(out, cat_string_wo_new_with(") "));

	JagAstIStatementInterface *p_iface = g_type_interface_peek_parent(JAG_AST_ISTATEMENT_GET_INTERFACE(self));
	p_iface->writeStatement(self, out);

	jag_ast_writer_print(out, cat_string_wo_new_with("\n"));
}

static void l_statement_iface_init(JagAstIStatementInterface *iface) {
	JagAstIStatementInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->setAtLeastLineNumber = p_iface->setAtLeastLineNumber;
	iface->writeStatement = l_statement_write_statement;
}

/********************* end JagAstIStatement implementation *********************/
