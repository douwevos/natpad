/*
   File:    cattreeentry.c
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


#include "cattreeentry.h"
#include "../../catistringable.h"
#include "../../woo/catstringwo.h"

#include <string.h>

#include "../../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatTreeEntry"
#include "../../logging/catlog.h"

struct _CatTreeEntryPrivate {
	int parent_location;
	int *children;
	int children_count;
	GObject *payload;
	gboolean marked_fixed;
};


static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatTreeEntry, cat_tree_entry, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_tree_entry_class_init(CatTreeEntryClass *clazz) {
	g_type_class_add_private(clazz, sizeof(CatTreeEntryPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_tree_entry_init(CatTreeEntry *instance) {
	CatTreeEntryPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, CAT_TYPE_TREE_ENTRY, CatTreeEntryPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatTreeEntry *instance = CAT_TREE_ENTRY(object);
	CatTreeEntryPrivate *priv = instance->priv;
	cat_free_ptr(priv->children);
	cat_unref_ptr(priv->payload);
	G_OBJECT_CLASS(cat_tree_entry_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_tree_entry_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatTreeEntry *cat_tree_entry_new() {
	CatTreeEntry *result = g_object_new(CAT_TYPE_TREE_ENTRY, NULL);
	cat_ref_anounce(result);
	CatTreeEntryPrivate *priv = result->priv;
	priv->children = NULL;
	priv->children_count = 0;
	priv->payload = NULL;
	priv->marked_fixed = FALSE;
	priv->parent_location = -1;
	return result;
}


void cat_tree_entry_mark(CatTreeEntry *entry) {
	CatTreeEntryPrivate *priv = CAT_TREE_ENTRY_GET_PRIVATE(entry);
	priv->marked_fixed = TRUE;
}

gboolean cat_tree_entry_is_marked_fixed(CatTreeEntry *entry) {
	return CAT_TREE_ENTRY_GET_PRIVATE(entry)->marked_fixed;
}

CatTreeEntry *cat_tree_entry_ensure_writable(CatTreeEntry *entry) {
	CatTreeEntryPrivate *priv = CAT_TREE_ENTRY_GET_PRIVATE(entry);
	if (!priv->marked_fixed) {
		return cat_ref_ptr(entry);
	}
	CatTreeEntry *result = cat_tree_entry_new();
	CatTreeEntryPrivate *rpriv = CAT_TREE_ENTRY_GET_PRIVATE(result);
	rpriv->parent_location = priv->parent_location;
	if (priv->children!=NULL) {
		rpriv->children = g_new(int, priv->children_count);
		rpriv->children_count = priv->children_count;
		memcpy(rpriv->children, priv->children, priv->children_count*sizeof(int));
	} else {
		rpriv->children = NULL;
		rpriv->children_count = 0;
	}
	rpriv->payload = cat_ref_ptr(priv->payload);
	return result;
}

void cat_tree_entry_set_parent_location(CatTreeEntry *entry, int location) {
	CatTreeEntryPrivate *priv = CAT_TREE_ENTRY_GET_PRIVATE(entry);
	priv->parent_location = location;
}

int cat_tree_entry_get_parent_location(CatTreeEntry *entry) {
	return CAT_TREE_ENTRY_GET_PRIVATE(entry)->parent_location;
}

int cat_tree_entry_find_child_index(CatTreeEntry *entry, int location) {
	CatTreeEntryPrivate *priv = CAT_TREE_ENTRY_GET_PRIVATE(entry);
	if (priv->children==NULL) {
		return -1;
	}
	int idx;
	for(idx=0 ; idx<priv->children_count; idx++) {
		if (priv->children[idx]==location) {
			return idx;
		}
	}
	return -1;
}


void cat_tree_entry_add_child(CatTreeEntry *entry, int location) {
	CatTreeEntryPrivate *priv = CAT_TREE_ENTRY_GET_PRIVATE(entry);
	if (priv->children==NULL) {
		priv->children = g_new(int, 1);
		priv->children_count = 1;
		priv->children[0] = location;
	} else {
		int *new_children = g_new(int, priv->children_count+1);
		memcpy(new_children, priv->children, priv->children_count*sizeof(int));
		new_children[priv->children_count] = location;
		priv->children_count++;
		cat_free_ptr(priv->children);
		priv->children = new_children;
	}
}

void cat_tree_entry_set_child_at(CatTreeEntry *entry, int index, int location) {
	CatTreeEntryPrivate *priv = CAT_TREE_ENTRY_GET_PRIVATE(entry);
	if (index>=0 && index<priv->children_count) {
		priv->children[index] = location;
	} else {
		cat_log_error("index (%d) out of bounds:", index);
	}
}

int cat_tree_entry_removed_child_at(CatTreeEntry *entry, int index) {
	CatTreeEntryPrivate *priv = CAT_TREE_ENTRY_GET_PRIVATE(entry);
	int result = priv->children[index];
	if (priv->children_count==1) {
		// TODO what if index not 0
		cat_free_ptr(priv->children);
		priv->children_count = 0;
	} else {
		int *new_children = g_new(int, priv->children_count+1);
		if (index>0) {
			memcpy(new_children, priv->children, index*sizeof(int));
		}
		index++;
		int rest = priv->children_count-index;
		if (rest>0) {
			memcpy(new_children+(index-1), priv->children+index, rest*sizeof(int));
		}
		priv->children_count--;
		cat_free_ptr(priv->children);
		priv->children = new_children;
	}
	return result;
}


int cat_tree_entry_get_child(CatTreeEntry *entry, int index) {
	CatTreeEntryPrivate *priv = CAT_TREE_ENTRY_GET_PRIVATE(entry);
	return priv->children[index];
}

int cat_tree_entry_count(CatTreeEntry *entry) {
	CatTreeEntryPrivate *priv = CAT_TREE_ENTRY_GET_PRIVATE(entry);
	return priv->children_count;
}



void cat_tree_entry_set_payload(CatTreeEntry *entry, GObject *payload) {
	CatTreeEntryPrivate *priv = CAT_TREE_ENTRY_GET_PRIVATE(entry);
	cat_ref_swap(priv->payload, payload);
}

GObject *cat_tree_entry_get_payload(CatTreeEntry *entry) {
	return CAT_TREE_ENTRY_GET_PRIVATE(entry)->payload;
}

gboolean cat_tree_entry_references_location(CatTreeEntry *entry, int location) {
	CatTreeEntryPrivate *priv = CAT_TREE_ENTRY_GET_PRIVATE(entry);
	if (priv->parent_location == location) {
		return TRUE;
	}
	if (priv->children!=NULL) {
		int idx;
		for(idx=0 ; idx<priv->children_count ; idx++) {
			if (priv->children[idx]==location) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

void cat_tree_entry_writable_move_location(CatTreeEntry *entry, int src_location, int dest_location) {
	CatTreeEntryPrivate *priv = CAT_TREE_ENTRY_GET_PRIVATE(entry);
	if (priv->parent_location==src_location) {
		priv->parent_location = dest_location;
	}

	if (priv->children!=NULL) {
		int idx;
		for(idx=0 ; idx<priv->children_count; idx++) {
			if (priv->children[idx]==src_location) {
				priv->children[idx] = dest_location;
			}
		}
	}
}






/********************* begin CatIStringable implementation *********************/

static void l_print(CatIStringable *self, struct _CatStringWo *append_to) {
	CatTreeEntryPrivate *priv = CAT_TREE_ENTRY_GET_PRIVATE(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
#if CAT_LOG_LEVEL<=CAT_LOG_DEBUG
	cat_string_wo_format(append_to, "%s[%p:payload=%o, marked_fixed=%d, parent-loc=%d", iname, self, priv->payload, priv->marked_fixed, priv->parent_location);
#else
	cat_string_wo_format(append_to, "%s[%p:payload=%p, marked_fixed=%d, parent-loc=%d", iname, self, priv->payload, priv->marked_fixed, priv->parent_location);
#endif
	if (priv->children_count>0) {
		cat_string_wo_append_chars(append_to, ", children");
		char sep = '=';
		int idx;
		for(idx=0; idx<priv->children_count; idx++) {
			cat_string_wo_append_char(append_to, sep);
			sep = ';';
			cat_string_wo_append_decimal(append_to, priv->children[idx]);
		}
	}
	cat_string_wo_append_char(append_to, ']');
}


static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_print;
}


/********************* end CatIStringable implementation *********************/


