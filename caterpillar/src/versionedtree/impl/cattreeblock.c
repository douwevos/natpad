/*
   File:    cattreeblock.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 2, 2014
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


#include "cattreeblock.h"

#include "../../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatTreeBlock"
#include "../../logging/catlog.h"

struct _CatTreeBlockPrivate {
	CatTreeEntry **internal;
	int used;
	gboolean marked_fixed;
};


G_DEFINE_TYPE_WITH_PRIVATE(CatTreeBlock, cat_tree_block, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_tree_block_class_init(CatTreeBlockClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_tree_block_init(CatTreeBlock *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatTreeBlock *instance = CAT_TREE_BLOCK(object);
	CatTreeBlockPrivate *priv = cat_tree_block_get_instance_private(instance);
	if (priv->internal) {
		int idx;
		for(idx=0; idx<CAT_TREE_BLOCK_SIZE; idx++) {
			cat_unref_ptr(priv->internal[idx]);
		}
		cat_free_ptr(priv->internal);
	}
	G_OBJECT_CLASS(cat_tree_block_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_tree_block_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatTreeBlock *cat_tree_block_new() {
	CatTreeBlock *result = g_object_new(CAT_TYPE_TREE_BLOCK, NULL);
	cat_ref_anounce(result);
	CatTreeBlockPrivate *priv = cat_tree_block_get_instance_private(result);
	priv->internal = g_malloc0(sizeof(CatTreeEntry *) * CAT_TREE_BLOCK_SIZE);
	priv->used = 0;
	priv->marked_fixed = FALSE;
	return result;
}





void cat_tree_block_mark(CatTreeBlock *block) {
	CatTreeBlockPrivate *priv = cat_tree_block_get_instance_private(block);
	priv->marked_fixed = TRUE;
	int idx;
	for(idx=0; idx<CAT_TREE_BLOCK_SIZE; idx++) {
		CatTreeEntry *entry = priv->internal[idx];
		if (entry!=NULL) {
			cat_tree_entry_mark(entry);
		}
	}
}

gboolean cat_tree_block_is_marked_fixed(CatTreeBlock *block) {
	CatTreeBlockPrivate *priv = cat_tree_block_get_instance_private(block);
	return priv->marked_fixed;
}

CatTreeBlock *cat_tree_block_ensure_writable(CatTreeBlock *block) {
	CatTreeBlockPrivate *priv = cat_tree_block_get_instance_private(block);
	if (!priv->marked_fixed) {
		return cat_ref_ptr(block);
	}
	CatTreeBlock *result = cat_tree_block_new();
	cat_ref_anounce(result);
	CatTreeBlockPrivate *rpriv = cat_tree_block_get_instance_private(result);
	int idx;
	for(idx=0; idx<CAT_TREE_BLOCK_SIZE; idx++) {
		rpriv->internal[idx] = cat_ref_ptr(priv->internal[idx]);
	}
	rpriv->used = priv->used;
	return result;
}


void cat_tree_block_set(CatTreeBlock *block, int index, CatTreeEntry *node) {
	CatTreeBlockPrivate *priv = cat_tree_block_get_instance_private(block);
	if (priv->internal[index]!=NULL) {
		priv->used--;
	}
	cat_ref_swap(priv->internal[index], node);
	if (node!=NULL) {
		priv->used++;
	}
}


gboolean cat_tree_block_has_available_spot(CatTreeBlock *block) {
	CatTreeBlockPrivate *priv = cat_tree_block_get_instance_private(block);
	return priv->used != CAT_TREE_BLOCK_SIZE;
}

gboolean cat_tree_block_is_empty(CatTreeBlock *block) {
	CatTreeBlockPrivate *priv = cat_tree_block_get_instance_private(block);
	return priv->used == 0;
}

int cat_tree_block_count(CatTreeBlock *block) {
	CatTreeBlockPrivate *priv = cat_tree_block_get_instance_private(block);
	return priv->used;
}


int cat_tree_block_create_node(CatTreeBlock *block) {
	CatTreeBlockPrivate *priv = cat_tree_block_get_instance_private(block);
	if (priv->used == CAT_TREE_BLOCK_SIZE) {
		return -1;
	}
	int idx;
	for(idx=0; idx<CAT_TREE_BLOCK_SIZE; idx++) {
		if (priv->internal[idx]==NULL) {
			priv->internal[idx] = cat_tree_entry_new();
			priv->used++;
			return idx;
		}
	}
	return -1;
}

int cat_tree_block_find(CatTreeBlock *block, CatIMatcher *matcher) {
	CatTreeBlockPrivate *priv = cat_tree_block_get_instance_private(block);
	int idx;
	for(idx=0; idx<CAT_TREE_BLOCK_SIZE; idx++) {
		if (priv->internal[idx]!=NULL) {
			GObject *payload = cat_tree_entry_get_payload(priv->internal[idx]);
			if (cat_imatcher_matches(matcher, payload)) {
				return idx;
			}
		}
	}
	return -1;
}


CatTreeEntry *cat_tree_block_get(CatTreeBlock *block, int index) {
	CatTreeBlockPrivate *priv = cat_tree_block_get_instance_private(block);
	return priv->internal[index];
}

CatTreeEntry *cat_tree_block_get_writable(CatTreeBlock *block, int entry_index) {
	CatTreeBlockPrivate *priv = cat_tree_block_get_instance_private(block);
	CatTreeEntry *entry = priv->internal[entry_index];
	if (entry!=NULL && cat_tree_entry_is_marked_fixed(entry)) {
		entry = cat_tree_entry_ensure_writable(entry);
		cat_unref(priv->internal[entry_index]);
		priv->internal[entry_index] = entry;
	}
	return entry;
}

