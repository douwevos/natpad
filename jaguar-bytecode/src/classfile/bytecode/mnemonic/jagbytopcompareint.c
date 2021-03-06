/*
   File:    jagbytopcompareint.c
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

#include "jagbytopcompareint.h"
#include "../jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpCompareInt"
#include <logging/catlog.h>

struct _JagBytOpCompareIntPrivate {
	int branch_offset;
	JagBytConditionType condition_type;
	gboolean with_zero;
};

static void l_mnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpCompareInt, jag_byt_op_compare_int, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, { // @suppress("Unused static function")
		G_ADD_PRIVATE(JagBytOpCompareInt)
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_mnemonic_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_compare_int_class_init(JagBytOpCompareIntClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_compare_int_init(JagBytOpCompareInt *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_byt_op_compare_int_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_op_compare_int_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpCompareInt *jag_byt_op_compare_int_new(JagBytOperation operation, int offset, int branch_offset, JagBytConditionType condition_type, gboolean with_zero) {
	JagBytOpCompareInt *result = g_object_new(JAG_BYT_TYPE_OP_COMPARE_INT, NULL);
	cat_ref_anounce(result);
	JagBytOpCompareIntPrivate *priv = jag_byt_op_compare_int_get_instance_private(result);
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, operation, offset, 3);
	priv->branch_offset = branch_offset;
	priv->condition_type = condition_type;
	priv->with_zero = with_zero;
	return result;
}

JagBytConditionType jag_byt_op_compare_int_get_condition_type(JagBytOpCompareInt *compare_int) {
	JagBytOpCompareIntPrivate *priv = jag_byt_op_compare_int_get_instance_private(compare_int);
	return priv->condition_type;
}

gboolean jag_byt_op_compare_int_is_with_zero(JagBytOpCompareInt *compare_int) {
	JagBytOpCompareIntPrivate *priv = jag_byt_op_compare_int_get_instance_private(compare_int);
	return priv->with_zero;
}


/********************* start JagBytIMnemonicInterface implementation *********************/

static int l_mnemonic_get_branch_offset(JagBytIMnemonic *self) {
	JagBytOpCompareInt *instance = JAG_BYT_OP_COMPARE_INT(self);
	JagBytOpCompareIntPrivate *priv = jag_byt_op_compare_int_get_instance_private(instance);
	int offset = jag_byt_imnemonic_get_offset(self);
	return offset+priv->branch_offset;
}

static CatStringWo *l_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	JagBytOpCompareInt *instance = JAG_BYT_OP_COMPARE_INT(self);
	JagBytOpCompareIntPrivate *priv = jag_byt_op_compare_int_get_instance_private(instance);
	CatStringWo *result = cat_string_wo_new();
	if (priv->with_zero) {
		switch(priv->condition_type) {
			case JAG_BYT_CONDITION_TYPE_EQUAL :
				cat_string_wo_append_chars(result, "ifeq ");
				break;
			case JAG_BYT_CONDITION_TYPE_NOT_EQUAL :
				cat_string_wo_append_chars(result, "ifne ");
				break;
			case JAG_BYT_CONDITION_TYPE_LESS_THEN :
				cat_string_wo_append_chars(result, "iflt ");
				break;
			case JAG_BYT_CONDITION_TYPE_LESS_OR_EQUAL :
				cat_string_wo_append_chars(result, "ifle ");
				break;
			case JAG_BYT_CONDITION_TYPE_GREATER_THEN :
				cat_string_wo_append_chars(result, "ifgt ");
				break;
			case JAG_BYT_CONDITION_TYPE_GREATER_OR_EQUAL :
				cat_string_wo_append_chars(result, "ifge ");
				break;
			default :
				break;
		}
	} else {
		switch(priv->condition_type) {
			case JAG_BYT_CONDITION_TYPE_EQUAL :
				cat_string_wo_append_chars(result, "if_icmpeq ");
				break;
			case JAG_BYT_CONDITION_TYPE_NOT_EQUAL :
				cat_string_wo_append_chars(result, "if_icmpne ");
				break;
			case JAG_BYT_CONDITION_TYPE_LESS_THEN :
				cat_string_wo_append_chars(result, "if_icmplt ");
				break;
			case JAG_BYT_CONDITION_TYPE_LESS_OR_EQUAL :
				cat_string_wo_append_chars(result, "if_icmple ");
				break;
			case JAG_BYT_CONDITION_TYPE_GREATER_THEN :
				cat_string_wo_append_chars(result, "if_icmpgt ");
				break;
			case JAG_BYT_CONDITION_TYPE_GREATER_OR_EQUAL :
				cat_string_wo_append_chars(result, "if_icmpge ");
				break;
			default :
				break;
		}
	}
	cat_string_wo_append_decimal(result, priv->branch_offset);
	return result;
}


static void l_mnemonic_iface_init(JagBytIMnemonicInterface *iface) {
	JagBytIMnemonicInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->getOperation = p_iface->getOperation;
	iface->getOppCode = p_iface->getOppCode;
	iface->getOffset = p_iface->getOffset;
	iface->getContinuesOffset = p_iface->getContinuesOffset;
	iface->getBranchOffset = l_mnemonic_get_branch_offset;
	iface->getLength = p_iface->getLength;
	iface->toString = l_to_string;
}

/********************* end JagBytIMnemonicInterface implementation *********************/
