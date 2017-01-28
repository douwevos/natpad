/*
 * catregexpnodesinglecharacter.h
 *
 *  Created on: Jun 24, 2010
 */

#ifndef CATREGEXPNODESINGLECHARACTER_H_
#define CATREGEXPNODESINGLECHARACTER_H_

#include "catregexpnode.h"

G_BEGIN_DECLS

#define CAT_TYPE_REGEXP_NODE_SINGLE_CHARACTER            (cat_regexp_node_single_character_get_type())
#define CAT_REGEXP_NODE_SINGLE_CHARACTER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_regexp_node_single_character_get_type(), CatRegexpNodeSingleCharacter))
#define CAT_REGEXP_NODE_SINGLE_CHARACTER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_REGEXP_NODE_SINGLE_CHARACTER, CatRegexpNodeSingleCharacterClass))
#define CAT_IS_REGEXP_NODE_SINGLE_CHARACTER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_REGEXP_NODE_SINGLE_CHARACTER))
#define CAT_IS_REGEXP_NODE_SINGLE_CHARACTER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_REGEXP_NODE_SINGLE_CHARACTER))
#define CAT_REGEXP_NODE_SINGLE_CHARACTER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_REGEXP_NODE_SINGLE_CHARACTER, CatRegexpNodeSingleCharacterClass))


typedef struct _CatRegexpNodeSingleCharacter         CatRegexpNodeSingleCharacter;

typedef struct _CatRegexpNodeSingleCharacterClass    CatRegexpNodeSingleCharacterClass;


struct _CatRegexpNodeSingleCharacter {
	CatRegexpNode parent;
	CatStringWo *a_chars;
	gboolean inverted;
	gboolean fixed_length;
};

struct _CatRegexpNodeSingleCharacterClass {
	CatRegexpNodeClass parent_class;
};

CatRegexpNodeSingleCharacter *cat_regexp_node_single_character_new(CatStringWo *a_chars, gboolean inverted, gboolean fixed_length);

void cat_regexp_node_single_character_append(CatRegexpNodeSingleCharacter *node, CatStringWo *a_chars);

G_END_DECLS


#endif /* CATREGEXPNODESINGLECHARACTER_H_ */
