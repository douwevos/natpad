/*
   File:    groruncontext.h
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

#ifndef GRORUNCONTEXT_H_
#define GRORUNCONTEXT_H_

#include "grorunitokenfactory.h"
#include "grorunstate.h"
#include "grorunmodel.h"
#include "groruniactionhandler.h"
#include "grorunitoken.h"
#include "grorunfork.h"
#include "groruntransition.h"
#include "grorunsymbol.h"
#include "grorunreducelayout.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_CONTEXT              (grorun_context_get_type())
#define GRORUN_CONTEXT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_context_get_type(), GroRunContext))
#define GRORUN_CONTEXT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_CONTEXT, GroRunContextClass))
#define GRORUN_IS_CONTEXT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_CONTEXT))
#define GRORUN_IS_CONTEXT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_CONTEXT))
#define GRORUN_CONTEXT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_CONTEXT, GroRunContextClass))


typedef struct _GroRunContext               GroRunContext;
typedef struct _GroRunContextPrivate        GroRunContextPrivate;
typedef struct _GroRunContextClass          GroRunContextClass;


struct _GroRunContext {
	GObject parent;
};

struct _GroRunContextClass {
	GObjectClass parent_class;
};


GType grorun_context_get_type();

GroRunContext *grorun_context_new(GroRunITokenFactory *token_factory, GroRunModel *state_model, GroRunIActionHandler *action_handler);

void grorun_context_create_start_fork(GroRunContext *context, GroRunIToken *first_lah);
GroRunSymbol *grorun_context_get_error_symbol(GroRunContext *context);
GroRunSymbol *grorun_context_get_eof_symbol(GroRunContext *context);
GroRunIToken *grorun_context_create_error_toklen(GroRunContext *context);

void grorun_context_fork(GroRunContext *context, int fork_idx, GroRunTransition *pending_transition);
void grorun_context_add_fork(GroRunContext *context, GroRunFork *fork);


int grorun_context_fork_count(GroRunContext *context);
GroRunFork *grorun_context_fork_at(GroRunContext *context, int fork_idx);

GroRunIToken *grorun_context_reduce(GroRunContext *context, GroRunFork *fork, GroRunReduceLayout *reduce_layout, int action_id);

GroRunIToken *grorun_context_create_for_pushback(GroRunContext *context, const GroRunLeftRight lr, GroRunLocation *location, const CatStringWo *terminal_name);

void grorun_context_mark_fork_as_error(GroRunContext *context, int fork_idx);

void grorun_context_mark_fork_as_accepted(GroRunContext *context, int fork_idx);

void grorun_context_add_accepted(GroRunContext *context, GroRunFork *fork);

GObject *grorun_context_get_first_accepted(GroRunContext *context);
GroRunFork *grorun_context_accepted_fork_first(GroRunContext *context);
gboolean grorun_context_fork_has_accepted(GroRunContext *context);

void grorun_context_error_forks_clear(GroRunContext *context);
void grorun_context_error_fork(GroRunContext *context, int fork_idx, GroRunTransition *pending_transition);
int grorun_context_error_fork_count(GroRunContext *context);
GroRunFork *grorun_context_error_fork_at(GroRunContext *context, int fork_idx);
void grorun_context_error_fork_remove(GroRunContext *context, int fork_idx);


G_END_DECLS

#endif /* GRORUNCONTEXT_H_ */
