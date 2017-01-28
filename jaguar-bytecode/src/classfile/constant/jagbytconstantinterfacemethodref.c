/*
   File:    jagbytconstantinterfacemethodref.c
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

#include "jagbytconstantinterfacemethodref.h"
#include "jagbytconstantclazz.h"
#include "jagbytconstantpool.h"
#include "jagbytconstantnameandtp.h"
#include "jagbyticonstant.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytConstantInterfaceMethodref"
#include <logging/catlog.h>

struct _JagBytConstantInterfaceMethodrefPrivate {
	int class_index;
	int name_and_type_index;
	gboolean is_resolved;
	JagBytMethodHeader *method_header;
};

static void l_constant_iface_init(JagBytIConstantInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytConstantInterfaceMethodref, jag_byt_constant_interface_methodref, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_ICONSTANT, l_constant_iface_init);
});

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void jag_byt_constant_interface_methodref_class_init(JagBytConstantInterfaceMethodrefClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytConstantInterfaceMethodrefPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void jag_byt_constant_interface_methodref_init(JagBytConstantInterfaceMethodref *instance) {
	JagBytConstantInterfaceMethodrefPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_CONSTANT_INTERFACE_METHODREF, JagBytConstantInterfaceMethodrefPrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytConstantInterfaceMethodref *instance = JAG_BYT_CONSTANT_INTERFACE_METHODREF(object);
	JagBytConstantInterfaceMethodrefPrivate *priv = instance->priv;
	cat_unref_ptr(priv->method_header);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytConstantInterfaceMethodref *jag_byt_constant_interface_methodref_new(int class_index, int name_and_type_index) {
	JagBytConstantInterfaceMethodref *result = g_object_new(JAG_BYT_TYPE_CONSTANT_INTERFACE_METHODREF, NULL);
	cat_ref_anounce(result);
	JagBytConstantInterfaceMethodrefPrivate *priv = result->priv;
	priv->class_index = class_index;
	priv->name_and_type_index = name_and_type_index;
	priv->is_resolved = FALSE;
	priv->method_header = NULL;
	return result;
}




/********************* start JagBytIConstantInterface implementation *********************/

static gboolean l_constant_is_resolved(JagBytIConstant *self) {
	JagBytConstantInterfaceMethodrefPrivate *priv = JAG_BYT_CONSTANT_INTERFACE_METHODREF_GET_PRIVATE(self);
	return priv->is_resolved;
}

static gboolean l_constant_try_resolve(JagBytIConstant *self, struct _JagBytConstantPool *constantPool, CatArrayWo *e_resolveStack) {
	JagBytConstantInterfaceMethodrefPrivate *priv = JAG_BYT_CONSTANT_INTERFACE_METHODREF_GET_PRIVATE(self);

	JagBytConstantClazz *constant_clazz = (JagBytConstantClazz *) jag_byt_constant_pool_get_unresolved(constantPool, priv->class_index-1);
	JagBytConstantNameAndTp *constant_name_and_type = (JagBytConstantNameAndTp *) jag_byt_constant_pool_get_unresolved(constantPool, priv->name_and_type_index-1);
	JagBytConstantClazz *constant_this_clazz = (JagBytConstantClazz *) jag_byt_constant_pool_get_this_class_constant(constantPool);

	gboolean result = TRUE;

	if (!jag_byt_iconstant_is_resolved((JagBytIConstant *) constant_clazz)) {
		result = FALSE;
//		if (resolveStack.contains(constant_clazz)) {
//			throw new RecursiveConstantsException();
//		}
		cat_array_wo_append(e_resolveStack, (GObject *) constant_clazz);
	}
	if (!jag_byt_iconstant_is_resolved((JagBytIConstant *) constant_name_and_type)) {
		result = FALSE;
//		if (resolveStack.contains(constantNameAndType)) {
//			throw new RecursiveConstantsException();
//		}
		cat_array_wo_append(e_resolveStack, (GObject *) constant_name_and_type);
	}

	if (!jag_byt_iconstant_is_resolved((JagBytIConstant *) constant_this_clazz)) {
		result = FALSE;
//		if (resolveStack.contains(constant_clazz)) {
//			throw new RecursiveConstantsException();
//		}
		cat_array_wo_append(e_resolveStack, (GObject *) constant_this_clazz);
	}

	if (result) {
		JagBytName *clazz_name = jag_byt_constant_clazz_get_refrence_type_name(constant_this_clazz);
		CatStringWo *a_method_name = jag_byt_constant_name_and_tp_get_name(constant_name_and_type);
		CatStringWo *a_description = jag_byt_constant_name_and_tp_get_descriptor(constant_name_and_type);
		cat_unref_ptr(priv->method_header);

		priv->method_header = jag_byt_method_header_new(clazz_name, a_method_name, a_description);

		priv->is_resolved = TRUE;
	}
	return result;
}


static void l_constant_iface_init(JagBytIConstantInterface *iface) {
	iface->isResolved = l_constant_is_resolved;
	iface->tryResolve = l_constant_try_resolve;
}


/********************* end JagBytIConstantInterface implementation *********************/
