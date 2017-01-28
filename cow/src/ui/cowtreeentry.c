/*
   File:    cowtreeentry.c
   Project: cow
   Author:  Douwe Vos
   Date:    Aug 30, 2014
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

#include "cowtreeentry.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CowTreeEntry"
#include <logging/catlog.h>

struct _CowTreeEntryPrivate {
	CatStringWo *name;
	CatArrayWo *entries;	/* factory[n], description[n], factory[n+1], description[n+1], factory[n+2], description[n+2], ...*/
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CowTreeEntry, cow_tree_entry, G_TYPE_OBJECT,
		G_ADD_PRIVATE(CowTreeEntry)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cow_tree_entry_class_init(CowTreeEntryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cow_tree_entry_init(CowTreeEntry *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CowTreeEntry *instance = COW_TREE_ENTRY(object);
	CowTreeEntryPrivate *priv = cow_tree_entry_get_instance_private(instance);
	cat_unref_ptr(priv->name);
	cat_unref_ptr(priv->entries);
	G_OBJECT_CLASS(cow_tree_entry_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cow_tree_entry_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CowTreeEntry *cow_tree_entry_new(CatStringWo *name) {
	CowTreeEntry *result = g_object_new(COW_TYPE_TREE_ENTRY, NULL);
	cat_ref_anounce(result);
	CowTreeEntryPrivate *priv = cow_tree_entry_get_instance_private(result);
	priv->name = cat_ref_ptr(name);
	priv->entries = cat_array_wo_new();
	return result;
}

CatStringWo *cow_tree_entry_get_name(CowTreeEntry *entry) {
	CowTreeEntryPrivate *priv = cow_tree_entry_get_instance_private(entry);
	return priv->name;
}

void cow_tree_entry_add_description(CowTreeEntry *entry, CowIPanelFactory *factory, CowPanelDescription *description) {
	CowTreeEntryPrivate *priv = cow_tree_entry_get_instance_private(entry);
	int index = cat_array_wo_find_index(priv->entries, (GObject *) factory, -1);
	if (index>=0) {
		cat_array_wo_set(priv->entries, (GObject *) description, index+1, NULL);
	} else {
		cat_array_wo_append(priv->entries, (GObject *) factory);
		cat_array_wo_append(priv->entries, (GObject *) description);
	}
}

int cow_tree_entry_count(CowTreeEntry *entry) {
	CowTreeEntryPrivate *priv = cow_tree_entry_get_instance_private(entry);
	return cat_array_wo_size(priv->entries)/2;
}

CowIPanelFactory *cow_tree_entry_factory_at(CowTreeEntry *entry, int index) {
	CowTreeEntryPrivate *priv = cow_tree_entry_get_instance_private(entry);
	return (CowIPanelFactory *) cat_array_wo_get(priv->entries, index*2);
}

CowPanelDescription *cow_tree_entry_description_at(CowTreeEntry *entry, int index) {
	CowTreeEntryPrivate *priv = cow_tree_entry_get_instance_private(entry);
	return (CowPanelDescription *) cat_array_wo_get(priv->entries, 1+index*2);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CowTreeEntry *instance = COW_TREE_ENTRY(self);
	CowTreeEntryPrivate *priv = cow_tree_entry_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p] name=%o", iname, self, priv->name);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/






