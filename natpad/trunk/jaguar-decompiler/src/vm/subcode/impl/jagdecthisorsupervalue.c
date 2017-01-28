/*
   File:    jagdecthisorsupervalue.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 7, 2012
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

#include "jagdecthisorsupervalue.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecThisOrSuperValue"
#include <logging/catlog.h>

struct _JagDecThisOrSuperValuePrivate {
	JagAstDeclarationType *this_declaration_type;
	JagAstIdentifier *super_identifier;
};

G_DEFINE_TYPE (JagDecThisOrSuperValue, jag_dec_this_or_super_value, JAG_DEC_TYPE_METHOD_FRAME_VALUE)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
JagAstIExpression *l_get_expression_ref(JagDecMethodFrameValue *frame_value, JagAstDeclarationType *type_descr);

static void jag_dec_this_or_super_value_class_init(JagDecThisOrSuperValueClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecThisOrSuperValuePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	JagDecMethodFrameValueClass *sub_clazz = JAG_DEC_METHOD_FRAME_VALUE_CLASS(clazz);
	sub_clazz->getExpressionRef = l_get_expression_ref;
}

static void jag_dec_this_or_super_value_init(JagDecThisOrSuperValue *instance) {
	JagDecThisOrSuperValuePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_THIS_OR_SUPER_VALUE, JagDecThisOrSuperValuePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecThisOrSuperValue *instance = JAG_DEC_THIS_OR_SUPER_VALUE(object);
	JagDecThisOrSuperValuePrivate *priv = instance->priv;
	cat_unref_ptr(priv->this_declaration_type);
	cat_unref_ptr(priv->super_identifier);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecThisOrSuperValue *jag_dec_this_or_super_value_new(int frame_index, JagAstDeclarationType *this_declaration_type) {
	JagDecThisOrSuperValue *result = g_object_new(JAG_DEC_TYPE_THIS_OR_SUPER_VALUE, NULL);
	cat_ref_anounce(result);
	JagDecThisOrSuperValuePrivate *priv = result->priv;
	JagAstIdentifier *this_id = jag_ast_identifier_new(cat_string_wo_new_with("this"));
	jag_dec_method_frame_value_construct((JagDecMethodFrameValue *) result, frame_index, this_id);
	cat_unref_ptr(this_id);
	priv->this_declaration_type = cat_ref_ptr(this_declaration_type);
	priv->super_identifier = jag_ast_identifier_new(cat_string_wo_new_with("super"));
	return result;
}




JagAstIExpression *l_get_expression_ref(JagDecMethodFrameValue *frame_value, JagAstDeclarationType *type_descr) {
	JagDecThisOrSuperValuePrivate *priv = JAG_DEC_THIS_OR_SUPER_VALUE_GET_PRIVATE(frame_value);
	if (type_descr!=NULL && jag_ast_declaration_type_is_reference(type_descr)) {
		if (!jag_ast_declaration_type_equal(type_descr, priv->this_declaration_type)) {
			return cat_ref_ptr(priv->super_identifier);
		}
	}
	return JAG_DEC_METHOD_FRAME_VALUE_CLASS(parent_class)->getExpressionRef(frame_value, type_descr);
}


