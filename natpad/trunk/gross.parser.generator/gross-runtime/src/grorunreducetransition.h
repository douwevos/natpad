/*
   File:    grorunreducetransition.h
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

#ifndef GRORUNREDUCETRANSITION_H_
#define GRORUNREDUCETRANSITION_H_

#include "grorunsymbol.h"
#include "grorunreducelayout.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_REDUCE_TRANSITION              (grorun_reduce_transition_get_type())
#define GRORUN_REDUCE_TRANSITION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_reduce_transition_get_type(), GroRunReduceTransition))
#define GRORUN_REDUCE_TRANSITION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_REDUCE_TRANSITION, GroRunReduceTransitionClass))
#define GRORUN_IS_REDUCE_TRANSITION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_REDUCE_TRANSITION))
#define GRORUN_IS_REDUCE_TRANSITION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_REDUCE_TRANSITION))
#define GRORUN_REDUCE_TRANSITION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_REDUCE_TRANSITION, GroRunReduceTransitionClass))


typedef struct _GroRunReduceTransition               GroRunReduceTransition;
typedef struct _GroRunReduceTransitionPrivate        GroRunReduceTransitionPrivate;
typedef struct _GroRunReduceTransitionClass          GroRunReduceTransitionClass;


struct _GroRunReduceTransition {
	GObject parent;
};

struct _GroRunReduceTransitionClass {
	GObjectClass parent_class;
};


GType grorun_reduce_transition_get_type();

GroRunReduceTransition *grorun_reduce_transition_new(GroRunReduceLayout *reduce_layout, int action_id);

int grorun_reduce_transition_get_rhs_count(GroRunReduceTransition *red_trans);
int grorun_reduce_transition_get_action_id(GroRunReduceTransition *red_trans);
GroRunReduceLayout *grorun_reduce_transition_get_reduce_layout(GroRunReduceTransition *red_trans);


G_END_DECLS

#endif /* GRORUNREDUCETRANSITION_H_ */
