/*
   File:    moonodelistwo.c
   Project: moose
   Author:  Douwe Vos
   Date:    May 9, 2014
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

#include "moonodelistwo.h"
#include "moonodewo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooNodeListWo"
#include <logging/catlog.h>


struct _WoInfo {
	MooNodeListWo *original;
	CatArrayWo *original_list;
};

typedef struct _WoInfo WoInfo;


struct _MooNodeListWoPrivate {
	WoInfo *wo_info;
	int version;
	CatArrayWo *list;
};


G_DEFINE_TYPE(MooNodeListWo, moo_node_list_wo, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_node_list_wo_class_init(MooNodeListWoClass *clazz) {
	g_type_class_add_private(clazz, sizeof(MooNodeListWoPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_node_list_wo_init(MooNodeListWo *instance) {
	MooNodeListWoPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_NODE_LIST_WO, MooNodeListWoPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooNodeListWo *instance = MOO_NODE_LIST_WO(object);
	MooNodeListWoPrivate *priv = instance->priv;
	if (priv->wo_info) {
		cat_unref_ptr(priv->wo_info->original);
		cat_unref_ptr(priv->wo_info->original_list);
		cat_free_ptr(priv->wo_info);
	}
	cat_unref_ptr(priv->list);
	G_OBJECT_CLASS(moo_node_list_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_node_list_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooNodeListWo *moo_node_list_wo_new() {
	MooNodeListWo *result = g_object_new(MOO_TYPE_NODE_LIST_WO, NULL);
	cat_ref_anounce(result);
	MooNodeListWoPrivate *priv = result->priv;
	priv->wo_info = g_new0(WoInfo, 1);
	priv->version = 1;
	priv->list = cat_array_wo_new();
	return result;
}

MooNodeListWo *moo_node_list_wo_ensure_editable(MooNodeListWo *node_list) {
	MooNodeListWoPrivate *priv = MOO_NODE_LIST_WO_GET_PRIVATE(node_list);
	if (priv->wo_info==NULL) {
		MooNodeListWo *result = g_object_new(MOO_TYPE_NODE_LIST_WO, NULL);
		cat_ref_anounce(result);
		MooNodeListWoPrivate *spriv = result->priv;
		spriv->wo_info = g_new0(WoInfo, 1);
		spriv->wo_info->original = cat_ref_ptr(node_list);
		spriv->wo_info->original_list = cat_ref_ptr(priv->list);
		spriv->version = priv->version+1;
		spriv->list = cat_ref_ptr(priv->list);
		return result;
	}
	return cat_ref_ptr(node_list);
}

gboolean moo_node_list_wo_is_fixed(MooNodeListWo *node_list) {
	MooNodeListWoPrivate *priv = MOO_NODE_LIST_WO_GET_PRIVATE(node_list);
	return priv->wo_info == NULL;
}

MooNodeListWo *moo_node_list_wo_anchor(MooNodeListWo *node_list, int version) {
	MooNodeListWoPrivate *priv = MOO_NODE_LIST_WO_GET_PRIVATE(node_list);
	if (priv->wo_info == NULL) {
		return node_list;
	}

	if (priv->wo_info->original_list != priv->list) {

		/* anchor children */
		int count = cat_array_wo_size(priv->list);
		int idx;
		for(idx=count-1; idx>=0; idx--) {
			MooNodeWo *child = (MooNodeWo *) cat_array_wo_get(priv->list, idx);
			if (!moo_node_wo_is_fixed(child)) {
				cat_ref_ptr(child);
				MooNodeWo *replacement = moo_node_wo_anchor(child, version);
				cat_array_wo_set(priv->list, (GObject *) replacement, idx, NULL);
				cat_unref_ptr(replacement);
			}
		}

		if (!cat_array_wo_equal(priv->list, priv->wo_info->original_list, NULL)) {
			/* return the new version */
			priv->version = version;
			cat_unref_ptr(priv->wo_info->original_list);
			cat_unref_ptr(priv->wo_info->original);
			cat_free_ptr(priv->wo_info);
			return node_list;
		}
//	} else {
//		int count = cat_array_wo_size(priv->list);
//		int idx;
//		for(idx=count-1; idx>=0; idx--) {
//			MooNodeWo *child = (MooNodeWo *) cat_array_wo_get(priv->list, idx);
//			if (!moo_node_wo_is_fixed(child)) {
//
	}

	/* return original */
	MooNodeListWo *result = priv->wo_info->original;
	priv->wo_info->original = NULL;
	cat_unref_ptr(node_list);
	return result;
}

