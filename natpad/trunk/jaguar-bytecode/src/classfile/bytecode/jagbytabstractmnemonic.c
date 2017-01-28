/*
   File:    jagbytabstractmnemonic.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 2, 2012
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

#include "jagbytabstractmnemonic.h"
#include "jagbytimnemonic.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytAbstractMnemonic"
#include <logging/catlog.h>

struct _JagBytAbstractMnemonicPrivate {
	JagBytOperation operation;
	int offset;
	int length;
};


static void l_mnemonic_iface_init(JagBytIMnemonicInterface *iface);

G_DEFINE_ABSTRACT_TYPE_WITH_CODE(JagBytAbstractMnemonic, jag_byt_abstract_mnemonic, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_IMNEMONIC, l_mnemonic_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_abstract_mnemonic_class_init(JagBytAbstractMnemonicClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytAbstractMnemonicPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_abstract_mnemonic_init(JagBytAbstractMnemonic *instance) {
	JagBytAbstractMnemonicPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_ABSTRACT_MNEMONIC, JagBytAbstractMnemonicPrivate);
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

void jag_byt_abstract_mnemonic_construct(JagBytAbstractMnemonic *abstract_mnemonic, JagBytOperation operation, int offset, int length) {
	JagBytAbstractMnemonicPrivate *priv = JAG_BYT_ABSTRACT_MNEMONIC_GET_PRIVATE(abstract_mnemonic);
	priv->operation = operation;
	priv->offset = offset;
	priv->length = length;
}




/********************* start JagBytIMnemonicInterface implementation *********************/



static JagBytOperation l_mnemonic_get_operation(JagBytIMnemonic *self) {
	JagBytAbstractMnemonicPrivate *priv = JAG_BYT_ABSTRACT_MNEMONIC_GET_PRIVATE(self);
	return priv->operation;
}


static short l_mnemonic_get_opp_code(JagBytIMnemonic *self) {
	JagBytAbstractMnemonicPrivate *priv = JAG_BYT_ABSTRACT_MNEMONIC_GET_PRIVATE(self);
	return (short) priv->operation;
}

static int l_mnemonic_get_offset(JagBytIMnemonic *self) {
	JagBytAbstractMnemonicPrivate *priv = JAG_BYT_ABSTRACT_MNEMONIC_GET_PRIVATE(self);
	return priv->offset;
}

static int l_mnemonic_get_continues_offset(JagBytIMnemonic *self) {
	JagBytAbstractMnemonicPrivate *priv = JAG_BYT_ABSTRACT_MNEMONIC_GET_PRIVATE(self);
	return priv->offset+priv->length;
}

static int l_mnemonic_get_branch_offset(JagBytIMnemonic *self) {
//	JagBytAbstractMnemonicPrivate *priv = JAG_BYT_ABSTRACT_MNEMONIC_GET_PRIVATE(self);
	return -1;
}

static int l_mnemonic_get_length(JagBytIMnemonic *self) {
	JagBytAbstractMnemonicPrivate *priv = JAG_BYT_ABSTRACT_MNEMONIC_GET_PRIVATE(self);
	return priv->length;
}

static void l_mnemonic_iface_init(JagBytIMnemonicInterface *iface) {
	iface->getOperation = l_mnemonic_get_operation;
	iface->getOppCode = l_mnemonic_get_opp_code;
	iface->getOffset = l_mnemonic_get_offset;
	iface->getContinuesOffset = l_mnemonic_get_continues_offset;
	iface->getBranchOffset = l_mnemonic_get_branch_offset;
	iface->getLength = l_mnemonic_get_length;

}

/********************* end JagBytIMnemonicInterface implementation *********************/
