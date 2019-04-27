/*
   File:    jagastimportstatement.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 1, 2012
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

#include "jagastimportstatement.h"
#include "jagastistatement.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstImportStatement"
#include <logging/catlog.h>

struct _JagAstImportStatementPrivate {
	JagBytName *name;
	int statement_line_nr;
};

static void l_statement_iface_init(JagAstIStatementInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagAstImportStatement, jag_ast_import_statement, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagAstImportStatement)
		G_IMPLEMENT_INTERFACE(JAG_AST_TYPE_ISTATEMENT, l_statement_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_import_statement_class_init(JagAstImportStatementClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_import_statement_init(JagAstImportStatement *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstImportStatement *instance = JAG_AST_IMPORT_STATEMENT(object);
	JagAstImportStatementPrivate *priv = jag_ast_import_statement_get_instance_private(instance);
	cat_unref_ptr(priv->name);
	G_OBJECT_CLASS(jag_ast_import_statement_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_import_statement_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstImportStatement *jag_ast_import_statement_new(JagBytName *name) {
	JagAstImportStatement *result = g_object_new(JAG_AST_TYPE_IMPORT_STATEMENT, NULL);
	cat_ref_anounce(result);
	JagAstImportStatementPrivate *priv = jag_ast_import_statement_get_instance_private(result);
	priv->name = cat_ref_sink_ptr(name);
	priv->statement_line_nr = -1;
	return result;
}

JagBytName *jag_ast_import_statement_get_name(JagAstImportStatement *import_statement) {
	JagAstImportStatementPrivate *priv = jag_ast_import_statement_get_instance_private(import_statement);
	return priv->name;
}

static CatS l_s_txt_import = CAT_S_DEF("import ");
static CatS l_s_txt_semi_newline = CAT_S_DEF(";\n");

void jag_ast_import_statement_write(JagAstImportStatement *import_statement, JagAstWriter *out) {
	JagAstImportStatementPrivate *priv = jag_ast_import_statement_get_instance_private(import_statement);
	jag_ast_writer_set_at_least_line_nr(out, priv->statement_line_nr);
	jag_ast_writer_print(out, CAT_S(l_s_txt_import));
	jag_ast_writer_print(out, jag_byt_name_create_fqn(priv->name));
	jag_ast_writer_print(out, CAT_S(l_s_txt_semi_newline));
}


/********************* start JagAstIStatement implementation *********************/

static void l_statement_set_at_least_line_nr(JagAstIStatement *self, int at_least_line_nr) {
	JagAstImportStatement *instance = JAG_AST_IMPORT_STATEMENT(self);
	JagAstImportStatementPrivate *priv = jag_ast_import_statement_get_instance_private(instance);
	priv->statement_line_nr = at_least_line_nr;
}

static void l_statement_write_statement(JagAstIStatement *self, JagAstWriter *out) {
//	JagAstImportStatementPrivate *priv = JAG_AST_IMPORT_STATEMENT_GET_PRIVATE(self);
//	jag_ast_writer_set_at_least_line_nr(out, priv->statement_line_nr);
	jag_ast_import_statement_write((JagAstImportStatement *) self, out);
//	jag_ast_writer_print(out, jag_byt_name_create_fqn(priv->name));
}

static void l_statement_iface_init(JagAstIStatementInterface *iface) {
	iface->setAtLeastLineNumber = l_statement_set_at_least_line_nr;
	iface->writeStatement = l_statement_write_statement;
}

/********************* end JagAstIStatement implementation *********************/
