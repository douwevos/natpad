/*
   File:    grorunstackentry.h
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

#ifndef GRORUNSTACKENTRY_H_
#define GRORUNSTACKENTRY_H_

#include "grorunstate.h"
#include "grorunitoken.h"
#include "grorunsymbol.h"
#include "groruntransitionsforsymbol.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_STACK_ENTRY              (grorun_stack_entry_get_type())
#define GRORUN_STACK_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_stack_entry_get_type(), GroRunStackEntry))
#define GRORUN_STACK_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_STACK_ENTRY, GroRunStackEntryClass))
#define GRORUN_IS_STACK_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_STACK_ENTRY))
#define GRORUN_IS_STACK_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_STACK_ENTRY))
#define GRORUN_STACK_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_STACK_ENTRY, GroRunStackEntryClass))


typedef struct _GroRunStackEntry               GroRunStackEntry;
typedef struct _GroRunStackEntryPrivate        GroRunStackEntryPrivate;
typedef struct _GroRunStackEntryClass          GroRunStackEntryClass;


struct _GroRunStackEntry {
	GObject parent;
};

struct _GroRunStackEntryClass {
	GObjectClass parent_class;
};


GType grorun_stack_entry_get_type();

GroRunStackEntry *grorun_stack_entry_new(GroRunStackEntry *parent, GroRunState *state, GroRunIToken *lookahead);

GroRunTransitionsForSymbol *grorun_stack_entry_get_transitions_for_symbol(GroRunStackEntry *entry);
GroRunTransitionsForSymbol *grorun_stack_entry_get_transitions_for_symbols(GroRunStackEntry *entry, GroRunSymbol *symbol);


GroRunStackEntry *grorun_stack_entry_get_parent(GroRunStackEntry *entry);

GroRunIToken *grorun_stack_entry_get_lookahead(GroRunStackEntry *entry);

GroRunState *grorun_stack_entry_get_state(GroRunStackEntry *entry);

void *grorun_stack_entry_get_value(GroRunStackEntry *entry);

G_END_DECLS

#endif /* GRORUNSTACKENTRY_H_ */
