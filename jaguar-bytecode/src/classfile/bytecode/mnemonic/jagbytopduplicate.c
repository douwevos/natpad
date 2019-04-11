/*
   File:    jagbytopduplicate.c
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

#include "jagbytopduplicate.h"
#include "../jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpDuplicate"
#include <logging/catlog.h>

static void l_mnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpDuplicate, jag_byt_op_duplicate, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, { // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_mnemonic_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_duplicate_class_init(JagBytOpDuplicateClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_duplicate_init(JagBytOpDuplicate *instance) {
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

JagBytOpDuplicate *jag_byt_op_duplicate_new(JagBytOperation operation, int offset) {
	JagBytOpDuplicate *result = g_object_new(JAG_BYT_TYPE_OP_DUPLICATE, NULL);
	cat_ref_anounce(result);
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, operation, offset, 1);
	return result;
}


/********************* start JagBytIMnemonicInterface implementation *********************/

static CatStringWo *l_to_string(JagBytIMnemonic *self, JagBytLabelRepository *label_repository) {
	CatStringWo *result = cat_string_wo_new();
	short opp_code = jag_byt_imnemonic_get_opp_code(self);
	switch(opp_code) {
		case OP_DUP :
			cat_string_wo_append_chars(result, "dup");
			break;
		case OP_DUP_X1 :
			cat_string_wo_append_chars(result, "dup_x1");
			break;
		case OP_DUP_X2 :
			cat_string_wo_append_chars(result, "dup_x2");
			break;
		case OP_DUP2 :
			cat_string_wo_append_chars(result, "dup2");
			break;
		case OP_DUP2_X1 :
			cat_string_wo_append_chars(result, "dup2_x1");
			break;
		case OP_DUP2_X2 :
			cat_string_wo_append_chars(result, "dup2_x2");
			break;
		default :
			break;
	}
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
