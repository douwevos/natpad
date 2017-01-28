/*
 * catregexpnode.h
 *
 *  Created on: Jun 24, 2010
 */

#ifndef CATREGEXPNODE_H_
#define CATREGEXPNODE_H_

#include <glib-object.h>
#include "../woo/catstringwo.h"

G_BEGIN_DECLS

#define CAT_TYPE_REGEXP_NODE            (cat_regexp_node_get_type())
#define CAT_REGEXP_NODE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_regexp_node_get_type(), CatRegexpNode))
#define CAT_REGEXP_NODE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_REGEXP_NODE, CatRegexpNodeClass))
#define CAT_IS_REGEXP_NODE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_REGEXP_NODE))
#define CAT_IS_REGEXP_NODE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_REGEXP_NODE))
#define CAT_REGEXP_NODE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_REGEXP_NODE, CatRegexpNodeClass))


typedef struct _CatRegexpNode         CatRegexpNode;

typedef struct _CatRegexpNodeClass    CatRegexpNodeClass;


struct _CatRegexpNode {
	GObject parent;
};

struct _CatRegexpNodeClass {
	GObjectClass parent_class;
	gboolean (*matches)(CatRegexpNode *self, CatStringWo *a_text, int offset, int length);
	int (*length)(CatRegexpNode *self);
	void (*dump)(CatRegexpNode *self, int indent_level, gboolean recurse);
};

GType cat_regexp_node_get_type(void);

gboolean cat_regexp_node_matches(CatRegexpNode *self, CatStringWo *a_text, int offset, int length);

int cat_regexp_node_length(CatRegexpNode *self);


void cat_regexp_node_dump(CatRegexpNode *self, int indent_level, gboolean recurse);

G_END_DECLS

#endif /* CATREGEXPNODE_H_ */
