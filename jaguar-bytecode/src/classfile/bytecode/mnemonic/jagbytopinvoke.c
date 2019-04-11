/*
   File:    jagbytopinvoke.c
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

#include "jagbytopinvoke.h"
#include "../jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpInvoke"
#include <logging/catlog.h>

struct _JagBytOpInvokePrivate {
	int methodref_pool_index;
	int count;
};

static void l_imnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpInvoke, jag_byt_op_invoke, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_imnemonic_iface_init)
);

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_invoke_class_init(JagBytOpInvokeClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytOpInvokePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_invoke_init(JagBytOpInvoke *instance) {
	JagBytOpInvokePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_OP_INVOKE, JagBytOpInvokePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpInvoke *jag_byt_op_invoke_new(JagBytOperation operation, int offset, int methodref_pool_index) {
	JagBytOpInvoke *result = g_object_new(JAG_BYT_TYPE_OP_INVOKE, NULL);
	cat_ref_anounce(result);
	JagBytOpInvokePrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, operation, offset, operation==OP_INVOKEDYNAMIC ? 5 : 3);
	priv->methodref_pool_index = methodref_pool_index;
	priv->count = 0;
	return result;
}


JagBytOpInvoke *jag_byt_op_invoke_new_interface(int offset, int methodref_pool_index, int count) {
	JagBytOpInvoke *result = g_object_new(JAG_BYT_TYPE_OP_INVOKE, NULL);
	cat_ref_anounce(result);
	JagBytOpInvokePrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, OP_INVOKEINTERFACE, offset, 5);
	priv->methodref_pool_index = methodref_pool_index;
	priv->count = count;
	return result;

}

int jag_byt_op_invoke_get_methodref_pool_index(JagBytOpInvoke *op_invoke) {
	return JAG_BYT_OP_INVOKE_GET_PRIVATE(op_invoke)->methodref_pool_index;
}

int jag_byt_op_invoke_get_count(JagBytOpInvoke *op_invoke) {
	return JAG_BYT_OP_INVOKE_GET_PRIVATE(op_invoke)->count;
}

/********************* start JagBytIMnemonic implementation *********************/

static CatStringWo *l_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	JagBytOpInvoke *instance = (JagBytOpInvoke *) self;
	JagBytOpInvokePrivate *priv = instance->priv;
	CatStringWo *result = cat_string_wo_new();
	short opp_code = jag_byt_imnemonic_get_opp_code(self);
	switch(opp_code) {
		case OP_INVOKEVIRTUAL :
			cat_string_wo_append_chars(result, "invokevirtual ");
			break;
		case OP_INVOKESPECIAL :
			cat_string_wo_append_chars(result, "invokespecial ");
			break;
		case OP_INVOKESTATIC :
			cat_string_wo_append_chars(result, "invokestatic ");
			break;
		case OP_INVOKEDYNAMIC :
			cat_string_wo_append_chars(result, "invokedynamic ");
			break;
		default :
			break;
	}
	cat_string_wo_append_decimal(result, priv->methodref_pool_index);
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
