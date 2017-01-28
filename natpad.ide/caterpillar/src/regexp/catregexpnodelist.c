/*
 * catregexpnodelist.c
 *
 *  Created on: Jun 24, 2010
 */

#include "catregexpnodelist.h"
#include "catbalancedcounter.h"
#include "../woo/catstringwo.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatRegexpNodeList"
#include "../logging/catlog.h"

static void l_dispose(GObject *object);
static gboolean l_matches(CatRegexpNode *self, CatStringWo *a_text, int offset, int length);
static int l_length(CatRegexpNode *self);
static void l_dump(CatRegexpNode *self, int indent_level, gboolean recurse);

G_DEFINE_TYPE(CatRegexpNodeList, cat_regexp_node_list, CAT_TYPE_REGEXP_NODE)

static void cat_regexp_node_list_class_init(CatRegexpNodeListClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;

	CatRegexpNodeClass *node_class = CAT_REGEXP_NODE_CLASS(clazz);
	node_class->dump = l_dump;
	node_class->matches = l_matches;
	node_class->length = l_length;
}

static void cat_regexp_node_list_init(CatRegexpNodeList *obj) {
}

static void l_dispose(GObject *object) {
	CatRegexpNodeList *instance = CAT_REGEXP_NODE_LIST(object);
	cat_unref_ptr(instance->e_list);
	G_OBJECT_CLASS(cat_regexp_node_list_parent_class)->dispose(object);
}

CatRegexpNodeList *cat_regexp_node_list_new() {
	CatRegexpNodeList *result = g_object_new(CAT_TYPE_REGEXP_NODE_LIST, NULL);
	cat_ref_anounce(result);
	result->e_list = cat_array_wo_new();
	return result;
}



void cat_regexp_node_list_add(CatRegexpNodeList *node_list, CatRegexpNode *to_add) {
	cat_array_wo_append(node_list->e_list, (GObject *) to_add);
}

CatRegexpNode *cat_regexp_node_list_get_last(CatRegexpNodeList *node_list) {
	if (cat_array_wo_size(node_list->e_list)==0) {
		return NULL;
	}
	return CAT_REGEXP_NODE(cat_array_wo_get_last(node_list->e_list));
}

void cat_regexp_node_list_replace_last(CatRegexpNodeList *node_list, CatRegexpNode *replace_node) {
	cat_array_wo_set(node_list->e_list, (GObject *) replace_node, cat_array_wo_size(node_list->e_list)-1, NULL);
}







static gboolean l_matches(CatRegexpNode *self, CatStringWo *a_text, int offset, int length) {
	cat_log_debug("node_list_matches: offset=%d, length=%d", offset, length);
	CatRegexpNodeList *node_list = (CatRegexpNodeList *) self;
	int recurse_len = cat_array_wo_size(node_list->e_list);
	int *recurse = (int *) g_malloc(sizeof(int)*recurse_len);
	int sum = 0;
	int nonStaticCount = 0;
	int idx;
	for(idx=0; idx<recurse_len; idx++) {
		CatRegexpNode *child = CAT_REGEXP_NODE(cat_array_wo_get(node_list->e_list, idx));
		recurse[idx] = cat_regexp_node_length(child);
		cat_log_trace("recurse[%d]=%d  child=%s", idx, recurse[idx], g_type_name_from_instance((GTypeInstance *) child));
		if (recurse[idx]<0) {
			nonStaticCount++;
			recurse[idx] = -nonStaticCount;
		} else {
			sum += recurse[idx];
		}
	}

	cat_log_debug("node_list_matches: sum=%d, length=%d", sum, length);
	if (sum>length) {
		g_free(recurse);
		return FALSE;
	}

	CatBalancedCounter *bc = cat_balanced_counter_new(nonStaticCount, length-sum);
	gboolean force_once = TRUE;
	while(cat_balanced_counter_next(bc) || force_once) {
		cat_log_on_trace({
			cat_balanced_counter_dump(bc);
		});
		force_once = FALSE;
		int loc_offset = offset;
		gboolean ok = TRUE;
		cat_log_debug("loc_offset=%d", loc_offset);
		for(idx=0; idx<recurse_len; idx++) {
			CatRegexpNode *child = CAT_REGEXP_NODE(cat_array_wo_get(node_list->e_list, idx));
			cat_log_debug("idx=%d, loc_offset=%d, child=%p", idx, loc_offset, child);
			if (recurse[idx]>0) {
				if (!cat_regexp_node_matches(child, a_text, loc_offset, recurse[idx])) {
					ok = FALSE;
					idx = recurse_len;
				} else {
					loc_offset += recurse[idx];
				}
			} else if (recurse[idx]<0) {
				int l = bc->digits[-recurse[idx]-1];
				if (!cat_regexp_node_matches(child, a_text, loc_offset, l)) {
					ok = FALSE;
					idx = recurse_len;
				}
				loc_offset += l;
			}
		}
		cat_log_debug("node_list_matches: ok=%d", ok);
		if (ok) {
			cat_unref(bc);
			g_free(recurse);
			return TRUE;
		}
	}
	g_free(recurse);
	cat_unref(bc);
	return FALSE;
}

static int l_length(CatRegexpNode *self) {
	CatRegexpNodeList *node_list = (CatRegexpNodeList *) self;
	int sum = 0;
	CatIIterator *iter = cat_array_wo_iterator(node_list->e_list);
	while(cat_iiterator_has_next(iter)) {
		CatRegexpNode *child = (CatRegexpNode *) cat_iiterator_next(iter);
		int l = cat_regexp_node_length(child);
		if (l<0) {
			return -1;
		}
		sum += l;
	}
	cat_unref_ptr(iter);
	return sum;
}

static void l_dump(CatRegexpNode *self, int indent_level, gboolean recurse) {
	CatStringWo *e_buffer = cat_string_wo_new();
	int indent = indent_level*3;
	for(;indent>=0; indent--) {
		cat_string_wo_append_char(e_buffer, ' ');
	}
	cat_string_wo_append_chars(e_buffer, g_type_name_from_instance((GTypeInstance *) self));
	printf("%s[%p]\n", cat_string_wo_getchars(e_buffer), self);
	indent_level++;

	if (recurse) {
		CatRegexpNodeList *node_list = (CatRegexpNodeList *) self;

		CatIIterator *iter = cat_array_wo_iterator(node_list->e_list);
		while(cat_iiterator_has_next(iter)) {
			CatRegexpNode *child = (CatRegexpNode *) cat_iiterator_next(iter);
			if (child) {
				cat_regexp_node_dump(child, indent_level, recurse);
			} else {
				printf("%s CHILD=NULL !!!\n", cat_string_wo_getchars(e_buffer));
			}
		}
		cat_unref_ptr(iter);
	}
	cat_unref_ptr(e_buffer);
}
