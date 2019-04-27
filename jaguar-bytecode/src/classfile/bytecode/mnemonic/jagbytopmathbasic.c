/*
   File:    jagbytopmathbasic.c
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

#include "jagbytopmathbasic.h"
#include "../jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpMathBasic"
#include <logging/catlog.h>

struct _JagBytOpMathBasicPrivate {
	JagBytType infix_type;
	JagBytMathOperator math_operator;
};

static void l_imnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpMathBasic, jag_byt_op_math_basic, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, // @suppress("Unused static function")
		G_ADD_PRIVATE(JagBytOpMathBasic)
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_imnemonic_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_math_basic_class_init(JagBytOpMathBasicClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_math_basic_init(JagBytOpMathBasic *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_byt_op_math_basic_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_op_math_basic_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpMathBasic *jag_byt_op_math_basic_new(JagBytOperation operation, int offset, JagBytType infix_type, JagBytMathOperator math_operator) {
	JagBytOpMathBasic *result = g_object_new(JAG_BYT_TYPE_OP_MATH_BASIC, NULL);
	cat_ref_anounce(result);
	JagBytOpMathBasicPrivate *priv = jag_byt_op_math_basic_get_instance_private(result);
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, operation, offset, 1);
	priv->infix_type = infix_type;
	priv->math_operator = math_operator;
	return result;
}


JagBytMathOperator jag_byt_op_math_basic_get_math_operator(JagBytOpMathBasic *op_math_logic) {
	JagBytOpMathBasicPrivate *priv = jag_byt_op_math_basic_get_instance_private(op_math_logic);
	return priv->math_operator;
}

/********************* start JagBytIMnemonic implementation *********************/

static CatStringWo *l_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	CatStringWo *result = cat_string_wo_new();
	JagBytOpMathBasic *instance= JAG_BYT_OP_MATH_BASIC(self);
	JagBytOpMathBasicPrivate *priv = jag_byt_op_math_basic_get_instance_private(instance);
	switch(priv->infix_type) {
		case JAG_BYT_TYPE_INT : cat_string_wo_append_char(result, 'i'); break;
		case JAG_BYT_TYPE_LONG : cat_string_wo_append_char(result, 'l'); break;
		case JAG_BYT_TYPE_FLOAT : cat_string_wo_append_char(result, 'f'); break;
		case JAG_BYT_TYPE_DOUBLE : cat_string_wo_append_char(result, 'd'); break;
		default : break;
	}

	switch(priv->math_operator) {
		case JAG_BYT_MATH_OPERATOR_ADD : cat_string_wo_append_chars(result, "add"); break;
		case JAG_BYT_MATH_OPERATOR_SUB : cat_string_wo_append_chars(result, "sub"); break;
		case JAG_BYT_MATH_OPERATOR_MULTIPLY : cat_string_wo_append_chars(result, "mul"); break;
		case JAG_BYT_MATH_OPERATOR_DIVIDE : cat_string_wo_append_chars(result, "div"); break;
		case JAG_BYT_MATH_OPERATOR_REMAINDER : cat_string_wo_append_chars(result, "rem"); break;
		case JAG_BYT_MATH_OPERATOR_UNARY_MINUS : cat_string_wo_append_chars(result, "neg"); break;
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
