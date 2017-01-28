/*
   File:    jagbytconstantutf8text.c
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

#include "jagbytconstantutf8text.h"
#include "jagbyticonstant.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytConstantUtf8Text"
#include <logging/catlog.h>

struct _JagBytConstantUtf8TextPrivate {
	CatStringWo *a_text;
};

static void l_constant_iface_init(JagBytIConstantInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytConstantUtf8Text, jag_byt_constant_utf8_text, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_ICONSTANT, l_constant_iface_init);
});

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void jag_byt_constant_utf8_text_class_init(JagBytConstantUtf8TextClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytConstantUtf8TextPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void jag_byt_constant_utf8_text_init(JagBytConstantUtf8Text *instance) {
	JagBytConstantUtf8TextPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_CONSTANT_UTF8_TEXT, JagBytConstantUtf8TextPrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytConstantUtf8Text *instance = JAG_BYT_CONSTANT_UTF8_TEXT(object);
	JagBytConstantUtf8TextPrivate *priv = instance->priv;
	cat_unref_ptr(priv->a_text);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytConstantUtf8Text *jag_byt_constant_utf8_text_new(CatStringWo *a_text) {
	JagBytConstantUtf8Text *result = g_object_new(JAG_BYT_TYPE_CONSTANT_UTF8_TEXT, NULL);
	cat_ref_anounce(result);
	JagBytConstantUtf8TextPrivate *priv = result->priv;
	priv->a_text = cat_ref_ptr(a_text);
	cat_log_debug("text=%s", cat_string_wo_getchars(priv->a_text));
	return result;
}


CatStringWo *jag_byt_constant_utf8_text_get_value(JagBytConstantUtf8Text *constant_ut8) {
	return constant_ut8->priv->a_text;
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
