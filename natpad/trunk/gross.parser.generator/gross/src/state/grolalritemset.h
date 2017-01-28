
/*
   File:    grolalritemset.h
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

#ifndef PARSER_STATE_GROLALRITEMSET_H_
#define PARSER_STATE_GROLALRITEMSET_H_

#include "grolalritem.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_LALR_ITEM_SET              (gro_lalr_item_set_get_type())
#define GRO_LALR_ITEM_SET(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_lalr_item_set_get_type(), GroLalrItemSet))
#define GRO_LALR_ITEM_SET_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_LALR_ITEM_SET, GroLalrItemSetClass))
#define GRO_IS_LALR_ITEM_SET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_LALR_ITEM_SET))
#define GRO_IS_LALR_ITEM_SET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_LALR_ITEM_SET))
#define GRO_LALR_ITEM_SET_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_LALR_ITEM_SET, GroLalrItemSetClass))


typedef struct _GroLalrItemSet               GroLalrItemSet;
typedef struct _GroLalrItemSetPrivate        GroLalrItemSetPrivate;
typedef struct _GroLalrItemSetClass          GroLalrItemSetClass;


struct _GroLalrItemSet {
	GObject parent;
};

struct _GroLalrItemSetClass {
	GObjectClass parent_class;
};


GType gro_lalr_item_set_get_type();

GroLalrItemSet *gro_lalr_item_set_new();
GroLalrItemSet *gro_lalr_item_set_clone(GroLalrItemSet *to_clone);

void gro_lalr_item_set_compute_closure(GroLalrItemSet *item_set);

GroLalrItem *gro_lalr_item_set_add(GroLalrItemSet *item_set, GroLalrItem *item);
GroLalrItem *gro_lalr_item_set_find(GroLalrItemSet *item_set, const GroLalrItem *item);

GroLalrItemIterator *gro_lalr_item_set_iterator(GroLalrItemSet *item_set);

int gro_lalr_item_set_hash(GroLalrItemSet *item_set);
gboolean gro_lalr_item_set_equal(const GroLalrItemSet *item_set_a, const GroLalrItemSet *item_set_b);

G_END_DECLS

#endif /* PARSER_STATE_GROLALRITEMSET_H_ */
