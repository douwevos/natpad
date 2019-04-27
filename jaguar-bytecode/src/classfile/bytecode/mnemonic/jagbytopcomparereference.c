/*
   File:    jagbytopcomparereference.c
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

#include "jagbytopcomparereference.h"
#include "../jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpCompareReference"
#include <logging/catlog.h>

struct _JagBytOpCompareReferencePrivate {
	int branch_offset;
	JagBytConditionType condition_type;
	gboolean with_zero;
};

static void l_mnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpCompareReference, jag_byt_op_compare_reference, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, { // @suppress("Unused static function")
		G_ADD_PRIVATE(JagBytOpCompareReference)
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_mnemonic_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_compare_reference_class_init(JagBytOpCompareReferenceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_compare_reference_init(JagBytOpCompareReference *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_byt_op_compare_reference_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_op_compare_reference_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpCompareReference *jag_byt_op_compare_reference_new(JagBytOperation operation, int offset, int branch_offset, JagBytConditionType condition_type, gboolean with_zero) {
	JagBytOpCompareReference *result = g_object_new(JAG_BYT_TYPE_OP_COMPARE_REFERENCE, NULL);
	cat_ref_anounce(result);
	JagBytOpCompareReferencePrivate *priv = jag_byt_op_compare_reference_get_instance_private(result);
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, operation, offset, 3);
	priv->branch_offset = branch_offset;
	priv->condition_type = condition_type;
	priv->with_zero = with_zero;
	return result;
}

JagBytConditionType jag_byt_op_compare_reference_get_condition_type(JagBytOpCompareReference *compare_reference) {
	JagBytOpCompareReferencePrivate *priv = jag_byt_op_compare_reference_get_instance_private(compare_reference);
	return priv->condition_type;
}

gboolean jag_byt_op_compare_reference_is_with_zero(JagBytOpCompareReference *compare_reference) {
	JagBytOpCompareReferencePrivate *priv = jag_byt_op_compare_reference_get_instance_private(compare_reference);
	return priv->with_zero;
}


/********************* start JagBytIMnemonicInterface implementation *********************/

static int l_mnemonic_get_branch_offset(JagBytIMnemonic *self) {
	JagBytOpCompareReference *instance = JAG_BYT_OP_COMPARE_REFERENCE(self);
	JagBytOpCompareReferencePrivate *priv = jag_byt_op_compare_reference_get_instance_private(instance);
	int offset = jag_byt_imnemonic_get_offset(self);
	return offset+priv->branch_offset;
}

static CatStringWo *l_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	JagBytOpCompareReference *instance = JAG_BYT_OP_COMPARE_REFERENCE(self);
	JagBytOpCompareReferencePrivate *priv = jag_byt_op_compare_reference_get_instance_private(instance);
	CatStringWo *result = cat_string_wo_new();
	if (priv->with_zero) {
		switch(priv->condition_type) {
			case JAG_BYT_CONDITION_TYPE_EQUAL :
				cat_string_wo_append_chars(result, "ifnull ");
				break;
			case JAG_BYT_CONDITION_TYPE_NOT_EQUAL :
				cat_string_wo_append_chars(result, "ifnonnull ");
				break;
			default :
				break;
		}
	} else {
		switch(priv->condition_type) {
			case JAG_BYT_CONDITION_TYPE_EQUAL :
				cat_string_wo_append_chars(result, "if_acmpeq ");
				break;
			case JAG_BYT_CONDITION_TYPE_NOT_EQUAL :
				cat_string_wo_append_chars(result, "if_acmpne ");
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
