/*
   File:    jagbytcatch.c
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

#include "jagbytcatch.h"
#include "jagbyticatchorfinally.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytCatch"
#include <logging/catlog.h>

struct _JagBytCatchPrivate {
	JagAstDeclarationType *exception_type;
	int handle_pc;
};

static void l_catch_or_finally_iface_init(JagBytICatchOrFinallyInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagBytCatch, jag_byt_catch, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(JagBytCatch)
		G_IMPLEMENT_INTERFACE(JAG_BYT_TYPE_ICATCH_OR_FINALLY, l_catch_or_finally_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_catch_class_init(JagBytCatchClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_catch_init(JagBytCatch *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytCatch *instance = JAG_BYT_CATCH(object);
	JagBytCatchPrivate *priv = jag_byt_catch_get_instance_private(instance);
	cat_unref_ptr(priv->exception_type);
	G_OBJECT_CLASS(jag_byt_catch_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_byt_catch_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytCatch *jag_byt_catch_new(JagAstDeclarationType *exception_type, int handle_pc) {
	JagBytCatch *result = g_object_new(JAG_BYT_TYPE_CATCH, NULL);
	cat_ref_anounce(result);
	JagBytCatchPrivate *priv = jag_byt_catch_get_instance_private(result);
	priv->exception_type = cat_ref_ptr(exception_type);
	priv->handle_pc = handle_pc;
	return result;
}

JagAstDeclarationType *jag_byt_catch_get_exception_type(JagBytCatch *catch) {
	JagBytCatchPrivate *priv = jag_byt_catch_get_instance_private(catch);
	return priv->exception_type;
}


/********************* start JagBytIConstantInterface implementation *********************/

static int l_catch_or_finally_get_handle_pc(JagBytICatchOrFinally *self) {
	JagBytCatch *instance = JAG_BYT_CATCH(self);
	JagBytCatchPrivate *priv = jag_byt_catch_get_instance_private(instance);
	return priv->handle_pc;
}


static void l_catch_or_finally_iface_init(JagBytICatchOrFinallyInterface *iface) {
	iface->getHandlePc = l_catch_or_finally_get_handle_pc;
}

/********************* end JagBytIConstantInterface implementation *********************/
