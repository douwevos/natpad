/*
   File:    jagbytoptableswitch.c
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

#include "jagbytoptableswitch.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpTableSwitch"
#include <logging/catlog.h>

struct _JagBytOpTableSwitchPrivate {
	int default_value;
	int low_value;
	int high_value;
	int *table;
};

G_DEFINE_TYPE (JagBytOpTableSwitch, jag_byt_op_table_switch, JAG_BYT_TYPE_ABSTRACT_MNEMONIC)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_table_switch_class_init(JagBytOpTableSwitchClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytOpTableSwitchPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_table_switch_init(JagBytOpTableSwitch *instance) {
	JagBytOpTableSwitchPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_OP_TABLE_SWITCH, JagBytOpTableSwitchPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagBytOpTableSwitch *instance = JAG_BYT_OP_TABLE_SWITCH(object);
//	JagBytOpTableSwitchPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpTableSwitch *jag_byt_op_table_switch_new(int offset, int default_value, int low_value, int high_value, int *table) {
	JagBytOpTableSwitch *result = g_object_new(JAG_BYT_TYPE_OP_TABLE_SWITCH, NULL);
	cat_ref_anounce(result);
	JagBytOpTableSwitchPrivate *priv = result->priv;
	int pad = (3-(offset+1))%4;
	int length = (high_value-low_value+1)*4+12 + pad;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, OP_TABLESWITCH, offset, length);
	priv->default_value = default_value;
	priv->low_value = low_value;
	priv->high_value = high_value;
	priv->table = table;
	return result;
}
