/*
   File:    jagbytoploadconstant.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 2, 2012
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

#include "jagbytoploadconstant.h"
#include "../jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpLoadConstant"
#include <logging/catlog.h>

struct _JagBytOpLoadConstantPrivate {
	JagBytType constant_type;
	CatIStringable *constant_value;
};

static void l_imnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpLoadConstant, jag_byt_op_load_constant, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_imnemonic_iface_init)
);

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_load_constant_class_init(JagBytOpLoadConstantClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytOpLoadConstantPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_load_constant_init(JagBytOpLoadConstant *instance) {
	JagBytOpLoadConstantPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_OP_LOAD_CONSTANT, JagBytOpLoadConstantPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytOpLoadConstant *instance = JAG_BYT_OP_LOAD_CONSTANT(object);
	JagBytOpLoadConstantPrivate *priv = instance->priv;
	cat_unref_ptr(priv->constant_value);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpLoadConstant *jag_byt_op_load_constant_new(JagBytOperation operation, JagBytType constant_type, CatIStringable *constant_value, int offset) {
	JagBytOpLoadConstant *result = g_object_new(JAG_BYT_TYPE_OP_LOAD_CONSTANT, NULL);
	cat_ref_anounce(result);
	JagBytOpLoadConstantPrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, operation, offset, 1);
	priv->constant_type = constant_type;
	priv->constant_value = constant_value;
	return result;
}



JagBytType jag_byt_op_load_constant_get_constant_type(JagBytOpLoadConstant *load_constant) {
	return JAG_BYT_OP_LOAD_CONSTANT_GET_PRIVATE(load_constant)->constant_type;
}

CatIStringable *jag_byt_op_load_constant_get_contant_value(JagBytOpLoadConstant *load_constant) {
	return JAG_BYT_OP_LOAD_CONSTANT_GET_PRIVATE(load_constant)->constant_value;
}


/********************* start JagBytIMnemonic implementation *********************/

static CatStringWo *l_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	CatStringWo *result = cat_string_wo_new();
	short op_code = jag_byt_imnemonic_get_opp_code(self);
	switch(op_code) {
		case OP_ICONST_M1 : cat_string_wo_append_chars(result, "iconst_m1"); break;
		case OP_ICONST_0 : cat_string_wo_append_chars(result, "iconst_0"); break;
		case OP_ICONST_1 : cat_string_wo_append_chars(result, "iconst_1"); break;
		case OP_ICONST_2 : cat_string_wo_append_chars(result, "iconst_2"); break;
		case OP_ICONST_3 : cat_string_wo_append_chars(result, "iconst_3"); break;
		case OP_ICONST_4 : cat_string_wo_append_chars(result, "iconst_4"); break;
		case OP_ICONST_5 : cat_string_wo_append_chars(result, "iconst_5"); break;
		case OP_LCONST_0 : cat_string_wo_append_chars(result, "lconst_0"); break;
		case OP_LCONST_1 : cat_string_wo_append_chars(result, "lconst_1"); break;
		case OP_FCONST_0 : cat_string_wo_append_chars(result, "fconst_0"); break;
		case OP_FCONST_1 : cat_string_wo_append_chars(result, "fconst_1"); break;
		case OP_FCONST_2 : cat_string_wo_append_chars(result, "fconst_2"); break;
		case OP_DCONST_0 : cat_string_wo_append_chars(result, "dconst_0"); break;
		case OP_DCONST_1 : cat_string_wo_append_chars(result, "dconst_1"); break;
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
