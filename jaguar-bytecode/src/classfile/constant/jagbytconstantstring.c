/*
   File:    jagbytconstantstring.c
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

#include "jagbytconstantstring.h"
#include "jagbytconstantutf8text.h"
#include "jagbytconstantpool.h"
#include "jagbyticonstant.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytConstantString"
#include <logging/catlog.h>

struct _JagBytConstantStringPrivate {
	int string_index;
	gboolean is_resolved;
	CatStringWo *a_string;
};

static void l_constant_iface_init(JagBytIConstantInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytConstantString, jag_byt_constant_string, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagBytConstantString)
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_ICONSTANT, l_constant_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_constant_string_class_init(JagBytConstantStringClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_constant_string_init(JagBytConstantString *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytConstantString *instance = JAG_BYT_CONSTANT_STRING(object);
	JagBytConstantStringPrivate *priv = jag_byt_constant_string_get_instance_private(instance);
	cat_unref_ptr(priv->a_string);
	G_OBJECT_CLASS(jag_byt_constant_string_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_constant_string_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytConstantString *jag_byt_constant_string_new(int string_index) {
	JagBytConstantString *result = g_object_new(JAG_BYT_TYPE_CONSTANT_STRING, NULL);
	cat_ref_anounce(result);
	JagBytConstantStringPrivate *priv = jag_byt_constant_string_get_instance_private(result);
	priv->string_index = string_index;
	priv->is_resolved = FALSE;
	priv->a_string = NULL;
	return result;
}

CatStringWo *jag_byt_constant_string_get_value(JagBytConstantString *constant_string) {
	JagBytConstantStringPrivate *priv = jag_byt_constant_string_get_instance_private(constant_string);
	return priv->a_string;
}


/********************* start JagBytIConstantInterface implementation *********************/

static gboolean l_constant_is_resolved(JagBytIConstant *self) {
	JagBytConstantString *instance = JAG_BYT_CONSTANT_STRING(self);
	JagBytConstantStringPrivate *priv = jag_byt_constant_string_get_instance_private(instance);
	return priv->is_resolved;
}

static gboolean l_constant_try_resolve(JagBytIConstant *self, struct _JagBytConstantPool *constantPool, CatArrayWo *e_resolveStack) {
	JagBytConstantString *instance = JAG_BYT_CONSTANT_STRING(self);
	JagBytConstantStringPrivate *priv = jag_byt_constant_string_get_instance_private(instance);

	JagBytConstantUtf8Text *constant_string = (JagBytConstantUtf8Text *) jag_byt_constant_pool_get_unresolved(constantPool, priv->string_index-1);
	CatStringWo *a_text = jag_byt_constant_utf8_text_get_value(constant_string);
	cat_ref_swap(priv->a_string, a_text);
	priv->is_resolved = TRUE;
	return TRUE;
}

static void l_constant_iface_init(JagBytIConstantInterface *iface) {
	iface->isResolved = l_constant_is_resolved;
	iface->tryResolve = l_constant_try_resolve;
}

/********************* end JagBytIConstantInterface implementation *********************/
