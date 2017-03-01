/*
   File:    xandomnode.c
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 13, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#include "xandomnode.h"
#include <turtle.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "XanDomNode"
#include <logging/catlog.h>

struct _XanDomNodePrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_node_iface_init(TurINodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(XanDomNode, xan_dom_node, G_TYPE_OBJECT,
		G_ADD_PRIVATE(XanDomNode)
		G_IMPLEMENT_INTERFACE(TUR_TYPE_INODE, l_node_iface_init)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void xan_dom_node_class_init(XanDomNodeClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void xan_dom_node_init(XanDomNode *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	XanDomNode *instance = XAN_DOM_NODE(object);
//	XanDomNodePrivate *priv = xan_dom_node_get_instance_private(instance);
	G_OBJECT_CLASS(xan_dom_node_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(xan_dom_node_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


XanDomNode *xan_dom_node_new() {
	XanDomNode *result = g_object_new(XAN_TYPE_DOM_NODE, NULL);
	cat_ref_anounce(result);
	XanDomNodePrivate *priv = xan_dom_node_get_instance_private(result);
	return result;
}


/********************* start TurINode implementation *********************/

static gboolean l_node_has_children(TurINode *node) {
	return FALSE;
}

static int l_node_id(TurINode *node) {
	return 1;
}

static int l_node_version(TurINode *node) {
	return 1;
}

static CatStringWo *l_node_name(TurINode *node) {
	return cat_string_wo_new_with("Testnode");
}


static void l_node_iface_init(TurINodeInterface *iface) {
	iface->hasChildren = l_node_has_children;
	iface->id = l_node_id;
	iface->version = l_node_version;
	iface->name = l_node_name;
}

/********************* end TurINode implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	XanDomNode *instance = XAN_DOM_NODE(self);
	XanDomNodePrivate *priv = xan_dom_node_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
