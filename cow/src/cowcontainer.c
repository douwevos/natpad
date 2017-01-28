/*
   File:    cowcontainer.c
   Project: cow
   Author:  Douwe Vos
   Date:    Aug 26, 2014
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

#include "cowcontainerpriv.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CowContainer"
#include <logging/catlog.h>

struct _CowContainerPrivate {
	CatWeakList *listeners;
	GObject *config;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CowContainer, cow_container, G_TYPE_OBJECT,
		G_ADD_PRIVATE(CowContainer)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_default_is_same_config(CowContainer *container, GObject *config_a, GObject *config_b);
static CowContainer *l_default_create_editor(CowContainer *container);

static void cow_container_class_init(CowContainerClass *clazz) {
	clazz->setConfiguration = cow_container_set_configuration_real;
	clazz->isSameConfig = l_default_is_same_config;
	clazz->createEditor = l_default_create_editor;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cow_container_init(CowContainer *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CowContainer *instance = COW_CONTAINER(object);
	CowContainerPrivate *priv = cow_container_get_instance_private(instance);
	cat_unref_ptr(priv->listeners);
	cat_unref_ptr(priv->config);
	G_OBJECT_CLASS(cow_container_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cow_container_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void cow_container_construct(CowContainer *container) {
	CowContainerPrivate *priv = cow_container_get_instance_private(container);
	priv->listeners = cat_weak_list_new();
	priv->config = NULL;
}


CowContainer *cow_container_new() {
	CowContainer *result = g_object_new(COW_TYPE_CONTAINER, NULL);
	cat_ref_anounce(result);
	cow_container_construct(result);
	return result;
}

CowContainer *cow_container_create_editor(CowContainer *container) {
	return COW_CONTAINER_GET_CLASS(container)->createEditor(container);
}

static CowContainer *l_default_create_editor(CowContainer *container) {
	CowContainer *result = cow_container_new();
	cow_container_copy_from(result, container);
	return result;
}

void cow_container_copy_from(CowContainer *dest, CowContainer *source) {
	CowContainerPrivate *spriv = cow_container_get_instance_private(source);
	CowContainerPrivate *priv = cow_container_get_instance_private(dest);
	priv->config = cat_ref_ptr(spriv->config);
}



GObject *cow_container_get_configuration(CowContainer *container) {
	CowContainerPrivate *priv = cow_container_get_instance_private(container);
	return priv->config;
}

void cow_container_set_configuration_real(CowContainer *container, gpointer new_config) {
	CowContainerPrivate *priv = cow_container_get_instance_private(container);
	if (cow_container_is_same_config(container, priv->config, new_config)) {
		return;
	}
	cat_ref_swap(priv->config, new_config);
	cat_log_debug("new_config=%p", new_config);
	cow_container_dispatch_change_event(container);
}

void cow_container_set_configuration(CowContainer *container, gpointer new_config) {
	COW_CONTAINER_GET_CLASS(container)->setConfiguration(container, new_config);
}

void cow_container_set_configuration_direct(CowContainer *container, gpointer new_config) {
	CowContainerPrivate *priv = cow_container_get_instance_private(container);
	cat_log_debug("new_config=%p", new_config);
	cat_ref_swap(priv->config, new_config);
}



void cow_container_dispatch_change_event(CowContainer *container) {
	CowContainerPrivate *priv = cow_container_get_instance_private(container);
	CatIIterator *iter = cat_weak_list_iterator(priv->listeners);
	while(cat_iiterator_has_next(iter)) {
		CowIChangeListener *listener = (CowIChangeListener *) cat_iiterator_next(iter);
		cat_log_detail("container=%p, listener=%s : config=%p", container, g_type_name_from_instance((GTypeInstance *) listener), priv->config);
		cow_ichange_listener_config_changed(listener, priv->config);
	}
	cat_unref_ptr(iter);

}


void cow_container_add_change_listener(CowContainer *container, CowIChangeListener *listener) {
	CowContainerPrivate *priv = cow_container_get_instance_private(container);
	cat_weak_list_append_once(priv->listeners, (GObject *) listener);
}

void cow_container_remove_change_listener(CowContainer *container, CowIChangeListener *listener) {
	CowContainerPrivate *priv = cow_container_get_instance_private(container);
	cat_weak_list_remove(priv->listeners, (GObject *) listener);
}


gboolean cow_container_is_same_config(CowContainer *container, GObject *config_a, GObject *config_b) {
	cat_log_debug("config_a=%p, config_b=%p", config_a, config_b);
	return COW_CONTAINER_GET_CLASS(container)->isSameConfig(container, config_a, config_b);
}

static gboolean l_default_is_same_config(CowContainer *container, GObject *config_a, GObject *config_b) {
	return config_a==config_b;
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CowContainer *instance = COW_CONTAINER(self);
	CowContainerPrivate *priv = cow_container_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p] config=%p", iname, self, priv->config);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

