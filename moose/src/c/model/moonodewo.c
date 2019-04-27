/*
   File:    moonode.c
   Project: moose
   Author:  Douwe Vos
   Date:    May 11, 2013
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

#include "moonodewo.h"
#include "access/mooidpath.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooNodeWo"
#include <logging/catlog.h>

struct _WoInfo {
	MooNodeWo *original;
	CatWeakReference *editing_parent_ref;
};

typedef struct _WoInfo WoInfo;


struct _MooNodeWoPrivate {
	long long unique_id;
	int version;
	CatStringWo *name;
	MooContentMapWo *content_map;
	int zorder;
//	MooNodeRecursiveCounter *recursive_error_count;
	MooNodeListWo *children;
	WoInfo *wo_info;
	MooINodeExpander *node_expander;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooNodeWo, moo_node_wo, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(MooNodeWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static MooNodeWo *l_clone(MooNodeWo *to_clone);

static void moo_node_wo_class_init(MooNodeWoClass *clazz) {
	clazz->clone = l_clone;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_node_wo_init(MooNodeWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooNodeWo *instance = MOO_NODE_WO(object);
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(instance);
	cat_unref_ptr(priv->node_expander);
	cat_unref_ptr(priv->children);
	cat_unref_ptr(priv->name);
	cat_unref_ptr(priv->content_map);
	if (priv->wo_info) {
		cat_unref_ptr(priv->wo_info->original);
		cat_unref_ptr(priv->wo_info->editing_parent_ref);
		cat_free_ptr(priv->wo_info);
	}
	G_OBJECT_CLASS(moo_node_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_node_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void moo_node_wo_construct(MooNodeWo *node, long long uniqueId, CatStringWo *c_name, MooNodeListWo *children, MooContentMapWo *content_map, int zorder) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	priv->wo_info = g_new0(WoInfo, 1);
	priv->wo_info->editing_parent_ref = NULL;
	priv->wo_info->original = NULL;
	priv->unique_id = uniqueId;
	priv->version = 1;
	priv->name = c_name;
	priv->children = children==NULL ? moo_node_list_wo_new() : cat_ref_ptr(children);
	priv->node_expander = NULL;
	priv->content_map = content_map==NULL ? moo_content_map_wo_new() : cat_ref_ptr(content_map);
	priv->zorder = zorder;
}


MooNodeWo *moo_node_wo_new(long long uniqueId, CatStringWo *c_name, MooNodeListWo *children, MooContentMapWo *content_map, int zorder) {
	MooNodeWo *result = g_object_new(MOO_TYPE_NODE_WO, NULL);
	cat_ref_anounce(result);
	moo_node_wo_construct(result, uniqueId, c_name, children, content_map, zorder);
	return result;
}


void moo_node_wo_clone_values(MooNodeWo *node, MooNodeWo *node_cloned) {
	MooNodeWoPrivate *spriv = moo_node_wo_get_instance_private(node);
	MooNodeWoPrivate *dpriv = moo_node_wo_get_instance_private(node_cloned);
	dpriv->unique_id = spriv->unique_id;
	dpriv->version = spriv->version;
	dpriv->name = cat_ref_ptr(spriv->name);
	dpriv->children = cat_ref_ptr(spriv->children);
	dpriv->node_expander = cat_ref_ptr(spriv->node_expander);
	dpriv->content_map = cat_ref_ptr(spriv->content_map);
	dpriv->zorder = spriv->zorder;
	dpriv->wo_info = g_new0(WoInfo, 1);
	dpriv->wo_info->original = cat_ref_ptr(node);
	dpriv->wo_info->editing_parent_ref = NULL;
}



static MooNodeWo *l_clone(MooNodeWo *to_clone) {
	MooNodeWo *result = g_object_new(MOO_TYPE_NODE_WO, NULL);
	cat_ref_anounce(result);
	moo_node_wo_clone_values(to_clone, result);
	return result;
}



MooNodeWo *moo_node_wo_clone(MooNodeWo *to_clone) {
	return MOO_NODE_WO_GET_CLASS(to_clone)->clone(to_clone);
}


#define CHECK_IF_WRITABLE(rval) \
		if (priv->wo_info==NULL) { \
			cat_log_error("Object is read only:%o", node); \
			return rval; \
		} \


MooNodeWo *moo_node_wo_ensure_editable(MooNodeWo *node, MooNodeWo *editing_parent) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	if (priv->wo_info==NULL) {
		MooNodeWo *new_node = MOO_NODE_WO_GET_CLASS(node)->clone(node);
		priv = moo_node_wo_get_instance_private(new_node);
		priv->version++;
		priv->wo_info->editing_parent_ref = cat_weak_reference_new((GObject *) editing_parent);
		return new_node;
	}
	return cat_ref_ptr(node);
}

MooNodeWo *moo_node_wo_get_original(MooNodeWo *node) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	if (priv->wo_info) {
		if (priv->wo_info->original) {
			return priv->wo_info->original;
		}
	}
	return node;
}

MooNodeWo *moo_node_wo_get_parent_node_ref(MooNodeWo *e_node) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(e_node);
	if (priv->wo_info && priv->wo_info->editing_parent_ref) {
		return (MooNodeWo *) cat_weak_reference_get(priv->wo_info->editing_parent_ref);
	}
	return NULL;
}



gboolean moo_node_wo_is_fixed(MooNodeWo *node) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	return priv->wo_info == NULL;
}

MooNodeWo *moo_node_wo_anchor(MooNodeWo *node, int version) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	cat_log_debug("anchoring node:%o", node);
	if (priv->wo_info == NULL) {
		cat_log_debug("wo_info was NULL");
		return node;
	}

	priv->children = moo_node_list_wo_anchor(priv->children, version);
	priv->content_map = moo_content_map_wo_anchor(priv->content_map, version);
	gboolean was_modified = TRUE;
	if (priv->wo_info->original) {
		MooNodeWoPrivate *opriv = moo_node_wo_get_instance_private(priv->wo_info->original);

		was_modified = !(cat_string_wo_equal(priv->name, opriv->name)
				&& (priv->zorder == opriv->zorder)
				&& (priv->children == opriv->children)
				&& (priv->node_expander == opriv->node_expander)
				&& (priv->content_map == opriv->content_map));
	}


	if (was_modified) {
		/* return edited version */
		cat_unref_ptr(priv->wo_info->original);
		cat_unref_ptr(priv->wo_info->editing_parent_ref);
		cat_free_ptr(priv->wo_info);
		priv->version = version;
		cat_log_debug("Returning edited as anchored:%o", node);
		return node;
	}
	/* return original, release edited version */
	MooNodeWo *result = priv->wo_info->original;
	priv->wo_info->original = NULL;
	cat_unref_ptr(node);
	cat_log_debug("Returning original as anchored:%o", result);
	return result;
}

