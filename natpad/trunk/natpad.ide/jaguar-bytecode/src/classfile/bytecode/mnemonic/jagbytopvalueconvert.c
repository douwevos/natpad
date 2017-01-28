/*
   File:    jagbytopvalueconvert.c
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

#include "jagbytopvalueconvert.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytOpValueConvert"
#include <logging/catlog.h>

struct _JagBytOpValueConvertPrivate {
	JagBytType source_type;
	JagBytType destination_type;
};

G_DEFINE_TYPE (JagBytOpValueConvert, jag_byt_op_value_convert, JAG_BYT_TYPE_ABSTRACT_MNEMONIC)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_op_value_convert_class_init(JagBytOpValueConvertClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytOpValueConvertPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_op_value_convert_init(JagBytOpValueConvert *instance) {
	JagBytOpValueConvertPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_OP_VALUE_CONVERT, JagBytOpValueConvertPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagBytOpValueConvert *instance = JAG_BYT_OP_VALUE_CONVERT(object);
//	JagBytOpValueConvertPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytOpValueConvert *jag_byt_op_value_convert_new(JagBytOperation operation, int offset, JagBytType source_type, JagBytType destination_type) {
	JagBytOpValueConvert *result = g_object_new(JAG_BYT_TYPE_OP_VALUE_CONVERT, NULL);
	cat_ref_anounce(result);
	JagBytOpValueConvertPrivate *priv = result->priv;
	jag_byt_abstract_mnemonic_construct((JagBytAbstractMnemonic *) result, operation, offset, 1);
	priv->source_type = source_type;
	priv->destination_type = destination_type;
	return result;
}






