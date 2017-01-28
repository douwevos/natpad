/*
 * catregexpnodelist.h
 *
 *  Created on: Jun 24, 2010
 */

#ifndef CATREGEXPNODELIST_H_
#define CATREGEXPNODELIST_H_

#include "catregexpnode.h"

G_BEGIN_DECLS

#define CAT_TYPE_REGEXP_NODE_LIST            (cat_regexp_node_list_get_type())
#define CAT_REGEXP_NODE_LIST(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_regexp_node_list_get_type(), CatRegexpNodeList))
#define CAT_REGEXP_NODE_LIST_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_REGEXP_NODE_LIST, CatRegexpNodeListClass))
#define CAT_IS_REGEXP_NODE_LIST(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_REGEXP_NODE_LIST))
#define CAT_IS_REGEXP_NODE_LIST_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_REGEXP_NODE_LIST))
#define CAT_REGEXP_NODE_LIST_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_REGEXP_NODE_LIST, CatRegexpNodeListClass))


typedef struct _CatRegexpNodeList         CatRegexpNodeList;

typedef struct _CatRegexpNodeListClass    CatRegexpNodeListClass;


struct _CatRegexpNodeList {
	CatRegexpNode parent;
	CatArrayWo *e_list;
};

struct _CatRegexpNodeListClass {
	CatRegexpNodeClass parent_class;
};

CatRegexpNodeList *cat_regexp_node_list_new();

void cat_regexp_node_list_add(CatRegexpNodeList *node_list, CatRegexpNode *to_add);

CatRegexpNode *cat_regexp_node_list_get_last(CatRegexpNodeList *node_list);

void cat_regexp_node_list_replace_last(CatRegexpNodeList *node_list, CatRegexpNode *replace_node);


G_END_DECLS

#endif /* CATREGEXPNODELIST_H_ */
