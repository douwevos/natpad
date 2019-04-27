/*
   File:    jagbytconstantfloat.c
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

#include "jagbytconstantfloat.h"
#include "jagbyticonstant.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytConstantFloat"
#include <logging/catlog.h>

struct _JagBytConstantFloatPrivate {
	int32_t value;
};

static void l_constant_iface_init(JagBytIConstantInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytConstantFloat, jag_byt_constant_float, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagBytConstantFloat)
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_ICONSTANT, l_constant_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_constant_float_class_init(JagBytConstantFloatClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_constant_float_init(JagBytConstantFloat *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_byt_constant_float_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_constant_float_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytConstantFloat *jag_byt_constant_float_new(int32_t value) {
	JagBytConstantFloat *result = g_object_new(JAG_BYT_TYPE_CONSTANT_FLOAT, NULL);
	cat_ref_anounce(result);
	JagBytConstantFloatPrivate *priv = jag_byt_constant_float_get_instance_private(result);
	priv->value = value;
	return result;
}

float jag_byt_constant_float_get_value(JagBytConstantFloat *constant_float) {
	JagBytConstantFloatPrivate *priv = jag_byt_constant_float_get_instance_private(constant_float);
	return (float) priv->value;	// TODO test if cast works as expected
}


/********************* start JagBytIConstantInterface implementation *********************/

static gboolean l_constant_is_resolved(JagBytIConstant *self) {
	return TRUE;
}

static gboolean l_constant_try_resolve(JagBytIConstant *self, struct _JagBytConstantPool *constantPool, CatArrayWo *e_resolveStack) {
	return TRUE;
}


static void l_constant_iface_init(JagBytIConstantInterface *iface) {
	iface->isResolved = l_constant_is_resolved;
	iface->tryResolve = l_constant_try_resolve;
}

/********************* end JagBytIConstantInterface implementation *********************/