gboolean moo_node_list_wo_equal(MooNodeListWo *list_a, MooNodeListWo *list_b) {
	if (list_a==list_b) {
		return TRUE;
	}
	if (list_a==NULL || list_b==NULL) {
		return FALSE;
	}
	cat_log_fatal("NOT IMPLEMENTED");
	return FALSE;
}


CatArrayWo *moo_node_list_wo_enlist(MooNodeListWo *node_list, CatArrayWo *e_enlist_to) {
	MooNodeListWoPrivate *priv = MOO_NODE_LIST_WO_GET_PRIVATE(node_list);
	if (e_enlist_to==NULL) {
		e_enlist_to = cat_array_wo_new();
	}
	cat_array_wo_append_all(e_enlist_to, priv->list);
	return e_enlist_to;
}

struct _MooNodeWo *moo_node_list_wo_get_at(MooNodeListWo *node_list, int index) {
	return (MooNodeWo *) cat_array_wo_get(MOO_NODE_LIST_WO_GET_PRIVATE(node_list)->list, index);
}

struct _MooNodeWo *moo_node_list_wo_get_first(MooNodeListWo *node_list, int index) {
	return (MooNodeWo *) cat_array_wo_get_first(MOO_NODE_LIST_WO_GET_PRIVATE(node_list)->list);
}

struct _MooNodeWo *moo_node_list_wo_get_last(MooNodeListWo *node_list, int index) {
	return (MooNodeWo *) cat_array_wo_get_last(MOO_NODE_LIST_WO_GET_PRIVATE(node_list)->list);
}

int moo_node_list_wo_count(MooNodeListWo *node_list) {
	return cat_array_wo_size(MOO_NODE_LIST_WO_GET_PRIVATE(node_list)->list);
}


CatIIterator *moo_node_list_wo_iterator(MooNodeListWo *node_list) {
	return cat_array_wo_iterator(MOO_NODE_LIST_WO_GET_PRIVATE(node_list)->list);
}


#define CHECK_IF_WRITABLE \
		if (priv->wo_info==NULL) { \
			cat_log_error("Object is read only:%o", node_list); \
			return; \
		} \


#define CHECK_IF_WRITABLE_LIST(nlist, rval) \
		if (priv->wo_info==NULL) { \
			cat_log_error("Object is read only:%o", nlist); \
			return rval; \
		} \
		if (priv->wo_info->original_list == priv->list) { \
			cat_unref_ptr(priv->list); \
			priv->list = cat_array_wo_new(); \
			if (priv->wo_info->original_list) { \
				cat_array_wo_append_all(priv->list, priv->wo_info->original_list); \
			} \
		} \


void moo_node_list_wo_append(MooNodeListWo *node_list, struct _MooNodeWo *node) {
	MooNodeListWoPrivate *priv = MOO_NODE_LIST_WO_GET_PRIVATE(node_list);
	CHECK_IF_WRITABLE_LIST(node_list,)
	cat_array_wo_append(priv->list, (GObject *) node);
}

void moo_node_list_wo_insert(MooNodeListWo *node_list, int index, struct _MooNodeWo *node) {
	MooNodeListWoPrivate *priv = MOO_NODE_LIST_WO_GET_PRIVATE(node_list);
	CHECK_IF_WRITABLE_LIST(node_list,)
	cat_array_wo_insert(priv->list, (GObject *) node, index);
}


void moo_node_list_wo_remove(MooNodeListWo *node_list, int index) {
	MooNodeListWoPrivate *priv = MOO_NODE_LIST_WO_GET_PRIVATE(node_list);
	CHECK_IF_WRITABLE_LIST(node_list,)
	cat_array_wo_remove(priv->list, index, NULL);
}

void moo_node_list_wo_clear(MooNodeListWo *node_list) {
	MooNodeListWoPrivate *priv = MOO_NODE_LIST_WO_GET_PRIVATE(node_list);
	CHECK_IF_WRITABLE_LIST(node_list,)
	cat_array_wo_clear(priv->list);
}


gboolean moo_node_list_wo_set_at(MooNodeListWo *e_node_list, struct _MooNodeWo *node, int index) {
	MooNodeListWoPrivate *priv = MOO_NODE_LIST_WO_GET_PRIVATE(e_node_list);
	MooNodeWo *current = (MooNodeWo *) cat_array_wo_get(priv->list, index);
	if (current==node) {
		return FALSE;
	}
	CHECK_IF_WRITABLE_LIST(e_node_list, FALSE);
	cat_array_wo_set(priv->list, (GObject *) node, index, NULL);
	return TRUE;
}



