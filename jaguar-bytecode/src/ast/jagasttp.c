/*
   File:    jagasttp.c
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

#include "jagasttp.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstTp"
#include <logging/catlog.h>

struct _JagAstTpPrivate {
	JagAstPackageStatement *package_statement;
	CatArrayWo *e_imports;
	JagAstTpHeader *type_header;
	CatArrayWo *e_fields;
	CatArrayWo *e_methods;
};

G_DEFINE_TYPE (JagAstTp, jag_ast_tp, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_tp_class_init(JagAstTpClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstTpPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_tp_init(JagAstTp *instance) {
	JagAstTpPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_TP, JagAstTpPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstTp *instance = JAG_AST_TP(object);
	JagAstTpPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_fields);
	cat_unref_ptr(priv->e_imports);
	cat_unref_ptr(priv->e_methods);
	cat_unref_ptr(priv->package_statement);
	cat_unref_ptr(priv->type_header);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstTp *jag_ast_tp_new(JagAstPackageStatement *package_statement, CatArrayWo *e_imports, JagAstTpHeader *type_header, CatArrayWo *e_fields, CatArrayWo *e_methods) {
	JagAstTp *result = g_object_new(JAG_AST_TYPE_TP, NULL);
	cat_ref_anounce(result);
	JagAstTpPrivate *priv = result->priv;
	priv->package_statement = cat_ref_ptr(package_statement);
	priv->e_imports = cat_ref_ptr(e_imports);
	priv->type_header = cat_ref_ptr(type_header);
	priv->e_fields = cat_ref_ptr(e_fields);
	priv->e_methods = cat_ref_ptr(e_methods);
	return result;
}

JagAstFieldDeclaration *jag_ast_tp_find_field_by_name(JagAstTp *tp, CatStringWo *a_field_name) {
	JagAstTpPrivate *priv = JAG_AST_TP_GET_PRIVATE(tp);
	JagAstFieldDeclaration *result = NULL;
	CatIIterator *iter = cat_array_wo_iterator(priv->e_fields);
	while(cat_iiterator_has_next(iter)) {
		JagAstFieldDeclaration *field = (JagAstFieldDeclaration *) cat_iiterator_next(iter);
		JagAstIdentifier *identifier = jag_ast_field_declaration_get_variable(field);
		if (cat_string_wo_equal(jag_ast_identifier_get_text(identifier), a_field_name)) {
			result = field;
			break;
		}
	}
	cat_unref_ptr(iter);
	return result;
}

void jag_ast_tp_write(JagAstTp *tp, JagAstWriter *out) {
	JagAstTpPrivate *priv = JAG_AST_TP_GET_PRIVATE(tp);
	if (priv->package_statement!=NULL) {
		jag_ast_istatement_write_statement((JagAstIStatement *) priv->package_statement, out);
		jag_ast_writer_print(out, cat_string_wo_new_with("\n"));
	}

	if (priv->e_imports!=NULL && cat_array_wo_size(priv->e_imports)>0) {
		CatIIterator *iter = cat_array_wo_iterator(priv->e_imports);
		while(cat_iiterator_has_next(iter)) {
			JagAstIStatement *import_statement = (JagAstIStatement *) cat_iiterator_next(iter);
			jag_ast_istatement_write_statement(import_statement, out);
		}
		cat_unref_ptr(iter);
		jag_ast_writer_print(out, cat_string_wo_new_with("\n"));
	}

	jag_ast_tp_header_write(priv->type_header, out);
	jag_ast_writer_print(out, cat_string_wo_new_with(" {\n\n"));
	jag_ast_writer_increase_indent(out);
	if (priv->e_fields!=NULL && cat_array_wo_size(priv->e_fields)>0) {
		CatIIterator *iter = cat_array_wo_iterator(priv->e_fields);
		while(cat_iiterator_has_next(iter)) {
			JagAstFieldDeclaration *field_declaration = (JagAstFieldDeclaration *) cat_iiterator_next(iter);
			jag_ast_field_declaration_write(field_declaration, out);
			jag_ast_writer_print(out, cat_string_wo_new_with(";\n"));
		}
		cat_unref_ptr(iter);

		jag_ast_writer_print(out, cat_string_wo_new_with("\n"));
	}

	if (cat_array_wo_size(priv->e_methods)>0) {
		CatIIterator *iter = cat_array_wo_iterator(priv->e_methods);
		while(cat_iiterator_has_next(iter)) {
			JagAstMethod *method = (JagAstMethod *) cat_iiterator_next(iter);
			jag_ast_istatement_write_statement((JagAstIStatement *) method, out);
			jag_ast_writer_print(out, cat_string_wo_new_with("\n\n"));
		}
		cat_unref_ptr(iter);
	}
	jag_ast_writer_decrease_indent(out);
	jag_ast_writer_print(out, cat_string_wo_new_with("}\n"));
}


