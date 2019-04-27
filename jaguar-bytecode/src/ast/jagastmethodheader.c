/*
   File:    jagastmethodheader.c
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

#include "jagastmethodheader.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstMethodHeader"
#include <logging/catlog.h>

struct _JagAstMethodHeaderPrivate {
	JagAstIdentifier *method_name;
	gboolean is_constructor;
	JagAstDeclarationType *return_type;
	CatArrayWo *e_args;
	CatArrayWo *e_exceptions;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagAstMethodHeader, jag_ast_method_header, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_method_header_class_init(JagAstMethodHeaderClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_method_header_init(JagAstMethodHeader *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstMethodHeader *instance = JAG_AST_METHOD_HEADER(object);
	JagAstMethodHeaderPrivate *priv = jag_ast_method_header_get_instance_private(instance);
	cat_unref_ptr(priv->e_args);
	cat_unref_ptr(priv->e_exceptions);
	cat_unref_ptr(priv->method_name);
	cat_unref_ptr(priv->return_type);
	G_OBJECT_CLASS(jag_ast_method_header_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_method_header_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstMethodHeader *jag_ast_method_header_new(JagAstIdentifier *method_name, gboolean is_constructor, JagAstDeclarationType *return_type, CatArrayWo *e_args, CatArrayWo *e_exceptions) {
	JagAstMethodHeader *result = g_object_new(JAG_AST_TYPE_METHOD_HEADER, NULL);
	cat_ref_anounce(result);
	JagAstMethodHeaderPrivate *priv = jag_ast_method_header_get_instance_private(result);
	priv->method_name = cat_ref_ptr(method_name);
	priv->is_constructor = is_constructor;
	priv->return_type = cat_ref_ptr(return_type);
	priv->e_args = cat_ref_ptr(e_args);
	priv->e_exceptions = cat_ref_ptr(e_exceptions);
	return result;
}


JagAstDeclarationType *jag_ast_method_header_get_return_type(JagAstMethodHeader *method_header) {
	JagAstMethodHeaderPrivate *priv = jag_ast_method_header_get_instance_private(method_header);
	return priv->return_type;
}

void jag_ast_method_header_write(JagAstMethodHeader *method_header, JagAstWriter *out) {
	JagAstMethodHeaderPrivate *priv = jag_ast_method_header_get_instance_private(method_header);
	if (!priv->is_constructor) {
		jag_ast_declaration_type_write(priv->return_type, out);
		jag_ast_writer_print(out, cat_string_wo_new_with(" "));
	}
	jag_ast_iexpression_write((JagAstIExpression *) priv->method_name, out);
	jag_ast_writer_print(out, cat_string_wo_new_with("("));
	gboolean isFirst = TRUE;
	CatIIterator *iter = cat_array_wo_iterator(priv->e_args);
	while(cat_iiterator_has_next(iter)) {
		JagAstFieldDeclaration *arg = (JagAstFieldDeclaration *) cat_iiterator_next(iter);
		if (isFirst) {
			isFirst = FALSE;
		} else {
			jag_ast_writer_print(out, cat_string_wo_new_with(", "));
		}
		jag_ast_field_declaration_write(arg, out);
	}
	cat_unref_ptr(iter);

	jag_ast_writer_print(out, cat_string_wo_new_with(")"));
	if (priv->e_exceptions!=NULL && cat_array_wo_size(priv->e_exceptions)>0) {
		jag_ast_writer_print(out, cat_string_wo_new_with(" throws "));
		isFirst = TRUE;

		CatIIterator *iter = cat_array_wo_iterator(priv->e_exceptions);
		while(cat_iiterator_has_next(iter)) {
			JagBytName *exception_name = (JagBytName *) cat_iiterator_next(iter);
			if (isFirst) {
				isFirst = FALSE;
			} else {
				jag_ast_writer_print(out, cat_string_wo_new_with(", "));
			}
			jag_ast_writer_print(out, jag_byt_name_create_fqn(exception_name));
		}
		cat_unref_ptr(iter);
	}
}
