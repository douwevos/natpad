/*
 * catregexpnoderepeater.h
 *
 *  Created on: Jun 24, 2010
 */

#ifndef CATREGEXPNODEREPEATER_H_
#define CATREGEXPNODEREPEATER_H_

#include "catregexpnode.h"

G_BEGIN_DECLS

#define CAT_TYPE_REGEXP_NODE_REPEATER            (cat_regexp_node_repeater_get_type())
#define CAT_REGEXP_NODE_REPEATER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_regexp_node_repeater_get_type(), CatRegexpNodeRepeater))
#define CAT_REGEXP_NODE_REPEATER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_REGEXP_NODE_REPEATER, CatRegexpNodeRepeaterClass))
#define CAT_IS_REGEXP_NODE_REPEATER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_REGEXP_NODE_REPEATER))
#define CAT_IS_REGEXP_NODE_REPEATER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_REGEXP_NODE_REPEATER))
#define CAT_REGEXP_NODE_REPEATER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_REGEXP_NODE_REPEATER, CatRegexpNodeRepeaterClass))


typedef struct _CatRegexpNodeRepeater         CatRegexpNodeRepeater;

typedef struct _CatRegexpNodeRepeaterClass    CatRegexpNodeRepeaterClass;


struct _CatRegexpNodeRepeater {
	CatRegexpNode parent;
	CatRegexpNode *child;
};

struct _CatRegexpNodeRepeaterClass {
	CatRegexpNodeClass parent_class;
};

CatRegexpNodeRepeater *cat_regexp_node_repeater_new(CatRegexpNode *child);

G_END_DECLS

#endif /* CATREGEXPNODEREPEATER_H_ */
