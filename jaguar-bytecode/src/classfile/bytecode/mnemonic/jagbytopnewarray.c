/*
   File:    jagbytopnewarray.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 4, 2012
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

#include "jagbytopnewarray.h"
#include "../jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpNewArray"
#include <logging/catlog.h>

struct _JagBytOpNewArrayPrivate {
	JagBytType array_type;
	int class_pool_index;
};

static void l_imnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpNewArray, jag_byt_op_new_array, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, // @suppress("Unused static function")
		G_ADD_PRIVATE(JagBytOpNewArray)
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_imnemonic_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_new_array_class_init(JagBytOpNewArrayClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_new_array_init(JagBytOpNewArray *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_byt_op_new_array_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_op_new_array_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagBytOpNewArray *jag_byt_op_new_array_new_primitive(int offset, JagBytType array_type) {
	JagBytOpNewArray *result = g_object_new(JAG_BYT_TYPE_OP_NEW_ARRAY, NULL);
	cat_ref_anounce(result);
	JagBytOpNewArrayPrivate *priv = jag_byt_op_new_array_get_instance_private(result);
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, OP_NEWARRAY, offset, 2);
	priv->array_type = array_type;
	priv->class_pool_index = 0;
	return result;
}

JagBytOpNewArray *jag_byt_op_new_array_new_reference(int offset, int class_pool_index) {
	JagBytOpNewArray *result = g_object_new(JAG_BYT_TYPE_OP_NEW_ARRAY, NULL);
	cat_ref_anounce(result);
	JagBytOpNewArrayPrivate *priv = jag_byt_op_new_array_get_instance_private(result);
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, OP_ANEWARRAY, offset, 3);
	priv->array_type = JAG_BYT_TYPE_REFERENCE;
	priv->class_pool_index = class_pool_index;
	return result;
}

JagBytType jag_byt_op_new_array_get_primitive_type(JagBytOpNewArray *op_new_array) {
	JagBytOpNewArrayPrivate *priv = jag_byt_op_new_array_get_instance_private(op_new_array);
	return priv->array_type;
}

int jag_byt_op_new_array_get_class_info_pool_index(JagBytOpNewArray *op_new_array) {
	JagBytOpNewArrayPrivate *priv = jag_byt_op_new_array_get_instance_private(op_new_array);
	return priv->class_pool_index;
}

/********************* start JagBytIMnemonic implementation *********************/

static CatStringWo *l_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	JagBytOpNewArray *instance = JAG_BYT_OP_NEW_ARRAY(self);
	JagBytOpNewArrayPrivate *priv = jag_byt_op_new_array_get_instance_private(instance);

	CatStringWo *result = cat_string_wo_new();
	if (priv->array_type == JAG_BYT_TYPE_REFERENCE) {
		cat_string_wo_append_chars(result, "anewarray ");
		cat_string_wo_append_decimal(result, priv->class_pool_index);
	} else {
		cat_string_wo_append_chars(result, "newarray ");
		switch(priv->array_type) {
			case JAG_BYT_TYPE_BOOLEAN : cat_string_wo_append_chars(result, "boolean"); break;
			case JAG_BYT_TYPE_BYTE : cat_string_wo_append_chars(result, "byte"); break;
			case JAG_BYT_TYPE_CHAR : cat_string_wo_append_chars(result, "char"); break;
			case JAG_BYT_TYPE_DOUBLE : cat_string_wo_append_chars(result, "double"); break;
			case JAG_BYT_TYPE_FLOAT : cat_string_wo_append_chars(result, "float"); break;
			case JAG_BYT_TYPE_INT : cat_string_wo_append_chars(result, "int"); break;
			case JAG_BYT_TYPE_LONG : cat_string_wo_append_chars(result, "long"); break;
			case JAG_BYT_TYPE_SHORT : cat_string_wo_append_chars(result, "short"); break;
			case JAG_BYT_TYPE_VOID : cat_string_wo_append_chars(result, "void"); break;
		}
	}
	cat_string_wo_append_decimal(result, priv->class_pool_index);
	return result;
}

static void l_imnemonic_iface_init(JagBytIMnemonicInterface *iface) {
	JagBytIMnemonicInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->getBranchOffset = p_iface->getBranchOffset;
	iface->getContinuesOffset = p_iface->getContinuesOffset;
	iface->getLength = p_iface->getLength;
	iface->getOffset = p_iface->getOffset;
	iface->getOperation = p_iface->getOperation;
	iface->getOppCode = p_iface->getOppCode;
	iface->toString = l_to_string;
}

/********************* end JagBytIMnemonic implementation *********************/
