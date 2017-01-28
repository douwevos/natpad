
/*
   File:    grostatemachine.h
   Project: gross
   Author:  Douwe Vos
   Date:    May 23, 2016
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

#ifndef PARSER_STATE_GROSTATEMACHINE_H_
#define PARSER_STATE_GROSTATEMACHINE_H_

#include "grolalrstate.h"
#include "grotablebuilder.h"
#include "groparseactiontable.h"
#include "groparsereducetable.h"
#include "../model/gromodel.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_STATE_MACHINE              (gro_state_machine_get_type())
#define GRO_STATE_MACHINE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_state_machine_get_type(), GroStateMachine))
#define GRO_STATE_MACHINE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_STATE_MACHINE, GroStateMachineClass))
#define GRO_IS_STATE_MACHINE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_STATE_MACHINE))
#define GRO_IS_STATE_MACHINE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_STATE_MACHINE))
#define GRO_STATE_MACHINE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_STATE_MACHINE, GroStateMachineClass))


typedef struct _GroStateMachine               GroStateMachine;
typedef struct _GroStateMachinePrivate        GroStateMachinePrivate;
typedef struct _GroStateMachineClass          GroStateMachineClass;


struct _GroStateMachine {
	GObject parent;
};

struct _GroStateMachineClass {
	GObjectClass parent_class;
};

GType gro_state_machine_get_type();

GroStateMachine *gro_state_machine_new(GroModel *model, GroTypeOfParser top);

void gro_state_machine_dump(GroStateMachine *state_machine);

void gro_state_machine_build(GroStateMachine *state_machine);

GroLalrState *gro_state_machine_create_states(GroStateMachine *state_machine);

int gro_state_machine_state_count(GroStateMachine *state_machine);

GroParseActionTable *gro_state_machine_get_action_table(GroStateMachine *state_machine);
GroParseReduceTable *gro_state_machine_get_reduce_table(GroStateMachine *state_machine);
GroLalrState *gro_state_machine_get_start_state(GroStateMachine *state_machine);

G_END_DECLS

#endif /* PARSER_STATE_GROSTATEMACHINE_H_ */
