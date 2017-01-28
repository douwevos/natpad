/*
 * catregexpnode.c
 *
 *  Created on: Jun 24, 2010
 */

#include "catregexpnode.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatRegexpNode"
#include "../logging/catlog.h"

//static void l_dispose(GObject *object);
static void l_dump(CatRegexpNode *self, int indent_level, gboolean recurse);

G_DEFINE_TYPE(CatRegexpNode, cat_regexp_node, G_TYPE_OBJECT)


static void cat_regexp_node_class_init(CatRegexpNodeClass *clazz) {
	clazz->dump = l_dump;

//	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
//	object_class->dispose = l_dispose;
}

static void cat_regexp_node_init(CatRegexpNode *obj) {
}

//static void l_dispose(GObject *object) {
//}


gboolean cat_regexp_node_matches(CatRegexpNode *self, CatStringWo *a_text, int offset, int length) {
	return CAT_REGEXP_NODE_GET_CLASS(self)->matches(self, a_text, offset, length);
}

int cat_regexp_node_length(CatRegexpNode *self) {
	return CAT_REGEXP_NODE_GET_CLASS(self)->length(self);
}

void cat_regexp_node_dump(CatRegexpNode *self, int indent_level, gboolean recurse) {
	CatRegexpNodeClass *node_class = CAT_REGEXP_NODE_GET_CLASS(self);
	node_class->dump(self, indent_level, recurse);
}




static void l_dump(CatRegexpNode *self, int indent_level, gboolean recurse) {
	CatStringWo *e_buffer = cat_string_wo_new();
	indent_level = indent_level*3;
	for(;indent_level>=0; indent_level--) {
		cat_string_wo_append_char(e_buffer, ' ');
	}
	cat_string_wo_append_chars(e_buffer, g_type_name_from_instance((GTypeInstance *) self));
	printf("%s[%p]\n", cat_string_wo_getchars(e_buffer), self);
	cat_unref_ptr(e_buffer);
}