gboolean moo_node_wo_equal(MooNodeWo *node_a, MooNodeWo *node_b) {
	if (node_a==node_b) {
		return TRUE;
	}
	if (node_a==NULL || node_b==NULL) {
		return FALSE;
	}
	MooNodeWoPrivate *priv_a = moo_node_wo_get_instance_private(node_a);
	MooNodeWoPrivate *priv_b = moo_node_wo_get_instance_private(node_b);
	if ((priv_a->unique_id==priv_b->unique_id)
			&& (cat_string_wo_equal(priv_a->name, priv_b->name))
			&& moo_node_list_wo_equal(priv_a->children, priv_b->children)
			&& moo_content_map_wo_equal(priv_a->content_map, priv_b->content_map)) {
		return TRUE;
	}
	return FALSE;
}



MooINodeExpander *moo_node_wo_get_expander(MooNodeWo *node) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	return priv->node_expander;
}


void moo_node_wo_set_expander(MooNodeWo *node, MooINodeExpander *expander) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	CHECK_IF_WRITABLE()
	cat_ref_swap(priv->node_expander, expander);
}


void moo_node_wo_set_children(MooNodeWo *node, MooNodeListWo *new_children) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	CHECK_IF_WRITABLE()
	cat_ref_swap(priv->children, new_children);
}

MooContentMapWo *moo_node_wo_get_content_map(MooNodeWo *node) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	return priv->content_map;
}

MooIContent *moo_node_wo_get_content(MooNodeWo *node, CatStringWo *key) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	return moo_content_map_wo_get(priv->content_map, key);
}


gboolean moo_node_wo_set_content(MooNodeWo *node, MooIContent *new_content) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	CHECK_IF_WRITABLE(FALSE)

	CatStringWo *key = moo_icontent_get_key(new_content);
	GObject *raw_entry = moo_content_map_wo_get_raw(priv->content_map, key);
	if (raw_entry == (GObject *) new_content) {
		return FALSE;
	}

	if (moo_content_map_wo_is_fixed(priv->content_map)) {
		MooContentMapWo *wcontent_map = moo_content_map_wo_ensure_editable(priv->content_map);
		cat_unref(priv->content_map);
		priv->content_map = wcontent_map;
	}

	moo_content_map_wo_set(priv->content_map, new_content);
	return TRUE;
}


