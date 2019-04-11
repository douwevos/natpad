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
#include "../jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpStoreFastIndex"
#include <logging/catlog.h>

struct _JagBytOpStoreFastIndexPrivate {
	JagBytType value_type;
	int index;
};

static void l_imnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpStoreFastIndex, jag_byt_op_store_fast_index, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_imnemonic_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_store_fast_index_class_init(JagBytOpStoreFastIndexClass *clazz) {
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
	G_OBJECT_CLASS(jag_byt_op_store_fast_index_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_op_store_fast_index_parent_class)->finalize(object);
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

/********************* start JagBytIMnemonic implementation *********************/

static CatStringWo *l_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	JagBytOpStoreFastIndexPrivate *priv = JAG_BYT_OP_STORE_FAST_INDEX_GET_PRIVATE(self);
	CatStringWo *result = cat_string_wo_new();
	short op_code = jag_byt_imnemonic_get_opp_code(self);
	switch(priv->value_type) {
		case JAG_BYT_TYPE_INT : cat_string_wo_append_char(result, 'i'); break;
		case JAG_BYT_TYPE_LONG : cat_string_wo_append_char(result, 'l'); break;
		case JAG_BYT_TYPE_FLOAT : cat_string_wo_append_char(result, 'f'); break;
		case JAG_BYT_TYPE_DOUBLE : cat_string_wo_append_char(result, 'd'); break;
		case JAG_BYT_TYPE_REFERENCE : cat_string_wo_append_char(result, 'a'); break;
		case JAG_BYT_TYPE_CHAR : cat_string_wo_append_char(result, 'c'); break;
		case JAG_BYT_TYPE_SHORT : cat_string_wo_append_char(result, 's'); break;
	}
	cat_string_wo_append_chars(result, "store_");
	cat_string_wo_append_decimal(result, priv->index);
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
