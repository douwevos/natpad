
/*
   File:    grolalrtransition.h
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 3, 2016
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

#ifndef PARSER_STATE_GROLALRTRANSITION_H_
#define PARSER_STATE_GROLALRTRANSITION_H_

#include "../model/gromsymbol.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_LALR_TRANSITION              (gro_lalr_transition_get_type())
#define GRO_LALR_TRANSITION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_lalr_transition_get_type(), GroLalrTransition))
#define GRO_LALR_TRANSITION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_LALR_TRANSITION, GroLalrTransitionClass))
#define GRO_IS_LALR_TRANSITION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_LALR_TRANSITION))
#define GRO_IS_LALR_TRANSITION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_LALR_TRANSITION))
#define GRO_LALR_TRANSITION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_LALR_TRANSITION, GroLalrTransitionClass))


typedef struct _GroLalrTransition               GroLalrTransition;
typedef struct _GroLalrTransitionPrivate        GroLalrTransitionPrivate;
typedef struct _GroLalrTransitionClass          GroLalrTransitionClass;


struct _GroLalrTransition {
	GObject parent;
};

struct _GroLalrTransitionClass {
	GObjectClass parent_class;
};


struct _GroLalrState;

GType gro_lalr_transition_get_type();

GroLalrTransition *gro_lalr_transition_new(GroMSymbol *on_sym, struct _GroLalrState *to_state, GroLalrTransition *next);

GroLalrTransition *gro_lalr_transition_next(GroLalrTransition *transition);

GroMSymbol *gro_lalr_transition_get_key_symbol(GroLalrTransition *transition);
struct _GroLalrState *gro_lalr_transition_get_to_state(GroLalrTransition *transition);

G_END_DECLS

#endif /* PARSER_STATE_GROLALRTRANSITION_H_ */
