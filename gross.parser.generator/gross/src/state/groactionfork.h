
/*
   File:    groactionfork.h
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 23, 2016
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

#ifndef STATE_GROACTIONFORK_H_
#define STATE_GROACTIONFORK_H_

#include "groiparseaction.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_ACTION_FORK              (gro_action_fork_get_type())
#define GRO_ACTION_FORK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_action_fork_get_type(), GroActionFork))
#define GRO_ACTION_FORK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_ACTION_FORK, GroActionForkClass))
#define GRO_IS_ACTION_FORK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_ACTION_FORK))
#define GRO_IS_ACTION_FORK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_ACTION_FORK))
#define GRO_ACTION_FORK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_ACTION_FORK, GroActionForkClass))


typedef struct _GroActionFork               GroActionFork;
typedef struct _GroActionForkPrivate        GroActionForkPrivate;
typedef struct _GroActionForkClass          GroActionForkClass;


struct _GroActionFork {
	GObject parent;
};

struct _GroActionForkClass {
	GObjectClass parent_class;
};


GType gro_action_fork_get_type();

GroActionFork *gro_action_fork_new(int fork_index, GroIParseAction *parse_action);

int gro_action_fork_get_index(const GroActionFork *action_fork);

int gro_action_fork_action_count(const GroActionFork *action_fork);
CatIIterator *gro_action_fork_action_iterator(GroActionFork *action_fork);

void gro_action_fork_add_action(GroActionFork *action_fork, GroIParseAction *parse_action);

gboolean gro_action_fork_actions_equal(GroIParseAction *parse_action_a, GroIParseAction *parse_action_b);

gint gro_action_fork_compare(const GroActionFork *a, const GroActionFork *b);

G_END_DECLS

#endif /* STATE_GROACTIONFORK_H_ */
