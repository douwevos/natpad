/*
   File:    jagbytfinally.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 4, 2012
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

#include "jagbytfinally.h"
#include "jagbyticatchorfinally.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytFinally"
#include <logging/catlog.h>

struct _JagBytFinallyPrivate {
	int handle_pc;
};

static void l_catch_or_finally_iface_init(JagBytICatchOrFinallyInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytFinally, jag_byt_finally, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_ICATCH_OR_FINALLY, l_catch_or_finally_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_finally_class_init(JagBytFinallyClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytFinallyPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_finally_init(JagBytFinally *instance) {
	JagBytFinallyPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_FINALLY, JagBytFinallyPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagBytFinally *instance = JAG_BYT_FINALLY(object);
//	JagBytFinallyPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytFinally *jag_byt_finally_new(int handle_pc) {
	JagBytFinally *result = g_object_new(JAG_BYT_TYPE_FINALLY, NULL);
	cat_ref_anounce(result);
	JagBytFinallyPrivate *priv = result->priv;
	priv->handle_pc = handle_pc;
	return result;
}






/********************* start JagBytIConstantInterface implementation *********************/

static int l_catch_or_finally_get_handle_pc(JagBytICatchOrFinally *self) {
	JagBytFinallyPrivate *priv = JAG_BYT_FINALLY_GET_PRIVATE(self);
	return priv->handle_pc;
}


static void l_catch_or_finally_iface_init(JagBytICatchOrFinallyInterface *iface) {
	iface->getHandlePc = l_catch_or_finally_get_handle_pc;
}

/********************* end JagBytIConstantInterface implementation *********************/

