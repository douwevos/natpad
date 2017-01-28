/*
   File:    jagbytconstantnameandtype.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Feb 1, 2012
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

#include "jagbytconstantnameandtp.h"
#include "jagbytconstantutf8text.h"
#include "jagbytconstantpool.h"
#include "jagbyticonstant.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytConstantNameAndTp"
#include <logging/catlog.h>

struct _JagBytConstantNameAndTpPrivate {
	int name_index;
	int descriptor_index;
	gboolean is_resolved;
	CatStringWo *a_name;
	CatStringWo *a_descriptor;
};

static void l_constant_iface_init(JagBytIConstantInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytConstantNameAndTp, jag_byt_constant_name_and_tp, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_ICONSTANT, l_constant_iface_init);
});

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void jag_byt_constant_name_and_tp_class_init(JagBytConstantNameAndTpClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytConstantNameAndTpPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void jag_byt_constant_name_and_tp_init(JagBytConstantNameAndTp *instance) {
	JagBytConstantNameAndTpPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_CONSTANT_NAME_AND_TP, JagBytConstantNameAndTpPrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytConstantNameAndTp *instance = JAG_BYT_CONSTANT_NAME_AND_TP(object);
	JagBytConstantNameAndTpPrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_name);
	cat_unref_ptr(priv->a_descriptor);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytConstantNameAndTp *jag_byt_constant_name_and_tp_new(int name_index, int descriptor_index) {
	JagBytConstantNameAndTp *result = g_object_new(JAG_BYT_TYPE_CONSTANT_NAME_AND_TP, NULL);
	cat_ref_anounce(result);
	JagBytConstantNameAndTpPrivate *priv = result->priv;
	priv->name_index = name_index;
	priv->descriptor_index = descriptor_index;
	priv->is_resolved = FALSE;
	priv->a_descriptor = NULL;
	priv->a_name = NULL;
	return result;
}


CatStringWo *jag_byt_constant_name_and_tp_get_name(JagBytConstantNameAndTp *name_and_type) {
	JagBytConstantNameAndTpPrivate *priv = JAG_BYT_CONSTANT_NAME_AND_TP_GET_PRIVATE(name_and_type);
	return priv->a_name;
}

CatStringWo *jag_byt_constant_name_and_tp_get_descriptor(JagBytConstantNameAndTp *name_and_type) {
	JagBytConstantNameAndTpPrivate *priv = JAG_BYT_CONSTANT_NAME_AND_TP_GET_PRIVATE(name_and_type);
	return priv->a_descriptor;
}




/********************* start JagBytIConstantInterface implementation *********************/

static gboolean l_constant_is_resolved(JagBytIConstant *self) {
	JagBytConstantNameAndTpPrivate *priv = JAG_BYT_CONSTANT_NAME_AND_TP_GET_PRIVATE(self);
	return priv->is_resolved;
}

static gboolean l_constant_try_resolve(JagBytIConstant *self, struct _JagBytConstantPool *constantPool, CatArrayWo *e_resolveStack) {
	JagBytConstantNameAndTpPrivate *priv = JAG_BYT_CONSTANT_NAME_AND_TP_GET_PRIVATE(self);
	JagBytConstantUtf8Text *constant_name = (JagBytConstantUtf8Text *) jag_byt_constant_pool_get_unresolved(constantPool, priv->name_index-1);
	JagBytConstantUtf8Text *constant_descriptor = (JagBytConstantUtf8Text *) jag_byt_constant_pool_get_unresolved(constantPool, priv->descriptor_index-1);


	if (constant_name) {
		CatStringWo *a_name = jag_byt_constant_utf8_text_get_value(constant_name);
		cat_ref_swap(priv->a_name, a_name);
	}

	if (constant_descriptor) {
		CatStringWo *a_descriptor = jag_byt_constant_utf8_text_get_value(constant_descriptor);
		cat_ref_swap(priv->a_descriptor, a_descriptor);
	}

	priv->is_resolved = TRUE;
	return TRUE;
}


static void l_constant_iface_init(JagBytIConstantInterface *iface) {
	iface->isResolved = l_constant_is_resolved;
	iface->tryResolve = l_constant_try_resolve;
}


/********************* end JagBytIConstantInterface implementation *********************/
