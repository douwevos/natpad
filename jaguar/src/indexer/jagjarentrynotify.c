/*
   File:    jagjarentrynotify.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Sep 16, 2012
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

#include "jagjarentrynotify.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagJarEntryNotify"
#include <logging/catlog.h>

struct _JagJarEntryNotifyPrivate {
	void *dummy;
};

G_DEFINE_TYPE (JagJarEntryNotify, jag_jar_entry_notify, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_jar_entry_notify_class_init(JagJarEntryNotifyClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagJarEntryNotifyPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

//	GObjectClass *sub_clazz = G_OBJECT_CLASS(clazz);
}

static void jag_jar_entry_notify_init(JagJarEntryNotify *instance) {
	JagJarEntryNotifyPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_JAR_ENTRY_NOTIFY, JagJarEntryNotifyPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagJarEntryNotify *instance = JAG_JAR_ENTRY_NOTIFY(object);
//	JagJarEntryNotifyPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagJarEntryNotify *jag_jar_entry_notify_new() {
	JagJarEntryNotify *result = g_object_new(JAG_TYPE_JAR_ENTRY_NOTIFY, NULL);
	cat_ref_anounce(result);
//	JagJarEntryNotifyPrivate *priv = result->priv;
//	G_OBJECT_construct((GObject *) result);
	return result;
}







