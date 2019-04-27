/*
   File:    cattreeentrylist.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 4, 2014
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


#include "cattreeentrylist.h"
#include "cattreeblock.h"
#include "../../woo/catarraywo.h"

#include "../../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatTreeEntryList"
#include "../../logging/catlog.h"

struct _CatTreeEntryListPrivate {
	CatArrayWo *o_blocks;
	int block_edit_index;
	CatTreeBlock *block_edit;
	gboolean marked_fixed;
};


G_DEFINE_TYPE_WITH_PRIVATE(CatTreeEntryList, cat_tree_entry_list, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cat_tree_entry_list_class_init(CatTreeEntryListClass *clazz) {

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cat_tree_entry_list_init(CatTreeEntryList *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	CatTreeEntryList *instance = CAT_TREE_ENTRY_LIST(object);
	CatTreeEntryListPrivate *priv = cat_tree_entry_list_get_instance_private(instance);
	cat_unref_ptr(priv->o_blocks);
	cat_unref_ptr(priv->block_edit);
	G_OBJECT_CLASS(cat_tree_entry_list_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_tree_entry_list_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


CatTreeEntryList *cat_tree_entry_list_new() {
	CatTreeEntryList *result = g_object_new(CAT_TYPE_TREE_ENTRY_LIST, NULL);
	cat_ref_anounce(result);
	CatTreeEntryListPrivate *priv = cat_tree_entry_list_get_instance_private(result);
	priv->o_blocks = cat_array_wo_new();
	CatTreeBlock *initial_block = cat_tree_block_new();
	cat_tree_block_create_node(initial_block);
	cat_array_wo_append(priv->o_blocks, (GObject *) initial_block);
	cat_unref_ptr(initial_block);
	priv->block_edit = NULL;
	priv->block_edit_index = -1;
	priv->marked_fixed = FALSE;
	return result;
}


void cat_tree_entry_list_mark(CatTreeEntryList *entry_list) {
	CatTreeEntryListPrivate *priv = cat_tree_entry_list_get_instance_private(entry_list);
	if (priv->marked_fixed) {
		return;
	}
	priv->marked_fixed = TRUE;
	if (!cat_array_wo_is_anchored(priv->o_blocks)) {
		CatIIterator *iter = cat_array_wo_iterator(priv->o_blocks);
		while(cat_iiterator_has_next(iter)) {
			CatTreeBlock *block = (CatTreeBlock *) cat_iiterator_next(iter);
			cat_log_trace("marking block:%o", block);
			cat_tree_block_mark(block);
		}
		cat_unref_ptr(iter);
		priv->o_blocks = cat_array_wo_anchor(priv->o_blocks, 0);
	} else {
		if (priv->block_edit) {
			cat_log_trace("marking block-edit:%o", priv->block_edit);
			cat_tree_block_mark(priv->block_edit);
		}
	}
}


gboolean cat_tree_entry_list_is_marked_fixed(CatTreeEntryList *entry_list) {
	CatTreeEntryListPrivate *priv = cat_tree_entry_list_get_instance_private(entry_list);
	return priv->marked_fixed;
}

CatTreeEntryList *cat_tree_entry_list_ensure_writable(CatTreeEntryList *entry_list) {
	CatTreeEntryListPrivate *priv = cat_tree_entry_list_get_instance_private(entry_list);
	if (!priv->marked_fixed) {
		return cat_ref_ptr(entry_list);
	}

	CatTreeEntryList *result = g_object_new(CAT_TYPE_TREE_ENTRY_LIST, NULL);
	cat_ref_anounce(result);
	CatTreeEntryListPrivate *rpriv = cat_tree_entry_list_get_instance_private(result);
	rpriv->o_blocks = cat_ref_ptr(priv->o_blocks);
	rpriv->block_edit = cat_ref_ptr(priv->block_edit);
	rpriv->block_edit_index = priv->block_edit_index;
	rpriv->marked_fixed = FALSE;
	return result;
}


int cat_tree_entry_list_create_node(CatTreeEntryList *entry_list) {
	CatTreeEntryListPrivate *priv = cat_tree_entry_list_get_instance_private(entry_list);

	if (priv->marked_fixed) {
		cat_log_fatal("requesting create node on a fixed list");
		return -1;
	}

	/* first look at block_edit */
	if (priv->block_edit) {
		cat_log_trace("looking in block-edit:%o", priv->block_edit);

		if (cat_tree_block_has_available_spot(priv->block_edit)) {
			CatTreeBlock *block = priv->block_edit;
			if (cat_tree_block_is_marked_fixed(block)) {
				cat_log_trace("converting in writable block:%o", priv->block_edit);
				block = cat_tree_block_ensure_writable(block);
				cat_unref(priv->block_edit);
				priv->block_edit = block;
			}
			int found = cat_tree_block_create_node(block);
			cat_log_trace("new-index in edit_block:%d", found);
			if (found!=-1) {
				found = (priv->block_edit_index<<CAT_TREE_BLOCK_SHIFT) + found;
				return found;
			}

		}
	}

	/* browse through blocks and see if there is some spot available */
	int found = -1;
	int idx;
	for(idx=0; idx<cat_array_wo_size(priv->o_blocks); idx++) {
		if (idx==priv->block_edit_index) {
			continue;	/* skip edited block */
		}
		CatTreeBlock *block = (CatTreeBlock *) cat_array_wo_get(priv->o_blocks, idx);
		cat_log_trace("testing block[%d]:%o", idx, block);
		if (cat_tree_block_has_available_spot(block)) {
			if (cat_tree_block_is_marked_fixed(block)) {
				block = cat_tree_block_ensure_writable(block);
				cat_log_trace("converted to writable-block:%o", block);

				if (cat_array_wo_is_anchored(priv->o_blocks)) {
					cat_log_trace("trying to store block:%o, block_edit=%o", block, priv->block_edit);
					if (priv->block_edit==NULL) {
						/* create an edit block*/
						priv->block_edit = block;
						priv->block_edit_index = idx;
					} else {
						/* create a whole new copy of priv->blocks into a new CatArray */
						CatArrayWo *e_replacement = cat_array_wo_new();
						cat_array_wo_append_all(e_replacement, priv->o_blocks);
						if (priv->block_edit) {
							if (cat_array_wo_size(e_replacement)<=priv->block_edit_index) {
								cat_array_wo_append(e_replacement, (GObject *) priv->block_edit);
							} else {
								cat_array_wo_set(e_replacement, (GObject *) priv->block_edit, priv->block_edit_index, NULL);
							}
							cat_unref_ptr(priv->block_edit);
							priv->block_edit_index = -1;
						}
						cat_ref_swap(priv->o_blocks, e_replacement);
						cat_unref_ptr(e_replacement);
						cat_log_trace("blocks replaced with new array");

					}
				}
				if (!cat_array_wo_is_anchored(priv->o_blocks)) {
					if (cat_array_wo_size(priv->o_blocks)<=idx) {
						cat_array_wo_append(priv->o_blocks, (GObject *) block);
					} else {
						cat_array_wo_set(priv->o_blocks, (GObject *) block, idx, NULL);
					}
					cat_unref(block);
				}
			}
			found = cat_tree_block_create_node(block);
			cat_log_trace("new node at %d in block:%o", found, block);
			if (found!=-1) {
				found = (idx<<CAT_TREE_BLOCK_SHIFT) + found;
				return found;
			}
		}
	}

	cat_log_trace("appending new block");

	idx = cat_array_wo_size(priv->o_blocks);
	CatTreeBlock *block = cat_tree_block_new();
	if (cat_array_wo_is_anchored(priv->o_blocks)) {
		if (priv->block_edit==NULL) {
			/* create an edit block*/
			priv->block_edit = block;
			priv->block_edit_index = idx;
		} else {
			/* create a whole new copy of priv->blocks into a new CatArray */
			CatArrayWo *e_replacement = cat_array_wo_new();
			cat_array_wo_append_all(e_replacement, priv->o_blocks);
			if (priv->block_edit) {
				if (cat_array_wo_size(e_replacement)<=priv->block_edit_index) {
					cat_array_wo_append(e_replacement, (GObject *) priv->block_edit);
				} else {
					cat_array_wo_set(e_replacement, (GObject *) priv->block_edit, priv->block_edit_index, NULL);
				}
				cat_unref_ptr(priv->block_edit);
				priv->block_edit_index = -1;
			}
			cat_ref_swap(priv->o_blocks, e_replacement);
			cat_unref_ptr(e_replacement);
		}
	}
	if (!cat_array_wo_is_anchored(priv->o_blocks)) {
		if (cat_array_wo_size(priv->o_blocks)<=idx) {
			cat_array_wo_append(priv->o_blocks, (GObject *) block);
		} else {
			cat_array_wo_set(priv->o_blocks, (GObject *) block, idx, NULL);
		}
		cat_unref(block);
	}


	found = cat_tree_block_create_node(block);
	cat_log_trace("new node at %d in block:%o", found, block);
	found = (idx<<CAT_TREE_BLOCK_SHIFT) + found;
	return found;
}


