/*
   File:    jaglink.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 18, 2014
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

#include "jaglink.h"
#include "jagilinklistener.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagLink"
#include <logging/catlog.h>

struct _JagLinkPrivate {
	CatWeakList *reference_by_list;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagLink, jag_link, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_link_class_init(JagLinkClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_link_init(JagLink *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagLink *instance = JAG_LINK(object);
	JagLinkPrivate *priv = jag_link_get_instance_private(instance);
	cat_unref_ptr(priv->reference_by_list);
	G_OBJECT_CLASS(jag_link_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_link_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void jag_link_construct(JagLink *link) {
	JagLinkPrivate *priv = jag_link_get_instance_private(link);
	priv->reference_by_list = cat_weak_list_new();
}


void jag_link_add_referred_by(JagLink *link, GObject *content) {
	JagLinkPrivate *priv = jag_link_get_instance_private(link);
	cat_weak_list_append_once(priv->reference_by_list, content);
}

void jag_link_remove_referred_by(JagLink *link, GObject *content) {
	JagLinkPrivate *priv = jag_link_get_instance_private(link);
	cat_weak_list_remove(priv->reference_by_list, content);
}

void jag_link_notify(JagLink *link, GObject *notify_data) {
	JagLinkPrivate *priv = jag_link_get_instance_private(link);
	CatIIterator *iter = cat_weak_list_iterator(priv->reference_by_list);
	while(cat_iiterator_has_next(iter)) {
		GObject *referred_by = cat_iiterator_next(iter);
		if (JAG_IS_ILINK_LISTENER(referred_by)) {
			JagILinkListener *listener = JAG_ILINK_LISTENER(referred_by);
			jag_ilink_listener_notify(listener, link, notify_data);
		}
	}
	cat_unref_ptr(iter);
}

void jag_link_set_value(JagLink *link, GObject *value) {
	JAG_LINK_GET_CLASS(link)->setValue(link, value);
}

GObject *jag_link_get_value_ref(JagLink *link) {
	return JAG_LINK_GET_CLASS(link)->getValueRef(link);
}
