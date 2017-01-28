/*
   File:    grorunshifttransition.h
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

#ifndef GRORUNSHIFTTRANSITION_H_
#define GRORUNSHIFTTRANSITION_H_

#include "grorunstate.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_SHIFT_TRANSITION              (grorun_shift_transition_get_type())
#define GRORUN_SHIFT_TRANSITION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_shift_transition_get_type(), GroRunShiftTransition))
#define GRORUN_SHIFT_TRANSITION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_SHIFT_TRANSITION, GroRunShiftTransitionClass))
#define GRORUN_IS_SHIFT_TRANSITION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_SHIFT_TRANSITION))
#define GRORUN_IS_SHIFT_TRANSITION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_SHIFT_TRANSITION))
#define GRORUN_SHIFT_TRANSITION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_SHIFT_TRANSITION, GroRunShiftTransitionClass))


typedef struct _GroRunShiftTransition               GroRunShiftTransition;
typedef struct _GroRunShiftTransitionPrivate        GroRunShiftTransitionPrivate;
typedef struct _GroRunShiftTransitionClass          GroRunShiftTransitionClass;


struct _GroRunShiftTransition {
	GObject parent;
};

struct _GroRunShiftTransitionClass {
	GObjectClass parent_class;
};


GType grorun_shift_transition_get_type();

GroRunShiftTransition *grorun_shift_transition_new(GroRunState *shift_to_state);

GroRunState *grorun_shift_transition_get_forward_state(GroRunShiftTransition *shift_trans);

G_END_DECLS

#endif /* GRORUNSHIFTTRANSITION_H_ */