void cat_tree_entry_list_remove_entry(CatTreeEntryList *entry_list, int entry_location, gboolean recursive) {
	if (entry_location == -1 || entry_location == 0) {
		return;
	}
	CatTreeEntryListPrivate *priv = cat_tree_entry_list_get_instance_private(entry_list);
	if (priv->marked_fixed) {
		cat_log_fatal("requesting remove on a fixed list");
		return;
	}

	int block_index = entry_location>>CAT_TREE_BLOCK_SHIFT;
	int entry_index = entry_location % CAT_TREE_BLOCK_SIZE;


	CatTreeBlock *block = NULL;
	if (block_index==priv->block_edit_index) {
		block = priv->block_edit;
		if (cat_tree_block_is_marked_fixed(block)) {
			block = cat_tree_block_ensure_writable(block);
			cat_unref(priv->block_edit);
			priv->block_edit = block;
		}

	} else {
		block = (CatTreeBlock *) cat_array_wo_get(priv->o_blocks, block_index);
		if (block==NULL) {
			cat_log_error("block at %d is NULL", block_index);
			return;
		}
		if (cat_tree_block_is_marked_fixed(block)) {
			block = cat_tree_block_ensure_writable(block);
			if (cat_array_wo_is_anchored(priv->o_blocks)) {
				if (priv->block_edit==NULL) {
					/* create an edit block*/
					priv->block_edit = block;
					priv->block_edit_index = block_index;
				} else {
					/* create a whole new copy of priv->blocks into a new CatArray */
					CatArrayWo *e_replacement = cat_array_wo_new();
					cat_array_wo_append_all(e_replacement, priv->o_blocks);
					if (priv->block_edit) {
						if (cat_array_wo_size(e_replacement)<=priv->block_edit_index) {
							cat_array_wo_append(e_replacement, (GObject *) priv->block_edit);
						} else {
							cat_array_wo_set(e_replacement, (GObject *) priv->block_edit, priv->block_edit_index, NULL);
						}
						cat_unref_ptr(priv->block_edit);
						priv->block_edit_index = -1;
					}
					cat_ref_swap(priv->o_blocks, e_replacement);
					cat_unref_ptr(e_replacement);
				}
			}
			if (!cat_array_wo_is_anchored(priv->o_blocks)) {
				if (cat_array_wo_size(priv->o_blocks)<=block_index) {
					cat_array_wo_append(priv->o_blocks, (GObject *) block);
				} else {
					cat_array_wo_set(priv->o_blocks, (GObject *) block, block_index, NULL);
				}
				cat_unref(block);
			}
		}
	}

	if (recursive) {
		CatTreeEntry *entry = cat_tree_block_get(block, entry_index);
		int child_count = cat_tree_entry_count(entry);
		int idx;
		for(idx=child_count-1; idx>=0; idx--) {
			int location = cat_tree_entry_get_child(entry, idx);
			cat_tree_entry_list_remove_entry(entry_list, location, TRUE);
		}
	}
	cat_tree_block_set(block, entry_index, NULL);
}



