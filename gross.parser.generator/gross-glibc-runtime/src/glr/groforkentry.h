
/*
   File:    groforkentry.h
   Project: gross-glibc-runtime
   Author:  Douwe Vos
   Date:    Jun 24, 2016
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

#ifndef GLR_GROFORKENTRY_H_
#define GLR_GROFORKENTRY_H_

#include "grostateandsymbol.h"
#include "grocontent.h"
#include "../groglibctypes.h"

G_BEGIN_DECLS


enum _GroForkEntryState {
	/* This is a normal fork with no special handling */
	GRO_FORK_ENTRY_NORMAL,
	/* This fork was accepted. No further processing necessary */
	GRO_FORK_ENTRY_ACCEPTED,

	/* This fork is in the error state and is spawning SYNCING forks for each shift. The fork itself
	 * is in the base state of error recovery. In other words the stack is popped till an error can
	 * be shifted. And a dummy error is pushed. This fork enlists all spawned syncing forks. Once a
	 * syncing fork is found that is accepted or has parsed sufficient sync symbols all other spawned
	 * forks are canceled as well as this fork and the state of the sync-fork is changed to normal
	 * in case it was still in state SYNCING  */
	GRO_FORK_ENTRY_ERROR_PARENT,

	/* This is a single syncing instance of a 'ERROR_PARENT' fork */
	GRO_FORK_ENTRY_ERROR_SYNCING,

	GRO_FORK_ENTRY_ACCEPTED_WITH_ERROR

};

typedef enum _GroForkEntryState GroForkEntryState;

#define GRO_TYPE_FORK_ENTRY              (gro_fork_entry_get_type())
#define GRO_FORK_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_fork_entry_get_type(), GroForkEntry))
#define GRO_FORK_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_FORK_ENTRY, GroForkEntryClass))
#define GRO_IS_FORK_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_FORK_ENTRY))
#define GRO_IS_FORK_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_FORK_ENTRY))
#define GRO_FORK_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_FORK_ENTRY, GroForkEntryClass))


typedef struct _GroForkEntry               GroForkEntry;
typedef struct _GroForkEntryPrivate        GroForkEntryPrivate;
typedef struct _GroForkEntryClass          GroForkEntryClass;


struct _GroForkEntry {
	GObject parent;
};

struct _GroForkEntryClass {
	GObjectClass parent_class;
};


GType gro_fork_entry_get_type();

GroForkEntry *gro_fork_entry_new(GroStateAndSymbol *state_and_symbol, GroContent *content);

GroForkEntry *gro_fork_entry_create_fork(GroForkEntry *src_fork_entry, GroStateAndSymbol *state_and_symbol, int pending_act);

void gro_fork_entry_shift(GroForkEntry *fork_entry, void *value, long long left_row, int left, long long right_row, int right);


GroStateAndSymbol *gro_fork_entry_get_state_and_symbol(GroForkEntry *fork_entry);
void gro_fork_entry_set_state_and_symbol(GroForkEntry *fork_entry, GroStateAndSymbol *new_sas);

gboolean gro_fork_entry_is_accepted(GroForkEntry *fork_entry);
void gro_fork_entry_mark_accepted(GroForkEntry *fork_entry);

void gro_fork_entry_set_error_config(GroForkEntry *fork_entry, GroStateAndSymbol *sas, int skip);
void gro_fork_entry_mark_as_recovered(GroForkEntry *fork_entry);
void gro_fork_entry_set_error_parent(GroForkEntry *fork_entry, GroForkEntry *error_parent);
gboolean gro_fork_entry_is_child_of(GroForkEntry *fork_entry, GroForkEntry *error_parent);

int gro_fork_entry_get_sync_count(GroForkEntry *fork_entry);
int gro_fork_entry_increase_and_get_sync_count(GroForkEntry *fork_entry);
int gro_fork_entry_get_error_recovery_count(GroForkEntry *fork_entry);


int gro_fork_entry_get_and_reset_pending_action(GroForkEntry *fork_entry);
void gro_fork_entry_set_pending_action(GroForkEntry *fork_entry, int pending_action);

GroForkEntryState gro_fork_entry_get_status(GroForkEntry *fork_entry);

void gro_fork_entry_set_content_list(GroForkEntry *fork_entry, GPtrArray *new_content_list);

int gro_fork_entry_content_count(GroForkEntry *fork_entry);
GroContent *gro_fork_entry_content_at(GroForkEntry *fork_entry, int index);


void gro_fork_entry_add_error_message(GroForkEntry *fork_entry, void *value, long long left_row, int left, long long right_row, int right);
int gro_fork_entry_message_list_count(GroForkEntry *fork_entry);
GroContent *gro_fork_entry_message_list_at(GroForkEntry *fork_entry, int index);


G_END_DECLS

#endif /* GLR_GROFORKENTRY_H_ */
