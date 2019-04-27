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
#include "../jagbytimnemonic.h"

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

static void l_imnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpTableSwitch, jag_byt_op_table_switch, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, // @suppress("Unused static function")
		G_ADD_PRIVATE(JagBytOpTableSwitch)
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_imnemonic_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_table_switch_class_init(JagBytOpTableSwitchClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_table_switch_init(JagBytOpTableSwitch *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_byt_op_table_switch_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	JagBytOpTableSwitch *instance = JAG_BYT_OP_TABLE_SWITCH(object);
	JagBytOpTableSwitchPrivate *priv = jag_byt_op_table_switch_get_instance_private(instance);
	if (priv->table!=NULL) {
		g_free(priv->table);
		priv->table = NULL;
	}
	G_OBJECT_CLASS(jag_byt_op_table_switch_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpTableSwitch *jag_byt_op_table_switch_new(int offset, int default_value, int low_value, int high_value, int *table) {
	JagBytOpTableSwitch *result = g_object_new(JAG_BYT_TYPE_OP_TABLE_SWITCH, NULL);
	cat_ref_anounce(result);
	JagBytOpTableSwitchPrivate *priv = jag_byt_op_table_switch_get_instance_private(result);
	int pad = (3-(offset+1))%4;
	int length = (high_value-low_value+1)*4+12 + pad;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, OP_TABLESWITCH, offset, length);
	priv->default_value = default_value;
	priv->low_value = low_value;
	priv->high_value = high_value;
	priv->table = table;
	return result;
}

/********************* start JagBytIMnemonic implementation *********************/

static CatStringWo *l_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	JagBytOpTableSwitch *instance = JAG_BYT_OP_TABLE_SWITCH(self);
	JagBytOpTableSwitchPrivate *priv = jag_byt_op_table_switch_get_instance_private(instance);
	CatStringWo *result = cat_string_wo_new_with("tableswitch ");

	cat_string_wo_append_decimal(result, priv->low_value);
	cat_string_wo_append_char(result, '/');
	cat_string_wo_append_decimal(result, priv->high_value);

	int count = priv->high_value-priv->low_value+1;
	int idx;
	for(idx=0; idx<count; idx++) {
		int u = priv->table[idx];
		cat_string_wo_append_chars(result, ", ");
		cat_string_wo_append_decimal(result, u);
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
