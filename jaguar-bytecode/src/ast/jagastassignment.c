/*
   File:    jagastassignment.c
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

#include "jagastassignment.h"
#include "jagastistatement.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstAssignment"
#include <logging/catlog.h>

struct _JagAstAssignmentPrivate {
	JagAstIExpression *lvalue;
	JagAstIExpression *rvalue;
	int statement_line_nr;
};

static void l_statement_iface_init(JagAstIStatementInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstAssignment, jag_ast_assignment, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstAssignment)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ISTATEMENT, l_statement_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_assignment_class_init(JagAstAssignmentClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_assignment_init(JagAstAssignment *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstAssignment *instance = JAG_AST_ASSIGNMENT(object);
	JagAstAssignmentPrivate *priv = jag_ast_assignment_get_instance_private(instance);
	cat_unref_ptr(priv->lvalue);
	cat_unref_ptr(priv->rvalue);
	G_OBJECT_CLASS(jag_ast_assignment_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_assignment_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstAssignment *jag_ast_assignment_new(JagAstIExpression *lvalue, JagAstIExpression *rvalue) {
	JagAstAssignment *result = g_object_new(JAG_AST_TYPE_ASSIGNMENT, NULL);
	cat_ref_anounce(result);
	JagAstAssignmentPrivate *priv = jag_ast_assignment_get_instance_private(result);
	priv->lvalue = cat_ref_ptr(lvalue);
	priv->rvalue = cat_ref_ptr(rvalue);
	priv->statement_line_nr = -1;
	return result;
}


void jag_ast_assignment_write(JagAstAssignment *assignment, JagAstWriter *out) {
	JagAstAssignmentPrivate *priv = jag_ast_assignment_get_instance_private(assignment);
	jag_ast_writer_set_at_least_line_nr(out, priv->statement_line_nr);
	cat_log_trace("rvalue=%p", priv->rvalue);
	if (priv->lvalue) {
		jag_ast_iexpression_write(priv->lvalue, out);
	} else {
		jag_ast_writer_print(out, cat_string_wo_new_with("<missing left-value>"));
	}
	jag_ast_writer_print(out, cat_string_wo_new_with(" = "));
	if (priv->rvalue) {
		jag_ast_iexpression_write(priv->rvalue, out);
	} else {
		jag_ast_writer_print(out, cat_string_wo_new_with("<missing right-value>"));
	}
}



/********************* start JagAstIStatement implementation *********************/


static void l_statement_set_at_least_line_nr(JagAstIStatement *self, int at_least_line_nr) {
	JagAstAssignment *instance = JAG_AST_ASSIGNMENT(self);
	JagAstAssignmentPrivate *priv = jag_ast_assignment_get_instance_private(instance);
	priv->statement_line_nr = at_least_line_nr;
}


static void l_statement_write_statement(JagAstIStatement *self, JagAstWriter *out) {
	JagAstAssignment *instance = JAG_AST_ASSIGNMENT(self);
	JagAstAssignmentPrivate *priv = jag_ast_assignment_get_instance_private(instance);
	jag_ast_writer_set_at_least_line_nr(out, priv->statement_line_nr);
	cat_log_trace("rvalue=%p", priv->rvalue);
	if (priv->lvalue) {
		jag_ast_iexpression_write(priv->lvalue, out);
	} else {
		jag_ast_writer_print(out, cat_string_wo_new_with("<missing left-value>"));
	}
	jag_ast_writer_print(out, cat_string_wo_new_with(" = "));
	if (priv->rvalue) {
		jag_ast_iexpression_write(priv->rvalue, out);
	} else {
		jag_ast_writer_print(out, cat_string_wo_new_with("<missing right-value>"));
	}
	jag_ast_writer_print(out, cat_string_wo_new_with(";\n"));
}

static void l_statement_iface_init(JagAstIStatementInterface *iface) {
	iface->setAtLeastLineNumber = l_statement_set_at_least_line_nr;
	iface->writeStatement = l_statement_write_statement;
}

/********************* end JagAstIStatement implementation *********************/
