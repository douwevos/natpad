/*
   File:    catreadableentrylistprovider.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 6, 2014
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


#include "catreadableentrylistprovider.h"
#include "../catitreeentrylistprovider.h"

#include "../../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatReadableEntryListProvider"
#include "../../logging/catlog.h"

struct _CatReadableEntryListProviderPrivate {
	CatTreeEntryList *entry_list;
};

static void l_tree_entry_list_provider_interface_init(CatITreeEntryListProviderInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatReadableEntryListProvider, cat_readable_entry_list_provider, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(CatReadableEntryListProvider)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ITREE_ENTRY_LIST_PROVIDER, l_tree_entry_list_provider_interface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_readable_entry_list_provider_class_init(CatReadableEntryListProviderClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_readable_entry_list_provider_init(CatReadableEntryListProvider *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatReadableEntryListProvider *instance = CAT_READABLE_ENTRY_LIST_PROVIDER(object);
	CatReadableEntryListProviderPrivate *priv = cat_readable_entry_list_provider_get_instance_private(instance);
	cat_unref_ptr(priv->entry_list);
	G_OBJECT_CLASS(cat_readable_entry_list_provider_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_readable_entry_list_provider_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatReadableEntryListProvider *cat_readable_entry_list_provider_new(CatTreeEntryList *entry_list) {
	CatReadableEntryListProvider *result = g_object_new(CAT_TYPE_READABLE_ENTRY_LIST_PROVIDER, NULL);
	cat_ref_anounce(result);
	CatReadableEntryListProviderPrivate *priv = cat_readable_entry_list_provider_get_instance_private(result);
	priv->entry_list = cat_ref_ptr(entry_list);
	return result;
}


/********************* begin CatITreeEntryListProvider implementation *********************/

static CatTreeEntryList *l_get_entry_list(CatITreeEntryListProvider *self) {
	CatReadableEntryListProvider *instance = CAT_READABLE_ENTRY_LIST_PROVIDER(self);
	CatReadableEntryListProviderPrivate *priv = cat_readable_entry_list_provider_get_instance_private(instance);
	return priv->entry_list;
}

static CatTreeEntryList *l_get_writable_entry_list(CatITreeEntryListProvider *self) {
	return NULL;
}

static void l_tree_entry_list_provider_interface_init(CatITreeEntryListProviderInterface *iface) {
	iface->getEntryList = l_get_entry_list;
	iface->getWritableEntryList = l_get_writable_entry_list;
}

/********************* end CatITreeEntryListProvider implementation *********************/
