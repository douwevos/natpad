/*
   File:    jagbytoploadarray.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 3, 2012
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

#include "jagbytoploadarray.h"
#include "../jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpLoadArray"
#include <logging/catlog.h>

struct _JagBytOpLoadArrayPrivate {
	JagBytType array_content_type;
};

static void l_imnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpLoadArray, jag_byt_op_load_array, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, // @suppress("Unused static function")
		G_ADD_PRIVATE(JagBytOpLoadArray)
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_imnemonic_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_load_array_class_init(JagBytOpLoadArrayClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_load_array_init(JagBytOpLoadArray *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_byt_op_load_array_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_op_load_array_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpLoadArray *jag_byt_op_load_array_new(JagBytOperation operation, int offset, JagBytType array_content_type) {
	JagBytOpLoadArray *result = g_object_new(JAG_BYT_TYPE_OP_LOAD_ARRAY, NULL);
	cat_ref_anounce(result);
	JagBytOpLoadArrayPrivate *priv = jag_byt_op_load_array_get_instance_private(result);
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, operation , offset, 1);
	priv->array_content_type = array_content_type;
	return result;
}


JagBytType jag_byt_op_load_array_get_load_type(JagBytOpLoadArray *op_load_array) {
	JagBytOpLoadArrayPrivate *priv = jag_byt_op_load_array_get_instance_private(op_load_array);
	return priv->array_content_type;
}


/********************* start JagBytIMnemonic implementation *********************/

static CatStringWo *l_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	JagBytOpLoadArray *instance = JAG_BYT_OP_LOAD_ARRAY(self);
	JagBytOpLoadArrayPrivate *priv = jag_byt_op_load_array_get_instance_private(instance);
	CatStringWo *result = cat_string_wo_new();
	switch(priv->array_content_type) {
		case JAG_BYT_TYPE_INT :
			cat_string_wo_append_chars(result, "iaload");
			break;
		case JAG_BYT_TYPE_LONG :
			cat_string_wo_append_chars(result, "laload");
			break;
		case JAG_BYT_TYPE_FLOAT :
			cat_string_wo_append_chars(result, "faload");
			break;
		case JAG_BYT_TYPE_DOUBLE :
			cat_string_wo_append_chars(result, "daload");
			break;
		case JAG_BYT_TYPE_REFERENCE :
			cat_string_wo_append_chars(result, "aaload");
			break;
		case JAG_BYT_TYPE_BYTE :
			cat_string_wo_append_chars(result, "baload");
			break;
		case JAG_BYT_TYPE_CHAR :
			cat_string_wo_append_chars(result, "caload");
			break;
		case JAG_BYT_TYPE_SHORT :
			cat_string_wo_append_chars(result, "saload");
			break;
		default :
			break;
	}
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
