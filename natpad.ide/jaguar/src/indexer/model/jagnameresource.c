/*
   File:    jagnameresource.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Sep 12, 2012
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

#include "jagnameresource.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagNameResource"
#include <logging/catlog.h>

struct _JagNameResourcePrivate {
	void *dummy;
};

G_DEFINE_TYPE (JagNameResource, jag_name_resource, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_name_resource_class_init(JagNameResourceClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagNameResourcePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

//	GObjectClass *sub_clazz = G_OBJECT_CLASS(clazz);
}

static void jag_name_resource_init(JagNameResource *instance) {
	JagNameResourcePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_NAME_RESOURCE, JagNameResourcePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagNameResource *instance = JAG_NAME_RESOURCE(object);
//	JagNameResourcePrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagNameResource *jag_name_resource_new() {
	JagNameResource *result = g_object_new(JAG_TYPE_NAME_RESOURCE, NULL);
	cat_ref_anounce(result);
//	JagNameResourcePrivate *priv = result->priv;
//	G_OBJECT_construct((GObject *) result);
	return result;
}






