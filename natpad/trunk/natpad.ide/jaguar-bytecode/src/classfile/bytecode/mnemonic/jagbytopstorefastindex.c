/*
   File:    jagbytopstorefastindex.c
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

#include "jagbytopstorefastindex.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpStoreFastIndex"
#include <logging/catlog.h>

struct _JagBytOpStoreFastIndexPrivate {
	JagBytType value_type;
	int index;
};

G_DEFINE_TYPE (JagBytOpStoreFastIndex, jag_byt_op_store_fast_index, JAG_BYT_TYPE_ABSTRACT_MNEMONIC)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_store_fast_index_class_init(JagBytOpStoreFastIndexClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytOpStoreFastIndexPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_store_fast_index_init(JagBytOpStoreFastIndex *instance) {
	JagBytOpStoreFastIndexPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_OP_STORE_FAST_INDEX, JagBytOpStoreFastIndexPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagBytOpStoreFastIndex *instance = JAG_BYT_OP_STORE_FAST_INDEX(object);
//	JagBytOpStoreFastIndexPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpStoreFastIndex *jag_byt_op_store_fast_index_new(JagBytOperation operation, int offset, JagBytType value_type, int index) {
	JagBytOpStoreFastIndex *result = g_object_new(JAG_BYT_TYPE_OP_STORE_FAST_INDEX, NULL);
	cat_ref_anounce(result);
	JagBytOpStoreFastIndexPrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, operation, offset, 1);
	priv->value_type = value_type;
	priv->index = index;
	return result;
}


int jag_byt_op_store_fast_index_get_frame_index(JagBytOpStoreFastIndex *store_fast_index) {
	return JAG_BYT_OP_STORE_FAST_INDEX_GET_PRIVATE(store_fast_index)->index;
}






