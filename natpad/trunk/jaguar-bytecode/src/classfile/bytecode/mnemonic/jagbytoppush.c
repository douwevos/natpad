/*
   File:    jagbytoppush.c
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

#include "jagbytoppush.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpPush"
#include <logging/catlog.h>

struct _JagBytOpPushPrivate {
	int value;
};

G_DEFINE_TYPE (JagBytOpPush, jag_byt_op_push, JAG_BYT_TYPE_ABSTRACT_MNEMONIC)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_push_class_init(JagBytOpPushClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytOpPushPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_push_init(JagBytOpPush *instance) {
	JagBytOpPushPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_OP_PUSH, JagBytOpPushPrivate);
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


JagBytOpPush *jag_byt_op_push_new_byte(int offset, int value) {
	JagBytOpPush *result = g_object_new(JAG_BYT_TYPE_OP_PUSH, NULL);
	cat_ref_anounce(result);
	JagBytOpPushPrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, OP_BIPUSH, offset, 2);
	priv->value = value;
	return result;
}

JagBytOpPush *jag_byt_op_push_new_short(int offset, int value) {
	JagBytOpPush *result = g_object_new(JAG_BYT_TYPE_OP_PUSH, NULL);
	cat_ref_anounce(result);
	JagBytOpPushPrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, OP_SIPUSH, offset, 3);
	priv->value = value;
	return result;
}

int jag_byt_op_push_get_value(JagBytOpPush *op_push) {
	JagBytOpPushPrivate *priv = JAG_BYT_OP_PUSH_GET_PRIVATE(op_push);
	return priv->value;
}




