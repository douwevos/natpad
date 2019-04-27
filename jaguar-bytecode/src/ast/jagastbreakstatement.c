/*
   File:    jagastbreakstatement.c
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

#include "jagastbreakstatement.h"
#include "jagastistatement.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstBreakStatement"
#include <logging/catlog.h>

struct _JagAstBreakStatementPrivate {
	int statement_line_nr;
};

static void l_statement_iface_init(JagAstIStatementInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstBreakStatement, jag_ast_break_statement, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstBreakStatement)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ISTATEMENT, l_statement_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_break_statement_class_init(JagAstBreakStatementClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_break_statement_init(JagAstBreakStatement *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_ast_break_statement_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_break_statement_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstBreakStatement *jag_ast_break_statement_new() {
	JagAstBreakStatement *result = g_object_new(JAG_AST_TYPE_BREAK_STATEMENT, NULL);
	cat_ref_anounce(result);
	JagAstBreakStatementPrivate *priv = jag_ast_break_statement_get_instance_private(result);
	priv->statement_line_nr = -1;
	return result;
}


/********************* start JagAstIStatement implementation *********************/

static void l_statement_set_at_least_line_nr(JagAstIStatement *self, int at_least_line_nr) {
	JagAstBreakStatement *instance = JAG_AST_BREAK_STATEMENT(self);
	JagAstBreakStatementPrivate *priv = jag_ast_break_statement_get_instance_private(instance);
	priv->statement_line_nr = at_least_line_nr;
}

static void l_statement_write_statement(JagAstIStatement *self, JagAstWriter *out) {
	JagAstBreakStatement *instance = JAG_AST_BREAK_STATEMENT(self);
	JagAstBreakStatementPrivate *priv = jag_ast_break_statement_get_instance_private(instance);
	jag_ast_writer_set_at_least_line_nr(out, priv->statement_line_nr);
	jag_ast_writer_print(out, cat_string_wo_new_with("break;\n"));
}

static void l_statement_iface_init(JagAstIStatementInterface *iface) {
	iface->setAtLeastLineNumber = l_statement_set_at_least_line_nr;
	iface->writeStatement = l_statement_write_statement;
}

/********************* end JagAstIStatement implementation *********************/
