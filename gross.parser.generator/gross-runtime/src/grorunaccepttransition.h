/*
   File:    grorunaccepttransition.h
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

#ifndef GRORUNACCEPTTRANSITION_H_
#define GRORUNACCEPTTRANSITION_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_ACCEPT_TRANSITION              (grorun_accept_transition_get_type())
#define GRORUN_ACCEPT_TRANSITION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_accept_transition_get_type(), GroRunAcceptTransition))
#define GRORUN_ACCEPT_TRANSITION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_ACCEPT_TRANSITION, GroRunAcceptTransitionClass))
#define GRORUN_IS_ACCEPT_TRANSITION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_ACCEPT_TRANSITION))
#define GRORUN_IS_ACCEPT_TRANSITION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_ACCEPT_TRANSITION))
#define GRORUN_ACCEPT_TRANSITION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_ACCEPT_TRANSITION, GroRunAcceptTransitionClass))


typedef struct _GroRunAcceptTransition               GroRunAcceptTransition;
typedef struct _GroRunAcceptTransitionPrivate        GroRunAcceptTransitionPrivate;
typedef struct _GroRunAcceptTransitionClass          GroRunAcceptTransitionClass;


struct _GroRunAcceptTransition {
	GObject parent;
};

struct _GroRunAcceptTransitionClass {
	GObjectClass parent_class;
};


GType grorun_accept_transition_get_type();

GroRunAcceptTransition *grorun_accept_transition_new();

G_END_DECLS

#endif /* GRORUNACCEPTTRANSITION_H_ */
