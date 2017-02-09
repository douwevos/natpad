/*
   File:    jagbytoppushindex.c
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

#include "jagbytoppushindex.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpPushIndex"
#include <logging/catlog.h>

struct _JagBytOpPushIndexPrivate {
	gboolean is_wide;
	int index;
	gboolean is_long_or_double;
};

G_DEFINE_TYPE (JagBytOpPushIndex, jag_byt_op_push_index, JAG_BYT_TYPE_ABSTRACT_MNEMONIC)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_push_index_class_init(JagBytOpPushIndexClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytOpPushIndexPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_push_index_init(JagBytOpPushIndex *instance) {
	JagBytOpPushIndexPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_OP_PUSH_INDEX, JagBytOpPushIndexPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagBytOpPushIndex *instance = JAG_BYT_OP_PUSH_INDEX(object);
//	JagBytOpPushIndexPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpPushIndex *jag_byt_op_push_index_new(JagBytOperation operation, int offset, int length, gboolean is_wide, int index, gboolean is_long_or_double) {
	JagBytOpPushIndex *result = g_object_new(JAG_BYT_TYPE_OP_PUSH_INDEX, NULL);
	cat_ref_anounce(result);
	JagBytOpPushIndexPrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, operation, offset, length);
	priv->is_wide = is_wide;
	priv->index = index;
	priv->is_long_or_double = is_long_or_double;
	return result;
}


int jag_byt_op_push_index_get_pool_index(JagBytOpPushIndex *op_push_index) {
	return JAG_BYT_OP_PUSH_INDEX_GET_PRIVATE(op_push_index)->index;
}




