/*
   File:    jagbytopnew.c
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

#include "jagbytopnew.h"
#include "../jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpNew"
#include <logging/catlog.h>

struct _JagBytOpNewPrivate {
	int class_pool_index;
};

static void l_imnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpNew, jag_byt_op_new, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, // @suppress("Unused static function")
		G_ADD_PRIVATE(JagBytOpNew)
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_imnemonic_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_new_class_init(JagBytOpNewClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_new_init(JagBytOpNew *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_byt_op_new_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_op_new_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpNew *jag_byt_op_new_new(int offset, int class_pool_index) {
	JagBytOpNew *result = g_object_new(JAG_BYT_TYPE_OP_NEW, NULL);
	cat_ref_anounce(result);
	JagBytOpNewPrivate *priv = jag_byt_op_new_get_instance_private(result);
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, OP_NEW, offset, 3);
	priv->class_pool_index = class_pool_index;
	return result;
}

int jag_byt_op_new_get_pool_index(JagBytOpNew *op_new) {
	JagBytOpNewPrivate *priv = jag_byt_op_new_get_instance_private(op_new);
	return priv->class_pool_index;
}

/********************* start JagBytIMnemonic implementation *********************/

static CatStringWo *l_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	JagBytOpNew *instance = JAG_BYT_OP_NEW(self);
	JagBytOpNewPrivate *priv = jag_byt_op_new_get_instance_private(instance);
	CatStringWo *result = cat_string_wo_new_with("new ");
	cat_string_wo_append_decimal(result, priv->class_pool_index);
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
