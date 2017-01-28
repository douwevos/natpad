/*
   File:    grorunstate.h
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 10, 2016
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

#ifndef GRORUNSTATE_H_
#define GRORUNSTATE_H_

#include "grorunsymbol.h"
#include "grorunreducelayout.h"
#include "groruntransitionsforsymbol.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_STATE              (grorun_state_get_type())
#define GRORUN_STATE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_state_get_type(), GroRunState))
#define GRORUN_STATE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_STATE, GroRunStateClass))
#define GRORUN_IS_STATE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_STATE))
#define GRORUN_IS_STATE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_STATE))
#define GRORUN_STATE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_STATE, GroRunStateClass))


typedef struct _GroRunState               GroRunState;
typedef struct _GroRunStatePrivate        GroRunStatePrivate;
typedef struct _GroRunStateClass          GroRunStateClass;


struct _GroRunState {
	GObject parent;
};

struct _GroRunStateClass {
	GObjectClass parent_class;
};


GType grorun_state_get_type();

GroRunState *grorun_state_new(int id);

GroRunTransitionsForSymbol *grorun_state_get_or_create_transitions_for_symbol(GroRunState *state, GroRunSymbol *for_symbol);
GroRunTransitionsForSymbol *grorun_state_get_transitions_for_symbol(GroRunState *state, GroRunSymbol *for_symbol);

void grorun_state_add_shift(GroRunState *state, GroRunSymbol *for_symbol, GroRunState *shift_to_state);

void grorun_state_add_reduce(GroRunState *state, GroRunSymbol *for_symbol, GroRunReduceLayout *reduce_layout, int action_id);

void grorun_state_add_accept(GroRunState *state, GroRunSymbol *for_symbol);

G_END_DECLS

#endif /* GRORUNSTATE_H_ */
