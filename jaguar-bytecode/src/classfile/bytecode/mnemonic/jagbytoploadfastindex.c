/*
   File:    jagbytoploadfastindex.c
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

#include "jagbytoploadfastindex.h"
#include "../jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpLoadFastIndex"
#include <logging/catlog.h>

struct _JagBytOpLoadFastIndexPrivate {
	JagBytType value_type;
	int index;
};

static void l_imnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpLoadFastIndex, jag_byt_op_load_fast_index, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_imnemonic_iface_init)
);

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_load_fast_index_class_init(JagBytOpLoadFastIndexClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytOpLoadFastIndexPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_load_fast_index_init(JagBytOpLoadFastIndex *instance) {
	JagBytOpLoadFastIndexPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_OP_LOAD_FAST_INDEX, JagBytOpLoadFastIndexPrivate);
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

JagBytOpLoadFastIndex *jag_byt_op_load_fast_index_new(JagBytOperation operation, int offset, JagBytType value_type, int frame_index) {
	JagBytOpLoadFastIndex *result = g_object_new(JAG_BYT_TYPE_OP_LOAD_FAST_INDEX, NULL);
	cat_ref_anounce(result);
	JagBytOpLoadFastIndexPrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, operation, offset, 1);
	priv->value_type = value_type;
	priv->index = frame_index;
	return result;
}

int jag_byt_op_load_fast_index_get_frame_index(JagBytOpLoadFastIndex *load_fast_index) {
	return JAG_BYT_OP_LOAD_FAST_INDEX_GET_PRIVATE(load_fast_index)->index;
}

/********************* start JagBytIMnemonic implementation *********************/

static CatStringWo *l_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	CatStringWo *result = cat_string_wo_new();
	short op_code = jag_byt_imnemonic_get_opp_code(self);
	switch(op_code) {
		case OP_ILOAD_0 : cat_string_wo_append_chars(result, "iload_0"); break;
		case OP_ILOAD_1 : cat_string_wo_append_chars(result, "iload_1"); break;
		case OP_ILOAD_2 : cat_string_wo_append_chars(result, "iload_2"); break;
		case OP_ILOAD_3 : cat_string_wo_append_chars(result, "iload_3"); break;
		case OP_LLOAD_0 : cat_string_wo_append_chars(result, "lload_0"); break;
		case OP_LLOAD_1 : cat_string_wo_append_chars(result, "lload_1"); break;
		case OP_LLOAD_2 : cat_string_wo_append_chars(result, "lload_2"); break;
		case OP_LLOAD_3 : cat_string_wo_append_chars(result, "lload_3"); break;
		case OP_FLOAD_0 : cat_string_wo_append_chars(result, "fload_0"); break;
		case OP_FLOAD_1 : cat_string_wo_append_chars(result, "fload_1"); break;
		case OP_FLOAD_2 : cat_string_wo_append_chars(result, "fload_2"); break;
		case OP_FLOAD_3 : cat_string_wo_append_chars(result, "fload_3"); break;
		case OP_DLOAD_0 : cat_string_wo_append_chars(result, "dload_0"); break;
		case OP_DLOAD_1 : cat_string_wo_append_chars(result, "dload_1"); break;
		case OP_DLOAD_2 : cat_string_wo_append_chars(result, "dload_2"); break;
		case OP_DLOAD_3 : cat_string_wo_append_chars(result, "dload_3"); break;
		case OP_ALOAD_0 : cat_string_wo_append_chars(result, "aload_0"); break;
		case OP_ALOAD_1 : cat_string_wo_append_chars(result, "aload_1"); break;
		case OP_ALOAD_2 : cat_string_wo_append_chars(result, "aload_2"); break;
		case OP_ALOAD_3 : cat_string_wo_append_chars(result, "aload_3"); break;
		default :
			break;
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