CatTreeEntry *cat_tree_entry_list_get_entry(CatTreeEntryList *entry_list, int entry_location) {
	if (entry_location == -1) {
		return NULL;
	}
	CatTreeEntryListPrivate *priv = cat_tree_entry_list_get_instance_private(entry_list);
	int block_index = entry_location>>CAT_TREE_BLOCK_SHIFT;
	int entry_index = entry_location % CAT_TREE_BLOCK_SIZE;
	cat_log_trace("get entry:entry_list=%o, block_index=%d, entry_index=%d, loc=%d", entry_list, block_index, entry_index, entry_location);
	CatTreeBlock *block = NULL;
	if (block_index==priv->block_edit_index) {
		block = priv->block_edit;
	} else {
		block = (CatTreeBlock *) cat_array_wo_get(priv->o_blocks, block_index);
	}
	cat_log_trace("entry_list=%o, block=%o", entry_list, block);
	if (block == NULL) {
		return NULL;
	}
	return cat_tree_block_get(block, entry_index);
}


CatTreeEntry *cat_tree_entry_list_get_writable_entry(CatTreeEntryList *entry_list, int entry_location) {
	if (entry_location == -1) {
		return NULL;
	}

	CatTreeEntryListPrivate *priv = cat_tree_entry_list_get_instance_private(entry_list);
	if (priv->marked_fixed) {
		cat_log_fatal("requesting writable entry on a fixed list");
		return NULL;
	}

	int block_index = entry_location>>CAT_TREE_BLOCK_SHIFT;
	int entry_index = entry_location % CAT_TREE_BLOCK_SIZE;
	CatTreeBlock *block = NULL;
	if (block_index==priv->block_edit_index) {
		block = priv->block_edit;
		if (block!=NULL && cat_tree_block_is_marked_fixed(block)) {
			block = cat_tree_block_ensure_writable(block);
			cat_unref(priv->block_edit);
			priv->block_edit = block;
		}
	} else {
		block = (CatTreeBlock *) cat_array_wo_get(priv->o_blocks, block_index);
		if (block!=NULL && cat_tree_block_is_marked_fixed(block)) {
			block = cat_tree_block_ensure_writable(block);
			if (cat_array_wo_is_anchored(priv->o_blocks)) {
				if (priv->block_edit==NULL) {
					/* create an edit block*/
					priv->block_edit = block;
					priv->block_edit_index = block_index;
				} else {
					/* create a whole new copy of priv->blocks into a new CatArray */
					CatArrayWo *e_replacement = cat_array_wo_new();
					cat_array_wo_append_all(e_replacement, priv->o_blocks);
					if (priv->block_edit) {
						if (cat_array_wo_size(e_replacement)<=priv->block_edit_index) {
							cat_array_wo_append(e_replacement, (GObject *) priv->block_edit);
						} else {
							cat_array_wo_set(e_replacement, (GObject *) priv->block_edit, priv->block_edit_index, NULL);
						}
						cat_unref_ptr(priv->block_edit);
						priv->block_edit_index = -1;
					}
					cat_ref_swap(priv->o_blocks, e_replacement);
					cat_unref_ptr(e_replacement);
				}
			}
			if (!cat_array_wo_is_anchored(priv->o_blocks)) {
				if (cat_array_wo_size(priv->o_blocks)<=block_index) {
					cat_array_wo_append(priv->o_blocks, (GObject *) block);
				} else {
					cat_array_wo_set(priv->o_blocks, (GObject *) block, block_index, NULL);
				}
				cat_unref(block);
			}
		}
	}
	if (block == NULL) {
		return NULL;
	}
	CatTreeEntry *result = cat_tree_block_get_writable(block, entry_index);
	cat_log_debug("request writable: entry_list=%o, block=%o, entry_index=%d, result=%o", entry_list, block, entry_index, result);
	return result;
}




int cat_tree_entry_list_find_location(CatTreeEntryList *entry_list, CatIMatcher *matcher, int guess_location) {
	CatTreeEntryListPrivate *priv = cat_tree_entry_list_get_instance_private(entry_list);

	if (guess_location!=-1) {

		CatTreeEntry *entry = cat_tree_entry_list_get_entry(entry_list, guess_location);
		if (entry!=NULL) {
			if (cat_imatcher_matches(matcher, cat_tree_entry_get_payload(entry))) {
				return guess_location;
			}
		}
	}
	int block_count = cat_array_wo_size(priv->o_blocks);
	if (block_count<=priv->block_edit_index) {
		block_count = priv->block_edit_index+1;
	}
	int block_index;
	for(block_index=0; block_index<block_count; block_index++) {
		CatTreeBlock *block = NULL;
		if (block_index==priv->block_edit_index) {
			block = priv->block_edit;
		} else {
			block = (CatTreeBlock *) cat_array_wo_get(priv->o_blocks, block_index);
		}
		int location = cat_tree_block_find(block, matcher);
		if (location!=-1) {
			location = (block_index<<CAT_TREE_BLOCK_SHIFT) + location;
			return location;
		}
	}

	return -1;
}
