/*
   File:    grorunfork.c
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

#include "grorunfork.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "GroRunFork"
#include <logging/catlog.h>

struct _GroRunForkPrivate {
	int id;
	GroRunStackEntry *reverse_stack;
	GroRunTransition *pending_transition;
	GroRunIToken *postponed;
	CatArrayWo *push_back;
	int recovery_count;
	GroRunMessage *messages;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(GroRunFork, grorun_fork, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroRunFork)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void grorun_fork_class_init(GroRunForkClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	clazz->sequence = cat_atomic_integer_new();
	cat_ref_ignore(clazz->sequence);
}

static void grorun_fork_init(GroRunFork *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	GroRunFork *instance = GRORUN_FORK(object);
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(instance);
	cat_unref_ptr(priv->pending_transition);
	cat_unref_ptr(priv->postponed);
	cat_unref_ptr(priv->reverse_stack);
	cat_unref_ptr(priv->messages);
	G_OBJECT_CLASS(grorun_fork_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(grorun_fork_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


GroRunFork *grorun_fork_new() {
	GroRunFork *result = g_object_new(GRORUN_TYPE_FORK, NULL);
	cat_ref_anounce(result);
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(result);
	priv->id = cat_atomic_integer_increment(GRORUN_FORK_GET_CLASS(result)->sequence);
	priv->reverse_stack = NULL;
	priv->pending_transition = NULL;
	priv->postponed = NULL;
	priv->messages = NULL;
	return result;
}

GroRunFork *grorun_fork_duplicate(GroRunFork *orig_fork, int recovery_count) {
	GroRunFork *result = g_object_new(GRORUN_TYPE_FORK, NULL);
	cat_ref_anounce(result);
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(result);
	priv->id = cat_atomic_integer_increment(GRORUN_FORK_GET_CLASS(result)->sequence);
	GroRunForkPrivate *spriv = grorun_fork_get_instance_private(orig_fork);
	priv->reverse_stack = cat_ref_ptr(spriv->reverse_stack);
	priv->pending_transition = NULL;
	priv->postponed = cat_ref_ptr(spriv->postponed);
	priv->recovery_count = recovery_count;
	priv->messages = cat_ref_ptr(spriv->messages);
	return result;
}

void grorun_fork_append_message(GroRunFork *fork, CatStringWo *text, GroRunLocation *location) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	GroRunMessage *message = grorun_message_new(priv->messages, text, location);
	cat_ref_swap(priv->messages, message);
	cat_unref_ptr(message);
}

GroRunMessage *grorun_fork_get_messages(GroRunFork *fork) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	return priv->messages;
}


int grorun_fork_get_recovery_count(GroRunFork *fork) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	return priv->recovery_count;
}

gboolean grorun_fork_decrease_recovery(GroRunFork *fork) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	priv->recovery_count--;
	return priv->recovery_count<0;
}


void grorun_fork_push(GroRunFork *fork, GroRunState *state, GroRunIToken *lah) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	GroRunStackEntry *nrs = grorun_stack_entry_new(priv->reverse_stack, state, lah);
	cat_unref_ptr(priv->reverse_stack);
	priv->reverse_stack = nrs;
}

GroRunStackEntry *grorun_fork_get_top_entry(GroRunFork *fork) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	return priv->reverse_stack;
}

void grorun_fork_set_top_entry(GroRunFork *fork, GroRunStackEntry *new_top) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	cat_ref_swap(priv->reverse_stack, new_top);
}

GroRunStackEntry *grorun_fork_get_entry(GroRunFork *fork, int pl_idx) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	if (pl_idx<0) {
		return NULL;
	}
	GroRunStackEntry *result = priv->reverse_stack;
	while(result!=NULL && pl_idx>0) {
		pl_idx--;
		result = grorun_stack_entry_get_parent(result);
	}
	return result;
}


GroRunTransition *grorun_fork_get_pending_transition(GroRunFork *fork) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	GroRunTransition *result = priv->pending_transition;
	priv->pending_transition = NULL;
	return result;
}

void grorun_fork_set_pending_transition(GroRunFork *fork, GroRunTransition *pending_transition) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	cat_ref_swap(priv->pending_transition, pending_transition);
}

void grorun_fork_push_back_token(GroRunFork *fork, const char *token_name) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	if (priv->push_back==NULL) {
		priv->push_back = cat_array_wo_new();
	}
	CatStringWo *tn = cat_string_wo_new_with(token_name);
	cat_array_wo_append(priv->push_back, tn);
	cat_log_error("pushing back:%O", tn);
	grorun_fork_dump(fork);
	cat_unref_ptr(tn);
}

CatStringWo *grorun_fork_get_push_back_token(GroRunFork *fork) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	CatStringWo *result = NULL;
	if (priv->push_back) {
		if (!cat_array_wo_remove_first(priv->push_back, &result)) {
			cat_unref_ptr(priv->push_back);
		}
	}
	return result;
}

gboolean grorun_fork_has_pushback(GroRunFork *fork) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	return priv->push_back!=NULL;
}


void grorun_fork_set_postponed(GroRunFork *fork, GroRunIToken *token) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	cat_ref_swap(priv->postponed, token);
}

GroRunIToken *grorun_fork_get_postponed(GroRunFork *fork) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	return priv->postponed;
}


void grorun_fork_dump(GroRunFork *fork) {
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(fork);
	cat_log_print("FORK", "Fork[%d]", priv->id)
	GroRunStackEntry *e = priv->reverse_stack;
	while(e!=NULL) {
		cat_log_print("FORK", " # State=%O, lookahead=%O", grorun_stack_entry_get_state(e), grorun_stack_entry_get_lookahead(e));
		e = grorun_stack_entry_get_parent(e);
	}
	cat_log_print("FORK", " - pending-trans : %O", priv->pending_transition);
	cat_log_print("FORK", " - postponed     : %O", priv->postponed);
	if (priv->recovery_count>0) {
		cat_log_print("FORK", " - rec-count     : %d", priv->recovery_count);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	GroRunFork *instance = GRORUN_FORK(self);
	GroRunForkPrivate *priv = grorun_fork_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p id=%d, postponed=%O, pending=%O]", iname, self, priv->id, priv->postponed, priv->pending_transition);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
