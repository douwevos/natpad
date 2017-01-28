/*
   File:    draactionreplay.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 23, 2016
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

#ifndef ACTION_MACRO_DRAACTIONREPLAY_H_
#define ACTION_MACRO_DRAACTIONREPLAY_H_

#include "../../macros/dramacromanager.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_REPLAY              (dra_action_replay_get_type())
#define DRA_ACTION_REPLAY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_replay_get_type(), DraActionReplay))
#define DRA_ACTION_REPLAY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_REPLAY, DraActionReplayClass))
#define DRA_IS_ACTION_REPLAY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_REPLAY))
#define DRA_IS_ACTION_REPLAY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_REPLAY))
#define DRA_ACTION_REPLAY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_REPLAY, DraActionReplayClass))


typedef struct _DraActionReplay               DraActionReplay;
typedef struct _DraActionReplayPrivate        DraActionReplayPrivate;
typedef struct _DraActionReplayClass          DraActionReplayClass;


struct _DraActionReplay {
	LeaAction parent;
};

struct _DraActionReplayClass {
	LeaActionClass parent_class;
};


GType dra_action_replay_get_type();

DraActionReplay *dra_action_replay_new(DraMacroManager *macro_manager);

G_END_DECLS

#endif /* ACTION_MACRO_DRAACTIONREPLAY_H_ */
