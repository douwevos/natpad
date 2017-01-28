/*
   File:    jagastblock.c
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



#include "jagastblock.h"
#include "jagastreturnstatement.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstBlock"
#include <logging/catlog.h>

struct _JagAstBlockPrivate {
	CatArrayWo *e_statement_list;			/* JagAstIStatement */
	int statement_line_nr;
};

static void l_statement_iface_init(JagAstIStatementInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstBlock, jag_ast_block, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ISTATEMENT, l_statement_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_block_class_init(JagAstBlockClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstBlockPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_block_init(JagAstBlock *instance) {
	JagAstBlockPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_BLOCK, JagAstBlockPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstBlock *instance = JAG_AST_BLOCK(object);
	JagAstBlockPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_statement_list);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void jag_ast_block_construct(JagAstBlock *block) {
	JagAstBlockPrivate *priv = JAG_AST_BLOCK_GET_PRIVATE(block);
	priv->e_statement_list = cat_array_wo_new();
	priv->statement_line_nr = -1;
}


JagAstBlock *jag_ast_block_new() {
	JagAstBlock *result = g_object_new(JAG_AST_TYPE_BLOCK, NULL);
	cat_ref_anounce(result);
	jag_ast_block_construct(result);
	return result;
}

void jag_ast_block_append_statement(JagAstBlock *block, JagAstIStatement *statement) {
	JagAstBlockPrivate *priv = JAG_AST_BLOCK_GET_PRIVATE(block);
	cat_array_wo_append(priv->e_statement_list, (GObject *) statement);
}



void jag_ast_block_remove_last_return(JagAstBlock *block) {
	JagAstBlockPrivate *priv = JAG_AST_BLOCK_GET_PRIVATE(block);
	if (cat_array_wo_size(priv->e_statement_list)==0) {
		return;
	}
	JagAstIStatement *statement = (JagAstIStatement *) cat_array_wo_get_last(priv->e_statement_list);
	if (JAG_AST_IS_RETURN_STATEMENT(statement)) {
		if (jag_ast_return_statement_get_expression((JagAstReturnStatement *) statement)==NULL) {
			cat_array_wo_remove_last(priv->e_statement_list, NULL);
		}
	}
}

int jag_ast_block_get_statement_line_nr(JagAstBlock *block) {
	return JAG_AST_BLOCK_GET_PRIVATE(block)->statement_line_nr;
}

JagAstIStatement *jag_ast_block_get_statement_at(JagAstBlock *block, int index) {
	return (JagAstIStatement *) cat_array_wo_get(JAG_AST_BLOCK_GET_PRIVATE(block)->e_statement_list, index);
}



/********************* start JagAstIStatement implementation *********************/


static void l_statement_set_at_least_line_nr(JagAstIStatement *self, int at_least_line_nr) {
	JagAstBlockPrivate *priv = JAG_AST_BLOCK_GET_PRIVATE(self);
	priv->statement_line_nr = at_least_line_nr;
}



static void l_statement_write_statement(JagAstIStatement *self, JagAstWriter *out) {
	JagAstBlockPrivate *priv = JAG_AST_BLOCK_GET_PRIVATE(self);
	jag_ast_writer_set_at_least_line_nr(out, priv->statement_line_nr);

	jag_ast_writer_print(out, cat_string_wo_new_with("{\n"));
	jag_ast_writer_increase_indent(out);
	CatIIterator *iter = cat_array_wo_iterator(priv->e_statement_list);
	while(cat_iiterator_has_next(iter)) {
		JagAstIStatement *statement = (JagAstIStatement *) cat_iiterator_next(iter);
		jag_ast_istatement_write_statement(statement, out);
	}
	cat_unref_ptr(iter);
	jag_ast_writer_decrease_indent(out);
	jag_ast_writer_print(out, cat_string_wo_new_with("}"));
}

static void l_statement_iface_init(JagAstIStatementInterface *iface) {
	iface->setAtLeastLineNumber = l_statement_set_at_least_line_nr;
	iface->writeStatement = l_statement_write_statement;
}

/********************* end JagAstIStatement implementation *********************/