MooNodeWo *moo_node_wo_get_editable_child_at(MooNodeWo *node, int index) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	CHECK_IF_WRITABLE(NULL)
	if (moo_node_list_wo_is_fixed(priv->children)) {
		MooNodeListWo *wchildren = moo_node_list_wo_ensure_editable(priv->children);
		cat_unref(priv->children);
		priv->children = wchildren;
	}

	MooNodeWo *child = moo_node_list_wo_get_at(priv->children, index);
	if (moo_node_wo_is_fixed(child)) {
		MooNodeWo *child_editable = moo_node_wo_ensure_editable(child, node);
		moo_node_list_wo_set_at(priv->children, child_editable, index);
		cat_unref(child_editable);
		return child_editable;
	}
	return child;
}

void moo_node_wo_insert_child_at(MooNodeWo *node, int index, MooNodeWo *child) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	CHECK_IF_WRITABLE()
	if (moo_node_list_wo_is_fixed(priv->children)) {
		MooNodeListWo *wchildren = moo_node_list_wo_ensure_editable(priv->children);
		cat_unref(priv->children);
		priv->children = wchildren;
	}

	moo_node_list_wo_insert(priv->children, index, child);
	MooNodeWoPrivate *cpriv = moo_node_wo_get_instance_private(child);
	if (cpriv->wo_info) {
		CatWeakReference *new_ref = cat_weak_reference_new((GObject *) node);
		cat_ref_swap(cpriv->wo_info->editing_parent_ref, new_ref);
		cat_unref_ptr(new_ref);
	}
}


void moo_node_wo_append_child(MooNodeWo *node, MooNodeWo *child) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	CHECK_IF_WRITABLE()
	if (moo_node_list_wo_is_fixed(priv->children)) {
		MooNodeListWo *wchildren = moo_node_list_wo_ensure_editable(priv->children);
		cat_unref(priv->children);
		priv->children = wchildren;
	}

	moo_node_list_wo_append(priv->children, child);
	MooNodeWoPrivate *cpriv = moo_node_wo_get_instance_private(child);
	if (cpriv->wo_info) {
		CatWeakReference *new_ref = cat_weak_reference_new((GObject *) node);
		cat_ref_swap(cpriv->wo_info->editing_parent_ref, new_ref);
		cat_unref_ptr(new_ref);
	}
}


MooNodeListWo *moo_node_wo_get_editable_children(MooNodeWo *node) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	CHECK_IF_WRITABLE(NULL)
	if (moo_node_list_wo_is_fixed(priv->children)) {
		MooNodeListWo *wchildren = moo_node_list_wo_ensure_editable(priv->children);
		cat_unref(priv->children);
		priv->children = wchildren;
	}
	return priv->children;
}


MooNodeListWo *moo_node_wo_get_children(MooNodeWo *node) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	return priv->children;
}


CatArrayWo *moo_node_wo_enlist_children(MooNodeWo *node, CatArrayWo *e_children) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	return moo_node_list_wo_enlist(priv->children, e_children);
}


int moo_node_wo_child_count(MooNodeWo *node) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	if (priv->children) {
		return moo_node_list_wo_count(priv->children);
	}
	return -1;
}

MooNodeWo *moo_node_wo_child_at(MooNodeWo *node, int index) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	if (priv->children) {
		return moo_node_list_wo_get_at(priv->children, index);
	}
	return NULL;
}

int moo_node_wo_find_index(MooNodeWo *node, MooINodeMatcher *nodeMatcher, gpointer userdata, int cache_index) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	int result = -1;
	if (priv->children) {
		int idx;
		MooINodeMatcherInterface *matcher_iface = MOO_INODE_MATCHER_GET_INTERFACE(nodeMatcher);
		int child_count = moo_node_list_wo_count(priv->children);
		if (cache_index>=0 && cache_index<child_count) {
			MooNodeWo *child = moo_node_list_wo_get_at(priv->children, cache_index);
			if (matcher_iface->matches(nodeMatcher, child, userdata)) {
				result = cache_index;
			}
		}

		if (result == -1) {
			for(idx=child_count-1; idx>=0; idx--) {
				MooNodeWo *child = moo_node_list_wo_get_at(priv->children, idx);
				if (matcher_iface->matches(nodeMatcher, child, userdata)) {
					result = idx;
					break;
				}
			}
		}
	}
	return result;
}


