/*
   File:    jagastpackagestatement.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Oct 28, 2012
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

#include "jagastpackagestatement.h"
#include "jagastistatement.h"
#include "jagastiexpression.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstPackageStatement"
#include <logging/catlog.h>

struct _JagAstPackageStatementPrivate {
	JagAstIdentifier *package_name;
	int statement_line_nr;
};

static void l_statement_iface_init(JagAstIStatementInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstPackageStatement, jag_ast_package_statement, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstPackageStatement)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ISTATEMENT, l_statement_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_package_statement_class_init(JagAstPackageStatementClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_package_statement_init(JagAstPackageStatement *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstPackageStatement *instance = JAG_AST_PACKAGE_STATEMENT(object);
	JagAstPackageStatementPrivate *priv = jag_ast_package_statement_get_instance_private(instance);
	cat_unref_ptr(priv->package_name);
	G_OBJECT_CLASS(jag_ast_package_statement_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_package_statement_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstPackageStatement *jag_ast_package_statement_new(JagAstIdentifier *package_name) {
	JagAstPackageStatement *result = g_object_new(JAG_AST_TYPE_PACKAGE_STATEMENT, NULL);
	cat_ref_anounce(result);
	JagAstPackageStatementPrivate *priv = jag_ast_package_statement_get_instance_private(result);
	priv->package_name = cat_ref_ptr(package_name);
	priv->statement_line_nr = -1;
	return result;
}


/********************* start JagAstIStatement implementation *********************/

static void l_statement_set_at_least_line_nr(JagAstIStatement *self, int at_least_line_nr) {
	JagAstPackageStatement *instance = JAG_AST_PACKAGE_STATEMENT(self);
	JagAstPackageStatementPrivate *priv = jag_ast_package_statement_get_instance_private(instance);
	priv->statement_line_nr = at_least_line_nr;
}


static CatS l_s_txt_package = CAT_S_DEF("package ");
static CatS l_s_txt_semi_newline = CAT_S_DEF(";\n");

static void l_statement_write_statement(JagAstIStatement *self, JagAstWriter *out) {
	JagAstPackageStatement *instance = JAG_AST_PACKAGE_STATEMENT(self);
	JagAstPackageStatementPrivate *priv = jag_ast_package_statement_get_instance_private(instance);
	jag_ast_writer_set_at_least_line_nr(out, priv->statement_line_nr);
	jag_ast_writer_print(out, CAT_S(l_s_txt_package));
	jag_ast_iexpression_write((JagAstIExpression *) priv->package_name, out);
	jag_ast_writer_print(out, CAT_S(l_s_txt_semi_newline));
}

static void l_statement_iface_init(JagAstIStatementInterface *iface) {
	iface->setAtLeastLineNumber = l_statement_set_at_least_line_nr;
	iface->writeStatement = l_statement_write_statement;
}

/********************* end JagAstIStatement implementation *********************/
