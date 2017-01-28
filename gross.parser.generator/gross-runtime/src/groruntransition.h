/*
   File:    groruntransition.h
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

#ifndef GRORUNTRANSITION_H_
#define GRORUNTRANSITION_H_

#include <caterpillar.h>

#define GRORUN_TYPE_TRANSITION                 (grorun_transition_get_type())
#define GRORUN_TRANSITION(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), GRORUN_TYPE_TRANSITION, GroRunTransition))
#define GRORUN_IS_TRANSITION(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), GRORUN_TYPE_TRANSITION))
#define GRORUN_TRANSITION_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), GRORUN_TYPE_TRANSITION, GroRunTransitionInterface))


typedef struct _GroRunTransition               GroRunTransition; /* dummy object */
typedef struct _GroRunTransitionInterface      GroRunTransitionInterface;


struct _GroRunTransitionInterface {
	GTypeInterface parent_iface;
};

GType grorun_transition_get_type(void);


#endif /* GRORUNTRANSITION_H_ */
