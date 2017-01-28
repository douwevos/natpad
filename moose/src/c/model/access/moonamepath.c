/*
   File:    moonamepath.c
   Project: moose
   Author:  Douwe Vos
   Date:    Jun 29, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#include "moonamepath.h"
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooNamePath"
#include <logging/catlog.h>

struct _MooNamePathPrivate {
	CatArrayWo *entries;
	int *index_cache;
	MooINodeMatcher *matcher;
};

static void l_node_matcher_iface_init(MooINodeMatcherInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooNamePath, moo_name_path, G_TYPE_OBJECT,{
		G_IMPLEMENT_INTERFACE(MOO_TYPE_INODE_MATCHER, l_node_matcher_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_name_path_class_init(MooNamePathClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(MooNamePathPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_name_path_init(MooNamePath *instance) {
	MooNamePathPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_NAME_PATH, MooNamePathPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooNamePath *instance = MOO_NAME_PATH(object);
	MooNamePathPrivate *priv = instance->priv;
	cat_unref_ptr(priv->entries);
	cat_unref_ptr(priv->matcher);
	cat_free_ptr(priv->index_cache);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooNamePath *moo_name_path_new(CatArrayWo *entries, MooINodeMatcher *matcher) {
	MooNamePath *result = g_object_new(MOO_TYPE_NAME_PATH, NULL);
	cat_ref_anounce(result);
	MooNamePathPrivate *priv = result->priv;
	priv->entries = cat_ref_ptr(entries);
	priv->index_cache = g_malloc0(sizeof(int) * cat_array_wo_size(priv->entries));
	priv->matcher = cat_ref_ptr(priv->matcher);
	return result;
}



MooNodeWo *moo_name_path_get_tail(MooNamePath *path, MooNodeWo *base_node) {
	MooNamePathPrivate *priv = MOO_NAME_PATH_GET_PRIVATE(path);
	int entry_idx = 0;

	MooINodeMatcher *matcher = priv->matcher ? priv->matcher : (MooINodeMatcher *) path;
	for(entry_idx=0; entry_idx<cat_array_wo_size(priv->entries); entry_idx++) {
		CatStringWo *name = (CatStringWo *) cat_array_wo_get(priv->entries, entry_idx);
		int cached_index = priv->index_cache[entry_idx];
		int new_index = moo_node_wo_find_index(base_node, matcher, name, cached_index);
		if (new_index>=0) {
			priv->index_cache[entry_idx] = new_index;
			base_node = moo_node_wo_child_at(base_node, new_index);
		} else {
			base_node = NULL;
			break;
		}
	}
	return base_node;
}


MooIdPath *moo_name_path_to_id_path(MooNamePath *path, MooNodeWo *base_node, MooIdPath *base_path) {
	MooNamePathPrivate *priv = MOO_NAME_PATH_GET_PRIVATE(path);
	int entry_idx = 0;
	cat_ref_ptr(base_path);

	MooINodeMatcher *matcher = priv->matcher ? priv->matcher : (MooINodeMatcher *) path;
	for(entry_idx=0; entry_idx<cat_array_wo_size(priv->entries); entry_idx++) {
		CatStringWo *name = (CatStringWo *) cat_array_wo_get(priv->entries, entry_idx);
		int cached_index = priv->index_cache[entry_idx];
		int new_index = moo_node_wo_find_index(base_node, matcher, name, cached_index);
		if (new_index>=0) {
			priv->index_cache[entry_idx] = new_index;
			base_node = moo_node_wo_child_at(base_node, new_index);
			MooIdPath *new_base_path = moo_id_path_append_id(base_path, moo_node_wo_get_unique_id(base_node), new_index);
			cat_unref_ptr(base_path);
			base_path = new_base_path;
		} else {
			cat_unref_ptr(base_path);
			break;
		}
	}
	return base_path;

}

CatStringWo *moo_name_path_create_flatten(MooNamePath *path) {
	MooNamePathPrivate *priv = MOO_NAME_PATH_GET_PRIVATE(path);
	CatStringWo *flatten = cat_string_wo_new();
	int entry_idx = 0;

	for(entry_idx=0; entry_idx<cat_array_wo_size(priv->entries); entry_idx++) {
		CatStringWo *name = (CatStringWo *) cat_array_wo_get(priv->entries, entry_idx);
		if (entry_idx>0) {
			cat_string_wo_append_char(flatten, '/');
		}
		cat_string_wo_append(flatten, name);
	}
	return flatten;
}

gboolean moo_name_path_equal(const MooNamePath *path_a, const MooNamePath *path_b) {
	if (path_a==path_b) {
		return TRUE;
	}
	if ((path_a==NULL) || (path_b==NULL)) {
		return FALSE;
	}
	MooNamePathPrivate *priv_a = MOO_NAME_PATH_GET_PRIVATE(path_a);
	MooNamePathPrivate *priv_b = MOO_NAME_PATH_GET_PRIVATE(path_b);

	CatArrayWo *arr_a = priv_a->entries;
	CatArrayWo *arr_b = priv_b->entries;

	if (cat_array_wo_size(arr_a)!=cat_array_wo_size(arr_b)) {
		return FALSE;
	}

	int idx;
	for(idx=cat_array_wo_size(arr_a)-1; idx>=0; idx--) {
		CatStringWo *ta = (CatStringWo *) cat_array_wo_get(arr_a, idx);
		CatStringWo *tb = (CatStringWo *) cat_array_wo_get(arr_b, idx);
		if (!cat_string_wo_equal(ta, tb)) {
			return FALSE;
		}
	}
	return TRUE;
}




/********************* begin MooINodeMatcher implementation *********************/


static gboolean l_node_matcher_matches(MooINodeMatcher *self, struct _MooNodeWo *node, gpointer userdata) {
	CatStringWo *path_entry = (CatStringWo *) userdata;
	CatStringWo *node_name = moo_node_wo_get_name(node);
	if (cat_string_wo_equal(node_name, path_entry)) {
		return TRUE;
	}
	return FALSE;
}


static void l_node_matcher_iface_init(MooINodeMatcherInterface *iface) {
	iface->matches = l_node_matcher_matches;
}

/********************* end MooINodeMatcher implementation *********************/



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	MooNamePathPrivate *priv = MOO_NAME_PATH_GET_PRIVATE(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p: %o]", iname, self, priv->entries);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
