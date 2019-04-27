/*
   File:    jagbytconstantfieldref.c
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

#include "jagbytconstantfieldref.h"
#include "jagbytconstantclazz.h"
#include "jagbytconstantnameandtp.h"
#include "jagbytconstantpool.h"
#include "jagbyticonstant.h"
#include "../jagbytfield.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytConstantFieldref"
#include <logging/catlog.h>

struct _JagBytConstantFieldrefPrivate {
	int class_index;
	int name_and_type_index;
	gboolean is_resolved;
	JagAstDeclarationType *declaration_type;
	JagBytConstantClazz *constant_clazz;
	JagBytConstantNameAndTp *constant_name_and_type;
};

static void l_constant_iface_init(JagBytIConstantInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytConstantFieldref, jag_byt_constant_fieldref, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagBytConstantFieldref)
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_ICONSTANT, l_constant_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_constant_fieldref_class_init(JagBytConstantFieldrefClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_constant_fieldref_init(JagBytConstantFieldref *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytConstantFieldref *instance = JAG_BYT_CONSTANT_FIELDREF(object);
	JagBytConstantFieldrefPrivate *priv = jag_byt_constant_fieldref_get_instance_private(instance);
	cat_unref_ptr(priv->declaration_type);
	cat_unref_ptr(priv->constant_clazz);
	cat_unref_ptr(priv->constant_name_and_type);
	G_OBJECT_CLASS(jag_byt_constant_fieldref_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_constant_fieldref_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytConstantFieldref *jag_byt_constant_fieldref_new(int class_index, int name_and_type_index) {
	JagBytConstantFieldref *result = g_object_new(JAG_BYT_TYPE_CONSTANT_FIELDREF, NULL);
	cat_ref_anounce(result);
	JagBytConstantFieldrefPrivate *priv = jag_byt_constant_fieldref_get_instance_private(result);
	priv->class_index = class_index;
	priv->name_and_type_index = name_and_type_index;
	priv->is_resolved = FALSE;
	priv->declaration_type = NULL;
	priv->constant_clazz = NULL;
	priv->constant_name_and_type = NULL;
	return result;
}

JagAstDeclarationType *jag_byt_constant_fieldref_get_declaration_type(JagBytConstantFieldref *constant_field_ref) {
	JagBytConstantFieldrefPrivate *priv = jag_byt_constant_fieldref_get_instance_private(constant_field_ref);
	return priv->declaration_type;
}

JagAstDeclarationType *jag_byt_constant_fieldref_get_field_source_declaration_type(JagBytConstantFieldref *constant_field_ref) {
	JagBytConstantFieldrefPrivate *priv = jag_byt_constant_fieldref_get_instance_private(constant_field_ref);
	return jag_byt_constant_clazz_get_parsed_descriptor(priv->constant_clazz);
}

CatStringWo *jag_byt_constant_fieldref_get_field_name(JagBytConstantFieldref *constant_field_ref) {
	JagBytConstantFieldrefPrivate *priv = jag_byt_constant_fieldref_get_instance_private(constant_field_ref);
	return jag_byt_constant_name_and_tp_get_name(priv->constant_name_and_type);
}

/********************* start JagBytIConstantInterface implementation *********************/

static gboolean l_constant_is_resolved(JagBytIConstant *self) {
	JagBytConstantFieldref *instance = JAG_BYT_CONSTANT_FIELDREF(self);
	JagBytConstantFieldrefPrivate *priv = jag_byt_constant_fieldref_get_instance_private(instance);
	return priv->is_resolved;
}

static gboolean l_constant_try_resolve(JagBytIConstant *self, struct _JagBytConstantPool *constantPool, CatArrayWo *e_resolveStack) {
	JagBytConstantFieldref *instance = JAG_BYT_CONSTANT_FIELDREF(self);
	JagBytConstantFieldrefPrivate *priv = jag_byt_constant_fieldref_get_instance_private(instance);
	JagBytConstantClazz *constant_clazz = (JagBytConstantClazz *) jag_byt_constant_pool_get_unresolved(constantPool, priv->class_index-1);
	JagBytConstantNameAndTp *constant_name_and_type = (JagBytConstantNameAndTp *) jag_byt_constant_pool_get_unresolved(constantPool, priv->name_and_type_index-1);

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

	if (result) {
		CatStringWo *a_descriptor = jag_byt_constant_name_and_tp_get_descriptor(constant_name_and_type);
		cat_unref_ptr(priv->declaration_type);
		priv->declaration_type = jag_byt_field_parse_descriptor(a_descriptor);
		priv->is_resolved = TRUE;
		cat_ref_swap(priv->constant_clazz, constant_clazz);
		cat_ref_swap(priv->constant_name_and_type, constant_name_and_type);
	}
	return result;
}

static void l_constant_iface_init(JagBytIConstantInterface *iface) {
	iface->isResolved = l_constant_is_resolved;
	iface->tryResolve = l_constant_try_resolve;
}

/********************* end JagBytIConstantInterface implementation *********************/