struct _MooIdPath *moo_node_wo_find_path(MooNodeWo *node, long long unique_id) {
	MooIdPath *result = NULL;
	CatArrayWo *e_stack = cat_array_wo_new();
	MooIdPath *path = moo_id_path_new();
	cat_ref_sink_ptr(path);
	cat_array_wo_append(e_stack, (GObject *) node);
	cat_array_wo_append(e_stack, (GObject *) path);
	while(cat_array_wo_size(e_stack)!=0) {
		MooIdPath *child_path = NULL;
		MooNodeWo *child_node = NULL;
		cat_array_wo_remove_last(e_stack, (GObject **) (&child_path));
		cat_array_wo_remove_last(e_stack, (GObject **) (&child_node));
		if (moo_node_wo_get_unique_id(child_node)==unique_id) {
			result = child_path;
			cat_unref_ptr(child_node);
			break;
		}
		int sub_count;
		for(sub_count = moo_node_wo_child_count(child_node)-1; sub_count>=0; sub_count--) {

			MooNodeWo *sub_node = moo_node_wo_child_at(child_node, sub_count);
			long long sub_unique_id = moo_node_wo_get_unique_id(sub_node);
			MooIdPath *sub_path = moo_id_path_append_id(child_path, sub_unique_id, sub_count);
			cat_ref_sink_ptr(sub_path);
			if (sub_unique_id==unique_id) {
				result = sub_path;
				break;
			}
			cat_array_wo_append(e_stack, (GObject *) sub_node);
			cat_array_wo_append(e_stack, (GObject *) sub_path);
			cat_unref_ptr(sub_path);
		}
		cat_unref_ptr(child_path);
		cat_unref_ptr(child_node);
	}
	cat_unref_ptr(e_stack);
	return result;
}



long long moo_node_wo_get_unique_id(MooNodeWo *node) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	return priv->unique_id;
}


int moo_node_wo_get_z_order(MooNodeWo *node) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	return priv->zorder;
}

void moo_node_wo_set_z_order(MooNodeWo *node, int new_z) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	CHECK_IF_WRITABLE()
	priv->zorder = new_z;
}


int moo_node_wo_get_version(MooNodeWo *node) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	return priv->version;
}

CatStringWo *moo_node_wo_get_name(MooNodeWo *node) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	return priv->name;
}

gboolean moo_node_wo_set_name(MooNodeWo *node, CatStringWo *new_name) {
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(node);
	if (priv->wo_info==NULL) {
		cat_log_error("Object is read only:%o", node);
		return FALSE;
	}
	gboolean result = !cat_string_wo_equal(priv->name, new_name);
	cat_ref_sink_swap(priv->name, new_name);
	return result;
}


gboolean moo_node_wo_equals(MooNodeWo *node_a, MooNodeWo *node_b) {
	return node_a == node_b;
}

int moo_node_wo_hash_code(MooNodeWo *node) {
	return (int) (long) node;
}


static void l_print_indented(MooNodeWo *node, CatStringWo *e_buf, int level) {
	int idx;
	for(idx=level; idx>=0; idx--) {
		cat_string_wo_append_chars(e_buf, "  ");
	}
	cat_istringable_print((CatIStringable *) node, e_buf);
	cat_string_wo_append_chars(e_buf, "\n");
	MooNodeListWo *children = moo_node_wo_get_children(node);
	if (children) {
		CatIIterator *iter = moo_node_list_wo_iterator(children);
		level++;
		while(cat_iiterator_has_next(iter)) {
			MooNodeWo *child = (MooNodeWo *) cat_iiterator_next(iter);
			l_print_indented(child, e_buf, level);
		}
		cat_unref_ptr(iter);
	}
}

void moo_node_wo_print(MooNodeWo *node, CatStringWo *buf) {
	l_print_indented(node, buf, 0);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	MooNodeWo *instance = MOO_NODE_WO(self);
	MooNodeWoPrivate *priv = moo_node_wo_get_instance_private(instance);
	MooNodeWo *node_wo = MOO_NODE_WO(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p:version=%d,id=%ld, name=%o,count=%d, content-map=%o, %s]", iname, self, priv->version, moo_node_wo_get_unique_id(node_wo), moo_node_wo_get_name(node_wo), moo_node_wo_child_count(node_wo), priv->content_map, priv->wo_info ? "editable" : "anchored");
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
