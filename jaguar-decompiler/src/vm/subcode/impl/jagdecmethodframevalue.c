/*
   File:    jagdecmethodframevalue.c
   Project: jaguar-decompiler
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

#include "jagdecmethodframevalue.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecMethodFrameValue"
#include <logging/catlog.h>

struct _JagDecMethodFrameValuePrivate {
	int frame_index;
	JagAstIdentifier *identifier;
};

G_DEFINE_TYPE (JagDecMethodFrameValue, jag_dec_method_frame_value, JAG_DEC_TYPE_VALUE_TYPE_DEFINITION_LIST)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static JagAstIExpression *l_get_expression_ref(JagDecMethodFrameValue *frame_value, JagAstDeclarationType *type_descr);

static void jag_dec_method_frame_value_class_init(JagDecMethodFrameValueClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecMethodFrameValuePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	clazz->getExpressionRef = l_get_expression_ref;
}

static void jag_dec_method_frame_value_init(JagDecMethodFrameValue *instance) {
	JagDecMethodFrameValuePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_METHOD_FRAME_VALUE, JagDecMethodFrameValuePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecMethodFrameValue *instance = JAG_DEC_METHOD_FRAME_VALUE(object);
	JagDecMethodFrameValuePrivate *priv = instance->priv;
	cat_unref_ptr(priv->identifier);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void jag_dec_method_frame_value_construct(JagDecMethodFrameValue *frame_value, int frame_index, JagAstIdentifier *identifier) {
	JagDecMethodFrameValuePrivate *priv = JAG_DEC_METHOD_FRAME_VALUE_GET_PRIVATE(frame_value);
	jag_dec_value_type_definition_list_construct((JagDecValueTypeDefinitionList *) frame_value);
	priv->frame_index = frame_index;
	priv->identifier = cat_ref_ptr(identifier);

}

JagDecMethodFrameValue *jag_dec_method_frame_value_new(int frame_index, JagAstIdentifier *identifier) {
	JagDecMethodFrameValue *result = g_object_new(JAG_DEC_TYPE_METHOD_FRAME_VALUE, NULL);
	cat_ref_anounce(result);
	jag_dec_method_frame_value_construct(result, frame_index, identifier);
	return result;
}

static JagAstIExpression *l_get_expression_ref(JagDecMethodFrameValue *frame_value, JagAstDeclarationType *type_descr) {
	JagDecMethodFrameValuePrivate *priv = JAG_DEC_METHOD_FRAME_VALUE_GET_PRIVATE(frame_value);
	JagAstIdentifier *result = priv->identifier;
	if (result==NULL) {
		char buf[50];
		sprintf(buf, "lfv%d", priv->frame_index);
		result = jag_ast_identifier_new(cat_string_wo_new_with(buf));
	} else {
		cat_ref_ptr(result);
	}
	return (JagAstIExpression *) result;
}


JagAstIExpression *jag_dec_method_frame_value_get_expression_ref(JagDecMethodFrameValue *frame_value, JagAstDeclarationType *type_descr) {
	return JAG_DEC_METHOD_FRAME_VALUE_GET_CLASS(frame_value)->getExpressionRef(frame_value, type_descr);
}

gboolean jag_dec_method_frame_value_set_name(JagDecMethodFrameValue *frame_value, CatStringWo *a_name) {
	JagDecMethodFrameValuePrivate *priv = JAG_DEC_METHOD_FRAME_VALUE_GET_PRIVATE(frame_value);
	gboolean result = TRUE;
	if (priv->identifier!=NULL) {
		result = cat_string_wo_equal(jag_ast_identifier_get_text(priv->identifier), a_name);
	} else {
		priv->identifier = jag_ast_identifier_new(a_name);
	}
	return result;
}

int jag_dec_method_frame_value_hash(JagDecMethodFrameValue *frame_value) {
	JagDecMethodFrameValuePrivate *priv = JAG_DEC_METHOD_FRAME_VALUE_GET_PRIVATE(frame_value);
	return priv->frame_index;
}

int jag_dec_method_frame_value_get_frame_index(JagDecMethodFrameValue *frame_value) {
	return JAG_DEC_METHOD_FRAME_VALUE_GET_PRIVATE(frame_value)->frame_index;
}



