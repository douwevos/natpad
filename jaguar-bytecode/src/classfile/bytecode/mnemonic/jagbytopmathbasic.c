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

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpMathBasic"
#include <logging/catlog.h>

struct _JagBytOpMathBasicPrivate {
	JagBytType infix_type;
	JagBytMathOperator math_operator;
};

G_DEFINE_TYPE (JagBytOpMathBasic, jag_byt_op_math_basic, JAG_BYT_TYPE_ABSTRACT_MNEMONIC)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_math_basic_class_init(JagBytOpMathBasicClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytOpMathBasicPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_math_basic_init(JagBytOpMathBasic *instance) {
	JagBytOpMathBasicPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_OP_MATH_BASIC, JagBytOpMathBasicPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagBytOpMathBasic *instance = JAG_BYT_OP_MATH_BASIC(object);
//	JagBytOpMathBasicPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpMathBasic *jag_byt_op_math_basic_new(JagBytOperation operation, int offset, JagBytType infix_type, JagBytMathOperator math_operator) {
	JagBytOpMathBasic *result = g_object_new(JAG_BYT_TYPE_OP_MATH_BASIC, NULL);
	cat_ref_anounce(result);
	JagBytOpMathBasicPrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, operation, offset, 1);
	priv->infix_type = infix_type;
	priv->math_operator = math_operator;
	return result;
}


JagBytMathOperator jag_byt_op_math_basic_get_math_operator(JagBytOpMathBasic *op_math_logic) {
	return JAG_BYT_OP_MATH_BASIC_GET_PRIVATE(op_math_logic)->math_operator;
}





