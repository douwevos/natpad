/*
   File:    grorunfork.h
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 11, 2016
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

#ifndef GRORUNFORK_H_
#define GRORUNFORK_H_

#include "grorunstate.h"
#include "grorunitoken.h"
#include "grorunstackentry.h"
#include "groruntransition.h"
#include "grorunmessage.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_FORK              (grorun_fork_get_type())
#define GRORUN_FORK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_fork_get_type(), GroRunFork))
#define GRORUN_FORK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_FORK, GroRunForkClass))
#define GRORUN_IS_FORK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_FORK))
#define GRORUN_IS_FORK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_FORK))
#define GRORUN_FORK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_FORK, GroRunForkClass))


typedef struct _GroRunFork               GroRunFork;
typedef struct _GroRunForkPrivate        GroRunForkPrivate;
typedef struct _GroRunForkClass          GroRunForkClass;


struct _GroRunFork {
	GObject parent;
};

struct _GroRunForkClass {
	GObjectClass parent_class;
	CatAtomicInteger *sequence;
};


GType grorun_fork_get_type();

GroRunFork *grorun_fork_new();

GroRunFork *grorun_fork_duplicate(GroRunFork *orig_fork, int recovery_count);

void grorun_fork_append_message(GroRunFork *fork, CatStringWo *text, GroRunLocation *location);
GroRunMessage *grorun_fork_get_messages(GroRunFork *fork);

int grorun_fork_get_recovery_count(GroRunFork *fork);
gboolean grorun_fork_decrease_recovery(GroRunFork *fork);

void grorun_fork_push(GroRunFork *fork, GroRunState *state, GroRunIToken *lah);

GroRunStackEntry *grorun_fork_get_top_entry(GroRunFork *fork);
void grorun_fork_set_top_entry(GroRunFork *fork, GroRunStackEntry *new_top);

GroRunStackEntry *grorun_fork_get_entry(GroRunFork *fork, int offset);


GroRunTransition *grorun_fork_get_pending_transition(GroRunFork *fork);

void grorun_fork_set_pending_transition(GroRunFork *fork, GroRunTransition *pending_transition);

void grorun_fork_push_back_token(GroRunFork *fork, const char *token_name);
CatStringWo *grorun_fork_get_push_back_token(GroRunFork *fork);
gboolean grorun_fork_has_pushback(GroRunFork *fork);

void grorun_fork_set_postponed(GroRunFork *fork, GroRunIToken *token);

GroRunIToken *grorun_fork_get_postponed(GroRunFork *fork);

void grorun_fork_dump(GroRunFork *fork);

G_END_DECLS

#endif /* GRORUNFORK_H_ */
