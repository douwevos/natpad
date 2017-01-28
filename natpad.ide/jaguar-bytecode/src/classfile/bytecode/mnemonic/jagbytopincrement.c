/*
   File:    jagbytopincrement.c
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

#include "jagbytopincrement.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpIncrement"
#include <logging/catlog.h>

struct _JagBytOpIncrementPrivate {
	int index;
	int constant;
	gboolean is_wide;
};

G_DEFINE_TYPE (JagBytOpIncrement, jag_byt_op_increment, JAG_BYT_TYPE_ABSTRACT_MNEMONIC)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_increment_class_init(JagBytOpIncrementClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytOpIncrementPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_increment_init(JagBytOpIncrement *instance) {
	JagBytOpIncrementPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_OP_INCREMENT, JagBytOpIncrementPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagBytOpIncrement *instance = JAG_BYT_OP_INCREMENT(object);
//	JagBytOpIncrementPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpIncrement *jag_byt_op_increment_new(int offset, int index, int constant, gboolean is_wide) {
	JagBytOpIncrement *result = g_object_new(JAG_BYT_TYPE_OP_INCREMENT, NULL);
	cat_ref_anounce(result);
	JagBytOpIncrementPrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, OP_IINC, offset, is_wide ? 5 : 3);
	priv->index = index;
	priv->constant = constant;
	priv->is_wide = is_wide;
	return result;
}



int jag_byt_op_increment_get_frame_index(JagBytOpIncrement *op_increment) {
	return JAG_BYT_OP_INCREMENT_GET_PRIVATE(op_increment)->index;
}

int jag_byt_op_increment_get_increment_value(JagBytOpIncrement *op_increment) {
	return JAG_BYT_OP_INCREMENT_GET_PRIVATE(op_increment)->constant;
}



