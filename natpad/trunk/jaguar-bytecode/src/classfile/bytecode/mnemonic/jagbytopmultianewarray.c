/*
   File:    jagbytopmultianewarray.c
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

#include "jagbytopmultianewarray.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpMultiANewArray"
#include <logging/catlog.h>

struct _JagBytOpMultiANewArrayPrivate {
	int class_pool_index;
	int dim_count;
};

G_DEFINE_TYPE (JagBytOpMultiANewArray, jag_byt_op_multi_a_new_array, JAG_BYT_TYPE_ABSTRACT_MNEMONIC)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_multi_a_new_array_class_init(JagBytOpMultiANewArrayClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytOpMultiANewArrayPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_multi_a_new_array_init(JagBytOpMultiANewArray *instance) {
	JagBytOpMultiANewArrayPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_OP_MULTI_A_NEW_ARRAY, JagBytOpMultiANewArrayPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagBytOpMultiANewArray *instance = JAG_BYT_OP_MULTI_A_NEW_ARRAY(object);
//	JagBytOpMultiANewArrayPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpMultiANewArray *jag_byt_op_multi_a_new_array_new(int offset, int class_pool_index, int dim_count) {
	JagBytOpMultiANewArray *result = g_object_new(JAG_BYT_TYPE_OP_MULTI_A_NEW_ARRAY, NULL);
	cat_ref_anounce(result);
	JagBytOpMultiANewArrayPrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, OP_MULTIANEWARRAY, offset, 4);
	priv->class_pool_index = class_pool_index;
	priv->dim_count = dim_count;
	return result;
}



int jag_byt_op_multi_a_new_array_get_class_info_pool_index(JagBytOpMultiANewArray *op_multi_new_array) {
	return JAG_BYT_OP_MULTI_A_NEW_ARRAY_GET_PRIVATE(op_multi_new_array)->class_pool_index;
}

int jag_byt_op_multi_a_new_array_get_nr_of_dimensions(JagBytOpMultiANewArray *op_multi_new_array) {
	return JAG_BYT_OP_MULTI_A_NEW_ARRAY_GET_PRIVATE(op_multi_new_array)->dim_count;
}


