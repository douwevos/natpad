/*
   File:    jagbytinterfacelist.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 1, 2012
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

#include "jagbytinterfacelist.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagBytInterfaceList"
#include <logging/catlog.h>

struct _JagBytInterfaceListPrivate {
	CatArrayWo *e_list;
};

G_DEFINE_TYPE (JagBytInterfaceList, jag_byt_interface_list, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_byt_interface_list_class_init(JagBytInterfaceListClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagBytInterfaceListPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_byt_interface_list_init(JagBytInterfaceList *instance) {
	JagBytInterfaceListPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_BYT_TYPE_INTERFACE_LIST, JagBytInterfaceListPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagBytInterfaceList *instance = JAG_BYT_INTERFACE_LIST(object);
	JagBytInterfaceListPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_list);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagBytInterfaceList *jag_byt_interface_list_new() {
	JagBytInterfaceList *result = g_object_new(JAG_BYT_TYPE_INTERFACE_LIST, NULL);
	cat_ref_anounce(result);
	JagBytInterfaceListPrivate *priv = result->priv;
	priv->e_list = cat_array_wo_new();
	return result;
}


void jag_byt_interface_list_add(JagBytInterfaceList *interface_list, JagAstDeclarationType *iface_type) {
	JagBytInterfaceListPrivate *priv = JAG_BYT_INTERFACE_LIST_GET_PRIVATE(interface_list);
	cat_array_wo_append(priv->e_list, (GObject *) iface_type);
}

int jag_byt_interface_list_count(JagBytInterfaceList *interface_list) {
	JagBytInterfaceListPrivate *priv = JAG_BYT_INTERFACE_LIST_GET_PRIVATE(interface_list);
	return cat_array_wo_size(priv->e_list);
}


JagAstDeclarationType *jag_byt_interface_list_get(JagBytInterfaceList *interface_list, int index) {
	JagBytInterfaceListPrivate *priv = JAG_BYT_INTERFACE_LIST_GET_PRIVATE(interface_list);
	return (JagAstDeclarationType *) cat_array_wo_get(priv->e_list, index);
}

