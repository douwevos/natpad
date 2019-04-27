/*
   File:    jagbytconstantlong.c
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

#include "jagbytconstantlong.h"
#include "jagbyticonstant.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytConstantLong"
#include <logging/catlog.h>

struct _JagBytConstantLongPrivate {
	int64_t value;
};

static void l_constant_iface_init(JagBytIConstantInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytConstantLong, jag_byt_constant_long, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagBytConstantLong)
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_ICONSTANT, l_constant_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_constant_long_class_init(JagBytConstantLongClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_constant_long_init(JagBytConstantLong *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_byt_constant_long_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_constant_long_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytConstantLong *jag_byt_constant_long_new(int64_t value) {
	JagBytConstantLong *result = g_object_new(JAG_BYT_TYPE_CONSTANT_LONG, NULL);
	cat_ref_anounce(result);
	JagBytConstantLongPrivate *priv = jag_byt_constant_long_get_instance_private(result);
	priv->value = value;
	return result;
}

int64_t jag_byt_constant_long_get_value(JagBytConstantLong *constant_long) {
	JagBytConstantLongPrivate *priv = jag_byt_constant_long_get_instance_private(constant_long);
	return priv->value;
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
