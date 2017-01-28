/*
   File:    jagbytopgoto.c
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

#include "jagbytopgoto.h"
#include "../jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpGoto"
#include <logging/catlog.h>

struct _JagBytOpGotoPrivate {
	int branch_offset;
};

static void l_mnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytOpGoto, jag_byt_op_goto, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, {
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_mnemonic_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_goto_class_init(JagBytOpGotoClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytOpGotoPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_goto_init(JagBytOpGoto *instance) {
	JagBytOpGotoPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_OP_GOTO, JagBytOpGotoPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagBytOpGoto *instance = JAG_BYT_OP_GOTO(object);
//	JagBytOpGotoPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpGoto *jag_byt_op_goto_new(JagBytOperation operation, int offset, int branch_offset) {
	JagBytOpGoto *result = g_object_new(JAG_BYT_TYPE_OP_GOTO, NULL);
	cat_ref_anounce(result);
	JagBytOpGotoPrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, operation, offset, operation==OP_GOTO_W ? 5 : 3);
	priv->branch_offset = branch_offset;
	return result;
}






/********************* start JagBytIMnemonicInterface implementation *********************/

static int l_mnemonic_get_continues_offset(JagBytIMnemonic *self) {
	return -1;
}


static int l_mnemonic_get_branch_offset(JagBytIMnemonic *self) {
//	JagBytAbstractMnemonicPrivate *priv = JAG_BYT_ABSTRACT_MNEMONIC_GET_PRIVATE(self);
	int offset = jag_byt_imnemonic_get_offset(self);
	return JAG_BYT_OP_GOTO_GET_PRIVATE(self)->branch_offset + offset;
}




static void l_mnemonic_iface_init(JagBytIMnemonicInterface *iface) {
	JagBytIMnemonicInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->getOperation = p_iface->getOperation;
	iface->getOppCode = p_iface->getOppCode;
	iface->getOffset = p_iface->getOffset;
	iface->getContinuesOffset = l_mnemonic_get_continues_offset;
	iface->getBranchOffset = l_mnemonic_get_branch_offset;
	iface->getLength = p_iface->getLength;
}

/********************* end JagBytIMnemonicInterface implementation *********************/
