/*
 * catregexpnoderepeater.c
 *
 *  Created on: Jun 24, 2010
 */

#include "catregexpnoderepeater.h"
#include "../woo/catstringwo.h"
#include "catbalancedcounter.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatRegexpNodeRepeater"
#include "../logging/catlog.h"

static void l_dispose(GObject *object);
static gboolean l_matches(CatRegexpNode *self, CatStringWo *a_text, int offset, int length);
static int l_length(CatRegexpNode *self);
static void l_dump(CatRegexpNode *self, int indent_level, gboolean recurse);

G_DEFINE_TYPE(CatRegexpNodeRepeater, cat_regexp_node_repeater, CAT_TYPE_REGEXP_NODE)

static void cat_regexp_node_repeater_class_init(CatRegexpNodeRepeaterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;

	CatRegexpNodeClass *node_class = CAT_REGEXP_NODE_CLASS(clazz);
	node_class->dump = l_dump;
	node_class->matches = l_matches;
	node_class->length = l_length;
}

static void cat_regexp_node_repeater_init(CatRegexpNodeRepeater *obj) {
}

static void l_dispose(GObject *object) {
	CatRegexpNodeRepeater *instance = CAT_REGEXP_NODE_REPEATER(object);
	cat_unref_ptr(instance->child);
	G_OBJECT_CLASS(cat_regexp_node_repeater_parent_class)->dispose(object);
}

CatRegexpNodeRepeater *cat_regexp_node_repeater_new(CatRegexpNode *child) {
	CatRegexpNodeRepeater *regexp = g_object_new(CAT_TYPE_REGEXP_NODE_REPEATER, NULL);
	cat_ref_anounce(regexp);
	regexp->child = cat_ref_ptr(child);
	return regexp;
}

static gboolean l_matches(CatRegexpNode *self, CatStringWo *a_text, int offset, int length) {
	CatRegexpNodeRepeater *node_repeater = (CatRegexpNodeRepeater *) self;
	cat_log_debug("node_repeater_matches: offset=%d, length=%d", offset, length);
	if (length==0) {
		return TRUE;
	}
	CatRegexpNode *child = node_repeater->child;
	int clen = cat_regexp_node_length(child);
	if (clen>=0) {
		// If the child has a static length then the faster algorithm below is used
		if ((length % clen)!=0) {
			return FALSE;
		}

		while(TRUE) {
			if (length<=0) {
				return TRUE;
			}
			if (cat_regexp_node_matches(child, a_text, offset, clen)) {
				offset+=clen;
				length-=clen;
			} else {
				return FALSE;
			}
		}
	} else {
		// the slower algorithm
		CatBalancedCounter *bc = cat_balanced_counter_new(length, length);
		gboolean still_matches = TRUE;
		while(still_matches && cat_balanced_counter_next(bc)) {
			still_matches = TRUE;
			int idx;
			int loc_offset = offset;
			int loc_len;
			for(idx=0; still_matches && idx<length; idx++) {
				loc_len = bc->digits[idx];
				if (!cat_regexp_node_matches(child, a_text, loc_offset, loc_len)) {
					still_matches = FALSE;
				}
				loc_offset+=loc_len;
			}
		}
		cat_unref(bc);
		return still_matches;
	}
	return FALSE;
}

static int l_length(CatRegexpNode *self) {
	return -1;
}


static void l_dump(CatRegexpNode *self, int indent_level, gboolean recurse) {
	CatStringWo *e_buffer = cat_string_wo_new();
	int indent = indent_level*3;
	for(;indent>=0; indent--) {
		cat_string_wo_append_char(e_buffer, ' ');
	}
	cat_string_wo_append_chars(e_buffer, g_type_name_from_instance((GTypeInstance *) self));
	const char *buf_chrs = cat_string_wo_getchars(e_buffer);
	printf("%s[%p]\n", buf_chrs, self);
	indent_level++;

	if (recurse) {
		CatRegexpNodeRepeater *node_repeater = (CatRegexpNodeRepeater *) self;
		cat_regexp_node_dump(node_repeater->child, indent_level, recurse);
	}
}
