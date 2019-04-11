/*
   File:    jagbytopvalueconvert.c
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

#include "jagbytopvalueconvert.h"
#include "../jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpValueConvert"
#include <logging/catlog.h>

struct _JagBytOpValueConvertPrivate {
	JagBytType source_type;
	JagBytType destination_type;
};

static void l_imnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpValueConvert, jag_byt_op_value_convert, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_imnemonic_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_value_convert_class_init(JagBytOpValueConvertClass *clazz) {
	g_type_class_add_private(clazz, sizeof(JagBytOpValueConvertPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_value_convert_init(JagBytOpValueConvert *instance) {
	JagBytOpValueConvertPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_OP_VALUE_CONVERT, JagBytOpValueConvertPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_byt_op_value_convert_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_op_value_convert_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpValueConvert *jag_byt_op_value_convert_new(JagBytOperation operation, int offset, JagBytType source_type, JagBytType destination_type) {
	JagBytOpValueConvert *result = g_object_new(JAG_BYT_TYPE_OP_VALUE_CONVERT, NULL);
	cat_ref_anounce(result);
	JagBytOpValueConvertPrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, operation, offset, 1);
	priv->source_type = source_type;
	priv->destination_type = destination_type;
	return result;
}

/********************* start JagBytIMnemonic implementation *********************/

static CatStringWo *l_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	CatStringWo *result = cat_string_wo_new();
	short op_code = jag_byt_imnemonic_get_opp_code(self);
	switch(op_code) {
		case OP_I2L : cat_string_wo_append_chars(result, "i2l"); break;
		case OP_I2F : cat_string_wo_append_chars(result, "i2f"); break;
		case OP_I2D : cat_string_wo_append_chars(result, "i2d"); break;
		case OP_I2S : cat_string_wo_append_chars(result, "i2s"); break;
		case OP_I2B : cat_string_wo_append_chars(result, "i2b"); break;
		case OP_I2C : cat_string_wo_append_chars(result, "i2c"); break;
		case OP_L2I : cat_string_wo_append_chars(result, "l2i"); break;
		case OP_L2F : cat_string_wo_append_chars(result, "l2f"); break;
		case OP_L2D : cat_string_wo_append_chars(result, "l2d"); break;
		case OP_D2I : cat_string_wo_append_chars(result, "d2i"); break;
		case OP_D2L : cat_string_wo_append_chars(result, "d2l"); break;
		case OP_D2F : cat_string_wo_append_chars(result, "d2f"); break;
		case OP_F2I : cat_string_wo_append_chars(result, "f2i"); break;
		case OP_F2L : cat_string_wo_append_chars(result, "f2l"); break;
		case OP_F2D : cat_string_wo_append_chars(result, "f2d"); break;
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
