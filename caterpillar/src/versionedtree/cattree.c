/*
   File:    cattree.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 1, 2014
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


#include "cattree.h"
#include "impl/catreadableentrylistprovider.h"
#include "../concurrent/catatomicreference.h"
#include "../catistringable.h"
#include "../woo/catstringwo.h"


#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatTree"
#include "../logging/catlog.h"

struct _CatTreePrivate {
		/* writer context */
	CatTreeEntryList *entry_list;

	/* reader/writer context */
	CatAtomicReference *root_reference;
};


static void l_tree_entry_list_provider_interface_init(CatITreeEntryListProviderInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatTree, cat_tree, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(CatTree)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ITREE_ENTRY_LIST_PROVIDER, l_tree_entry_list_provider_interface_init)
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatTreeEntryList *l_get_writable_entry_list(CatITreeEntryListProvider *self);

static void cat_tree_class_init(CatTreeClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_tree_init(CatTree *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatTree *instance = CAT_TREE(object);
	CatTreePrivate *priv = cat_tree_get_instance_private(instance);
	cat_unref_ptr(priv->entry_list);
	cat_unref_ptr(priv->root_reference);
	G_OBJECT_CLASS(cat_tree_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_tree_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatTree *cat_tree_new() {
	CatTree *result = g_object_new(CAT_TYPE_TREE, NULL);
	cat_ref_anounce(result);
	CatTreePrivate *priv = cat_tree_get_instance_private(result);
	priv->entry_list = cat_tree_entry_list_new();
	CatITreeEntryListProvider *provider = (CatITreeEntryListProvider *) cat_readable_entry_list_provider_new(priv->entry_list);
	CatReadableTreeNode *tree_node = cat_readable_tree_node_new(provider, 0);
	priv->root_reference = cat_atomic_reference_new_val((GObject *) tree_node);
	cat_unref_ptr(tree_node);
	cat_unref_ptr(provider);
	return result;
}



CatWritableTreeNode *cat_tree_get_writable_root_node(CatTree *tree) {
	return cat_writable_tree_node_new((CatITreeEntryListProvider *) tree, 0);
}


CatReadableTreeNode *cat_tree_get_readable_root_node_ref(CatTree *tree) {
	CatTreePrivate *priv = cat_tree_get_instance_private(tree);
	return (CatReadableTreeNode *) cat_atomic_reference_get(priv->root_reference);
}

static void l_print_from(CatTreePrivate *priv, CatTreeEntry *entry, int level) {
	CatStringWo *string = cat_string_wo_new();
	int idx=0;
	for(idx=0; idx<level; idx++) {
		cat_string_wo_append_chars(string, "  ");
	}
	if (entry==NULL) {
		cat_string_wo_append_chars(string, "<nil>");
	} else {
		cat_istringable_print((CatIStringable *) entry, string);
	}
	cat_log_print("OUT", "%o", string);
	cat_unref_ptr(string);
	if (entry==NULL) {
		return;
	}

	int count = cat_tree_entry_count(entry);
	if (count>0) {
		level++;
		for(idx=0; idx<count; idx++) {
			int child_location = cat_tree_entry_get_child(entry, idx);
			CatTreeEntry *child_entry = cat_tree_entry_list_get_entry(priv->entry_list, child_location);
			l_print_from(priv, child_entry, level);
		}
	}
}

void cat_tree_print(CatTree *tree) {
	CatTreePrivate *priv = cat_tree_get_instance_private(tree);
	cat_log_print("OUT", "entry_list=%o", priv->entry_list);
	CatTreeEntry *entry = cat_tree_entry_list_get_entry(priv->entry_list, 0);
	l_print_from(priv, entry, 0);
}

CatReadableTreeNode *cat_tree_submit(CatTree *tree, gboolean pack) {
	CatTreePrivate *priv = cat_tree_get_instance_private(tree);
	CatReadableTreeNode *result = NULL;
	if (!cat_tree_entry_list_is_marked_fixed(priv->entry_list)) {
		if (pack) {
//			entry_list.pack();
		}

		cat_tree_entry_list_mark(priv->entry_list);
		CatITreeEntryListProvider *readable_list_provider = (CatITreeEntryListProvider *) cat_readable_entry_list_provider_new(priv->entry_list);
		cat_log_on_trace({
			cat_tree_print(tree);
		});
		result = cat_readable_tree_node_new(readable_list_provider, 0);
		cat_unref_ptr(readable_list_provider);
		cat_atomic_reference_set(priv->root_reference, (GObject *) result);
	}
	return result;
}



/********************* begin CatITreeEntryListProvider implementation *********************/

static CatTreeEntryList *l_get_entry_list(CatITreeEntryListProvider *self) {
	CatTreePrivate *priv = cat_tree_get_instance_private(CAT_TREE(self));
	return priv->entry_list;
}

static CatTreeEntryList *l_get_writable_entry_list(CatITreeEntryListProvider *self) {
	CatTreePrivate *priv = cat_tree_get_instance_private(CAT_TREE(self));
	if (cat_tree_entry_list_is_marked_fixed(priv->entry_list)) {
		CatTreeEntryList *writable = cat_tree_entry_list_ensure_writable(priv->entry_list);
		cat_unref_ptr(priv->entry_list);
		priv->entry_list = writable;
	}
	return priv->entry_list;
}

static void l_tree_entry_list_provider_interface_init(CatITreeEntryListProviderInterface *iface) {
	iface->getEntryList = l_get_entry_list;
	iface->getWritableEntryList = l_get_writable_entry_list;
}


/********************* end CatITreeEntryListProvider implementation *********************/

