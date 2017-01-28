/*
   File:    jagasttpheader.c
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

#include "jagasttpheader.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstTpHeader"
#include <logging/catlog.h>

struct _JagAstTpHeaderPrivate {
	JagBytName *classname;
	JagAstModifiers *modifiers;
	JagBytName *supertype;
	CatArrayWo *e_interfaces;
};

G_DEFINE_TYPE (JagAstTpHeader, jag_ast_tp_header, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_tp_header_class_init(JagAstTpHeaderClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstTpHeaderPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_tp_header_init(JagAstTpHeader *instance) {
	JagAstTpHeaderPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_AST_TYPE_TP_HEADER, JagAstTpHeaderPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagAstTpHeader *instance = JAG_AST_TP_HEADER(object);
	JagAstTpHeaderPrivate *priv = instance->priv;
	cat_unref_ptr(priv->classname);
	cat_unref_ptr(priv->e_interfaces);
	cat_unref_ptr(priv->modifiers);
	cat_unref_ptr(priv->supertype);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstTpHeader *jag_ast_tp_header_new(JagBytName *classname, JagAstModifiers *modifiers, JagBytName *supertype, CatArrayWo *e_interfaces) {
	JagAstTpHeader *result = g_object_new(JAG_AST_TYPE_TP_HEADER, NULL);
	cat_ref_anounce(result);
	JagAstTpHeaderPrivate *priv = result->priv;
	priv->classname = cat_ref_ptr(classname);
	priv->modifiers = cat_ref_ptr(modifiers);
	priv->supertype = cat_ref_ptr(supertype);
	priv->e_interfaces = cat_ref_ptr(e_interfaces);
	return result;
}

static CatS l_s_txt_java_lang_object = CAT_S_DEF("java.lang.Object");

void jag_ast_tp_header_write(JagAstTpHeader *tp_header, JagAstWriter *out) {
	JagAstTpHeaderPrivate *priv = JAG_AST_TP_HEADER_GET_PRIVATE(tp_header);
	if (priv->modifiers!=NULL) {
		jag_ast_writer_print(out, jag_ast_modifiers_as_text(priv->modifiers));
	}
	jag_ast_writer_print(out, cat_string_wo_new_with("class "));
	jag_ast_writer_print(out, jag_byt_name_get_short_type_name(priv->classname));
	if (priv->supertype!=NULL) {
		CatStringWo *a_fqn = jag_byt_name_create_fqn(priv->supertype);
		if (!cat_string_wo_equal(CAT_S(l_s_txt_java_lang_object), a_fqn)) {
			jag_ast_writer_print(out, cat_string_wo_new_with(" extends "));
			jag_ast_writer_print(out, a_fqn);
		}
		cat_unref_ptr(a_fqn);
	}
	if (priv->e_interfaces!=NULL && cat_array_wo_size(priv->e_interfaces)>0) {
		jag_ast_writer_print(out, cat_string_wo_new_with(" implements "));
		gboolean isFirst = TRUE;
		CatIIterator *iter = cat_array_wo_iterator(priv->e_interfaces);
		while(cat_iiterator_has_next(iter)) {
			JagBytName *iface = (JagBytName *) cat_iiterator_next(iter);
			if (isFirst) {
				isFirst = FALSE;
			} else {
				jag_ast_writer_print(out, cat_string_wo_new_with(", "));
			}
			jag_ast_writer_print(out, jag_byt_name_create_fqn(iface));
		}
		cat_unref_ptr(iter);
	}
}


