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
#include "../jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpIncrement"
#include <logging/catlog.h>

struct _JagBytOpIncrementPrivate {
	int index;
	int constant;
	gboolean is_wide;
};

static void l_mnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpIncrement, jag_byt_op_increment, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, { // @suppress("Unused static function")
		G_ADD_PRIVATE(JagBytOpIncrement)
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_mnemonic_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_increment_class_init(JagBytOpIncrementClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_increment_init(JagBytOpIncrement *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_byt_op_increment_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_op_increment_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpIncrement *jag_byt_op_increment_new(int offset, int index, int constant, gboolean is_wide) {
	JagBytOpIncrement *result = g_object_new(JAG_BYT_TYPE_OP_INCREMENT, NULL);
	cat_ref_anounce(result);
	JagBytOpIncrementPrivate *priv = jag_byt_op_increment_get_instance_private(result);
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, OP_IINC, offset, is_wide ? 5 : 3);
	priv->index = index;
	priv->constant = constant;
	priv->is_wide = is_wide;
	return result;
}

int jag_byt_op_increment_get_frame_index(JagBytOpIncrement *op_increment) {
	JagBytOpIncrementPrivate *priv = jag_byt_op_increment_get_instance_private(op_increment);
	return priv->index;
}

int jag_byt_op_increment_get_increment_value(JagBytOpIncrement *op_increment) {
	JagBytOpIncrementPrivate *priv = jag_byt_op_increment_get_instance_private(op_increment);
	return priv->constant;
}


/********************* start JagBytIMnemonicInterface implementation *********************/

static CatStringWo *l_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	JagBytOpIncrement *instance = JAG_BYT_OP_INCREMENT(self);
	JagBytOpIncrementPrivate *priv = jag_byt_op_increment_get_instance_private(instance);

	CatStringWo *result = cat_string_wo_new_with("inc ");
	int offset = jag_byt_imnemonic_get_offset(self);
	cat_string_wo_append_decimal(result, priv->index);
	cat_string_wo_append_chars(result, ", ");
	cat_string_wo_append_decimal(result, priv->constant);
	return result;
}

static void l_mnemonic_iface_init(JagBytIMnemonicInterface *iface) {
	JagBytIMnemonicInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->getOperation = p_iface->getOperation;
	iface->getOppCode = p_iface->getOppCode;
	iface->getOffset = p_iface->getOffset;
	iface->getContinuesOffset = p_iface->getContinuesOffset;
	iface->getBranchOffset = p_iface->getBranchOffset;
	iface->getLength = p_iface->getLength;
	iface->toString = l_to_string;
}

/********************* end JagBytIMnemonicInterface implementation *********************/
