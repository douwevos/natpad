
/*
   File:    grolalritem.h
   Project: gross
   Author:  Douwe Vos
   Date:    May 31, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef PARSER_STATE_GROLALRITEM_H_
#define PARSER_STATE_GROLALRITEM_H_

#include "../model/gromproduction.h"
#include "../model/groterminalset.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_LALR_ITEM              (gro_lalr_item_get_type())
#define GRO_LALR_ITEM(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_lalr_item_get_type(), GroLalrItem))
#define GRO_LALR_ITEM_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_LALR_ITEM, GroLalrItemClass))
#define GRO_IS_LALR_ITEM(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_LALR_ITEM))
#define GRO_IS_LALR_ITEM_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_LALR_ITEM))
#define GRO_LALR_ITEM_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_LALR_ITEM, GroLalrItemClass))


typedef struct _GroLalrItem               GroLalrItem;
typedef struct _GroLalrItemPrivate        GroLalrItemPrivate;
typedef struct _GroLalrItemClass          GroLalrItemClass;

#define GroLalrItemIterator CatIIterator
#define gro_lalr_item_iterator_has_next(iter) cat_iiterator_has_next(iter)
#define gro_lalr_item_iterator_next(iter) (GroLalrItem *) cat_iiterator_next(iter)

struct _GroLalrItem {
	GObject parent;
};

struct _GroLalrItemClass {
	GObjectClass parent_class;
};


GType gro_lalr_item_get_type();

GroLalrItem *gro_lalr_item_new(GroMProduction *production);

GroLalrItem *gro_lalr_item_new_full(GroMProduction *production, int dot_index, GroTerminalSet *lookahead);

int gro_lalr_item_get_dot_index(GroLalrItem *item);

/** Is the dot at the end of the production? */
gboolean gro_lalr_item_is_dot_at_end(const GroLalrItem *item);

GroTerminalSet *gro_lalr_item_get_lookahead_set(GroLalrItem *item);
GroTerminalSet *gro_lalr_item_calculate_lookahead(GroLalrItem *item);

GroMSymbol *gro_lalr_item_get_symbol_after_dot(GroLalrItem *item);

GroMSymbol *gro_lalr_item_get_first_symbol(GroLalrItem *item);

GroMProduction *gro_lalr_item_get_production(GroLalrItem *item);


/* Determine if everything from the symbol one beyond the dot all the way to
 * the end of the right hand side is nullable. This would indicate that the
 * lookahead of this item must be included in the lookaheads of all items
 * produced as a closure of this item. Note: this routine should not be
 * invoked until after first sets and nullability have been calculated for
 * all non terminals.
 */
gboolean gro_lalr_item_is_lookahead_visible(GroLalrItem *item);

void gro_lalr_item_propagate_lookaheads(GroLalrItem *item, GroTerminalSet *incoming);

/* Add a new item to the set of items we propagate to.
 */
void gro_lalr_item_add_propagate(GroLalrItem *item, GroLalrItem *prop_to);

CatArrayWo *gro_lalr_item_get_propagate_items(GroLalrItem *item);



/* Produce the new lalr_item that results from shifting the dot one position
 * to the right.
 */
GroLalrItem *gro_lalr_item_shift(GroLalrItem *item);


gboolean gro_lalr_item_equal(const GroLalrItem *item_a, const GroLalrItem *item_b);

int gro_lalr_item_hash(GroLalrItem *item);

G_END_DECLS

#endif /* PARSER_STATE_GROLALRITEM_H_ */
