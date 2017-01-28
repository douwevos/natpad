/*
   File:    jagastdeclarationstatement.c
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


#include "jagastdeclarationstatement.h"
#include "jagastistatement.h"
#include "jagastiastnode.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstDeclarationStatement"
#include <logging/catlog.h>

struct _JagAstDeclarationStatementPrivate {
	JagAstDeclarationType *declaration_type;
	JagAstAssignment *assignment;
	int statement_line_nr;
};

static void l_statement_iface_init(JagAstIStatementInterface *iface);
static void l_ast_node_iface_init(JagAstIAstNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstDeclarationStatement, jag_ast_declaration_statement, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ISTATEMENT, l_statement_iface_init);
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_IAST_NODE, l_ast_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_declaration_statement_class_init(JagAstDeclarationStatementClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstDeclarationStatementPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_declaration_statement_init(JagAstDeclarationStatement *instance) {
	JagAstDeclarationStatementPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_DECLARATION_STATEMENT, JagAstDeclarationStatementPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstDeclarationStatement *instance = JAG_AST_DECLARATION_STATEMENT(object);
	JagAstDeclarationStatementPrivate *priv = instance->priv;
	cat_unref_ptr(priv->declaration_type);
	cat_unref_ptr(priv->assignment);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstDeclarationStatement *jag_ast_declaration_statement_new(JagAstDeclarationType *declaration_type, JagAstAssignment *assignment) {
	JagAstDeclarationStatement *result = g_object_new(JAG_AST_TYPE_DECLARATION_STATEMENT, NULL);
	cat_ref_anounce(result);
	JagAstDeclarationStatementPrivate *priv = result->priv;
	priv->declaration_type = cat_ref_ptr(declaration_type);
	priv->assignment = cat_ref_ptr(assignment);
	priv->statement_line_nr = -1;
	return result;
}




/********************* start JagAstIStatement implementation *********************/

static void l_statement_set_at_least_line_nr(JagAstIStatement *self, int at_least_line_nr) {
	JagAstDeclarationStatementPrivate *priv = JAG_AST_DECLARATION_STATEMENT_GET_PRIVATE(self);
	priv->statement_line_nr = at_least_line_nr;
}

static void l_statement_write_statement(JagAstIStatement *self, JagAstWriter *out) {
	JagAstDeclarationStatementPrivate *priv = JAG_AST_DECLARATION_STATEMENT_GET_PRIVATE(self);
	jag_ast_writer_set_at_least_line_nr(out, priv->statement_line_nr);
	jag_ast_declaration_type_write(priv->declaration_type, out);
	jag_ast_writer_print(out, cat_string_wo_new_with(" "));
	jag_ast_istatement_write_statement((JagAstIStatement *) priv->assignment, out);
}

static void l_statement_iface_init(JagAstIStatementInterface *iface) {
	iface->setAtLeastLineNumber = l_statement_set_at_least_line_nr;
	iface->writeStatement = l_statement_write_statement;
}

/********************* end JagAstIStatement implementation *********************/



/********************* start JagAstIAstNode implementation *********************/

static void l_ast_node_write(JagAstIAstNode *self, JagAstWriter *out) {
	JagAstDeclarationStatementPrivate *priv = JAG_AST_DECLARATION_STATEMENT_GET_PRIVATE(self);
	jag_ast_writer_set_at_least_line_nr(out, priv->statement_line_nr);
	jag_ast_declaration_type_write(priv->declaration_type, out);
	jag_ast_writer_print(out, cat_string_wo_new_with(" "));
	jag_ast_assignment_write(priv->assignment, out);
}


static void l_ast_node_iface_init(JagAstIAstNodeInterface *iface) {
	iface->write = l_ast_node_write;
}

/********************* end JagAstIAstNode implementation *********************/
