/*
   File:    mooresourceclipboard.c
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Mar 8, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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


#include "mooresourceclipboard.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooResourceClipboard"
#include <logging/catlog.h>

struct _MooResourceClipboardPrivate {
	void *dummy;
};


G_DEFINE_TYPE(MooResourceClipboard, moo_resource_clipboard, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_resource_clipboard_class_init(MooResourceClipboardClass *clazz) {
	g_type_class_add_private(clazz, sizeof(MooResourceClipboardPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_resource_clipboard_init(MooResourceClipboard *instance) {
	MooResourceClipboardPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_RESOURCE_CLIPBOARD, MooResourceClipboardPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	MooResourceClipboard *instance = MOO_RESOURCE_CLIPBOARD(object);
//	MooResourceClipboardPrivate *priv = instance->priv;
	G_OBJECT_CLASS(moo_resource_clipboard_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_resource_clipboard_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooResourceClipboard *moo_resource_clipboard_new() {
	MooResourceClipboard *result = g_object_new(MOO_TYPE_RESOURCE_CLIPBOARD, NULL);
	cat_ref_anounce(result);
//	MooResourceClipboardPrivate *priv = result->priv;
//	G_OBJECT_construct((GObject *) result);
	return result;
}









