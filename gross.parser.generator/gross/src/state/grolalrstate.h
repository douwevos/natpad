
/*
   File:    grolalrstate.h
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 2, 2016
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

#ifndef PARSER_STATE_GROLALRSTATE_H_
#define PARSER_STATE_GROLALRSTATE_H_

#include "grolalritemset.h"
#include "grolalrtransition.h"
#include "../model/gromodel.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_LALR_STATE              (gro_lalr_state_get_type())
#define GRO_LALR_STATE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_lalr_state_get_type(), GroLalrState))
#define GRO_LALR_STATE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_LALR_STATE, GroLalrStateClass))
#define GRO_IS_LALR_STATE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_LALR_STATE))
#define GRO_IS_LALR_STATE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_LALR_STATE))
#define GRO_LALR_STATE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_LALR_STATE, GroLalrStateClass))


typedef struct _GroLalrState               GroLalrState;
typedef struct _GroLalrStatePrivate        GroLalrStatePrivate;
typedef struct _GroLalrStateClass          GroLalrStateClass;

#define GroLalrStateIterator CatIIterator
#define gro_lalr_state_iterator_has_next(iter) cat_iiterator_has_next(iter)
#define gro_lalr_state_iterator_next(iter) (GroLalrState *) cat_iiterator_next(iter)


struct _GroLalrState {
	GObject parent;
};

struct _GroLalrStateClass {
	GObjectClass parent_class;
};

struct _GroParseActionTable;
struct _GroParseReduceTable;

GType gro_lalr_state_get_type();

GroLalrState *gro_lalr_state_new(GroLalrItemSet *item_set, int index);

GroLalrItemSet *gro_lalr_state_get_item_set(GroLalrState *state);
int gro_lalr_state_get_index(GroLalrState *state);

void gro_lalr_state_add_transition(GroLalrState *state, GroMSymbol *on_sym, GroLalrState *to_state);

void gro_lalr_state_propagate_lookaheads(GroLalrState *state);

GroLalrTransition *gro_lalr_state_get_transitions(GroLalrState *state);

G_END_DECLS

#endif /* PARSER_STATE_GROLALRSTATE_H_ */
