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

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpNewArray"
#include <logging/catlog.h>

struct _JagBytOpNewArrayPrivate {
	JagBytType array_type;
	int class_pool_index;
};

G_DEFINE_TYPE (JagBytOpNewArray, jag_byt_op_new_array, JAG_BYT_TYPE_ABSTRACT_MNEMONIC)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_new_array_class_init(JagBytOpNewArrayClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytOpNewArrayPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_new_array_init(JagBytOpNewArray *instance) {
	JagBytOpNewArrayPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_OP_NEW_ARRAY, JagBytOpNewArrayPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagBytOpNewArray *instance = JAG_BYT_OP_NEW_ARRAY(object);
//	JagBytOpNewArrayPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagBytOpNewArray *jag_byt_op_new_array_new_primitive(int offset, JagBytType array_type) {
	JagBytOpNewArray *result = g_object_new(JAG_BYT_TYPE_OP_NEW_ARRAY, NULL);
	cat_ref_anounce(result);
	JagBytOpNewArrayPrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, OP_NEWARRAY, offset, 2);
	priv->array_type = array_type;
	priv->class_pool_index = 0;
	return result;
}

JagBytOpNewArray *jag_byt_op_new_array_new_reference(int offset, int class_pool_index) {
	JagBytOpNewArray *result = g_object_new(JAG_BYT_TYPE_OP_NEW_ARRAY, NULL);
	cat_ref_anounce(result);
	JagBytOpNewArrayPrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, OP_ANEWARRAY, offset, 3);
	priv->array_type = JAG_BYT_TYPE_REFERENCE;
	priv->class_pool_index = class_pool_index;
	return result;
}

JagBytType jag_byt_op_new_array_get_primitive_type(JagBytOpNewArray *op_new_array) {
	return JAG_BYT_OP_NEW_ARRAY_GET_PRIVATE(op_new_array)->array_type;
}

int jag_byt_op_new_array_get_class_info_pool_index(JagBytOpNewArray *op_new_array) {
	return JAG_BYT_OP_NEW_ARRAY_GET_PRIVATE(op_new_array)->class_pool_index;
}

