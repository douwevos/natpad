/*
   File:    jagbytopget.c
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

#include "jagbytopget.h"
#include "../jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpGet"
#include <logging/catlog.h>

struct _JagBytOpGetPrivate {
	int pool_field_reference_index;
};

static void l_mnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpGet, jag_byt_op_get, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, { // @suppress("Unused static function")
		G_ADD_PRIVATE(JagBytOpGet)
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_mnemonic_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_get_class_init(JagBytOpGetClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_get_init(JagBytOpGet *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_byt_op_get_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_op_get_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpGet *jag_byt_op_get_new(JagBytOperation operation, int offset, int pool_field_reference_index) {
	JagBytOpGet *result = g_object_new(JAG_BYT_TYPE_OP_GET, NULL);
	cat_ref_anounce(result);
	JagBytOpGetPrivate *priv = jag_byt_op_get_get_instance_private(result);
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, operation, offset, 3);
	priv->pool_field_reference_index = pool_field_reference_index;
	return result;
}

int jag_byt_op_get_get_field_reference_pool_index(JagBytOpGet *op_get) {
	JagBytOpGetPrivate *priv = jag_byt_op_get_get_instance_private(op_get);
	return priv->pool_field_reference_index;
}

/********************* start JagBytIMnemonicInterface implementation *********************/

static CatStringWo *l_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	JagBytOpGetPrivate *priv = jag_byt_op_get_get_instance_private(JAG_BYT_OP_GET(self));
	CatStringWo *result = cat_string_wo_new();
	short opp_code = jag_byt_imnemonic_get_opp_code(self);
	switch(opp_code) {
		case OP_GETSTATIC :
			cat_string_wo_append_chars(result, "getstatic ");
			break;
		case OP_GETFIELD :
			cat_string_wo_append_chars(result, "getfield ");
			break;
		default :
			break;
	}
	cat_string_wo_append_decimal(result, priv->pool_field_reference_index);
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
