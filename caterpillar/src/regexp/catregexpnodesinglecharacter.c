/*
 * catregexpnodesinglecharacter.c
 *
 *  Created on: Jun 24, 2010
 */

#include "catregexpnodesinglecharacter.h"
#include "catbalancedcounter.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatRegexpNodeSingleCharacter"
#include "../logging/catlog.h"

static void l_dispose(GObject *object);
static gboolean l_matches(CatRegexpNode *self, CatStringWo *a_text, int offset, int length);
static int l_length(CatRegexpNode *self);
static void l_dump(CatRegexpNode *self, int indent_level, gboolean recurse);


G_DEFINE_TYPE(CatRegexpNodeSingleCharacter, cat_regexp_node_single_character, CAT_TYPE_REGEXP_NODE)

static void cat_regexp_node_single_character_class_init(CatRegexpNodeSingleCharacterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;

	CatRegexpNodeClass *node_class = CAT_REGEXP_NODE_CLASS(clazz);
	node_class->dump = l_dump;
	node_class->matches = l_matches;
	node_class->length = l_length;
}

static void cat_regexp_node_single_character_init(CatRegexpNodeSingleCharacter *obj) {
}

static void l_dispose(GObject *object) {
	CatRegexpNodeSingleCharacter *node = CAT_REGEXP_NODE_SINGLE_CHARACTER(object);
	cat_unref_ptr(node->a_chars);
}

CatRegexpNodeSingleCharacter *cat_regexp_node_single_character_new(CatStringWo *a_chars, gboolean inverted, gboolean fixed_length) {
	CatRegexpNodeSingleCharacter *result = g_object_new(CAT_TYPE_REGEXP_NODE_SINGLE_CHARACTER, NULL);
	cat_ref_anounce(result);
	result->a_chars = a_chars == NULL ? cat_string_wo_new() : cat_ref_ptr(a_chars);
	result->inverted = inverted;
	result->fixed_length = TRUE;
	return result;
}

static gboolean l_matches(CatRegexpNode *self, CatStringWo *a_text, int offset, int length) {
	cat_log_debug("offset=%d, length=%d", offset, length);
	CatRegexpNodeSingleCharacter *node_single = (CatRegexpNodeSingleCharacter *) self;
	if ((node_single->fixed_length && length!=1) || (length>1)) {
		return FALSE;
	}
	if (length==0) {
		return TRUE;
	}
	char ch = cat_string_wo_char_at(a_text, offset);
	int idx = cat_string_wo_index_of(node_single->a_chars, ch);
	gboolean result = FALSE;
	if (idx>=0) {
		result = node_single->inverted ? FALSE : TRUE;
	} else {
		result = node_single->inverted ? TRUE : FALSE;
	}
	cat_log_trace("ch=%c, inverted=%d, found_idx=%d, result=%d", ch, node_single->inverted, idx, result);
	return result;
}

static int l_length(CatRegexpNode *self) {
	CatRegexpNodeSingleCharacter *node = (CatRegexpNodeSingleCharacter *) self;
	return node->fixed_length ? 1 : -1;
}

static void l_dump(CatRegexpNode *self, int indent_level, gboolean recurse) {
	CatStringWo *e_buffer = cat_string_wo_new();
	int indent = indent_level*3;
	for(;indent>=0; indent--) {
		cat_string_wo_append_char(e_buffer, ' ');
	}
	cat_string_wo_append_chars(e_buffer, g_type_name_from_instance((GTypeInstance *) self));
	CatRegexpNodeSingleCharacter *node_single = (CatRegexpNodeSingleCharacter *) self;
	printf("%s[%p] = chars=##%s##, fixed_length=%d, inverted=%d\n", cat_string_wo_getchars(e_buffer), self, cat_string_wo_getchars(node_single->a_chars), node_single->fixed_length, node_single->inverted);
	indent_level++;
	cat_unref_ptr(e_buffer);
}



void cat_regexp_node_single_character_append(CatRegexpNodeSingleCharacter *node, CatStringWo *a_chars) {
	CatStringWo *e_buf = cat_string_wo_clone(node->a_chars, CAT_CLONE_DEPTH_MAIN);
	cat_string_wo_append(e_buf, a_chars);
	CatStringWo *a_buf = cat_string_wo_anchor(e_buf, 0);
	cat_ref_swap(node->a_chars, a_buf);
	cat_unref_ptr(a_buf);
}
