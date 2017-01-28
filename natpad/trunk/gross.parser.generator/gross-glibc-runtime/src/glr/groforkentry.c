
/*
   File:    groforkentry.c
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

#include "groforkentry.h"

struct _GroForkEntryPrivate {
	GroStateAndSymbol *state_and_symbol;
	/* A list of content stacks. Each Entry is of the type GroContent which on its own forms a
	 * reversed linked list. This means each GroContent entry in the content_stacks is the last
	 * element of the linked list it refers to.
	 */
	GPtrArray *content_stacks;

	/* A list message lists. Similar as the content stacks, each entry is of type GroMessage which
	 * forms a reversed linked list.
	 */
	GPtrArray *message_lists;

	/* Pending action is the action which should be run first before continuing normal processing.
	 * The value 0 denotes no action is pending.
	 */
	int pending_action;
	GroForkEntryState status;
	int sync_count;
	int error_recovery_count;
	GroForkEntry *error_parent;
};


G_DEFINE_TYPE_WITH_CODE(GroForkEntry, gro_fork_entry, G_TYPE_OBJECT,
		G_ADD_PRIVATE(GroForkEntry)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void gro_fork_entry_class_init(GroForkEntryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void gro_fork_entry_init(GroForkEntry *instance) {
}


static void l_unref_func(gpointer data, gpointer user_data) {
	gro_unref_ptr(data);
}

static void l_clear_content_list(GPtrArray *content_list) {
	if (content_list) {
		g_ptr_array_foreach(content_list, (GFunc) l_unref_func, NULL);
		g_ptr_array_set_size(content_list, 0);
	}
}

static void l_dispose(GObject *object) {
	gro_log("dispose:%p", object);
	GroForkEntry *instance = GRO_FORK_ENTRY(object);
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(instance);
	gro_unref_ptr(priv->state_and_symbol);
	if (priv->content_stacks) {
		GPtrArray *content_list = priv->content_stacks;
		priv->content_stacks = NULL;
		l_clear_content_list(content_list);
		g_ptr_array_free(content_list, TRUE);
	}
	G_OBJECT_CLASS(gro_fork_entry_parent_class)->dispose(object);
	gro_log("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	gro_log("finalize:%p", object);
	G_OBJECT_CLASS(gro_fork_entry_parent_class)->finalize(object);
	gro_log("finalized:%p", object);
}


GroForkEntry *gro_fork_entry_new(GroStateAndSymbol *state_and_symbol, GroContent *content) {
	GroForkEntry *result = g_object_new(GRO_TYPE_FORK_ENTRY, NULL);
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(result);
	priv->state_and_symbol = gro_ref_ptr(state_and_symbol);
	priv->content_stacks = g_ptr_array_new();
	g_ptr_array_add(priv->content_stacks, content);
	priv->message_lists = g_ptr_array_new();
	g_ptr_array_add(priv->message_lists, NULL);
	priv->pending_action = 0;
	priv->status = GRO_FORK_ENTRY_NORMAL;
	priv->sync_count = 0;
	priv->error_parent = NULL;
	priv->error_recovery_count = 0;
	return result;
}

GroForkEntry *gro_fork_entry_create_fork(GroForkEntry *src_fork_entry, GroStateAndSymbol *state_and_symbol, int pending_act) {
	GroForkEntry *result = g_object_new(GRO_TYPE_FORK_ENTRY, NULL);
	GroForkEntryPrivate *spriv = gro_fork_entry_get_instance_private(src_fork_entry);
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(result);
	priv->state_and_symbol = gro_ref_ptr(state_and_symbol);
	priv->content_stacks = g_ptr_array_sized_new(spriv->content_stacks->len);
	int idx;
	for(idx=0; idx<spriv->content_stacks->len; idx++) {
		void *content = g_ptr_array_index(spriv->content_stacks, idx);
		g_ptr_array_add(priv->content_stacks, content);
		gro_ref_ptr(content);
	}
	priv->message_lists = g_ptr_array_sized_new(spriv->message_lists->len);
	for(idx=0; idx<spriv->message_lists->len; idx++) {
		void *message = g_ptr_array_index(spriv->message_lists, idx);
		g_ptr_array_add(priv->message_lists, message);
		gro_ref_ptr(message);
	}

	priv->pending_action = pending_act;
	priv->status = spriv->status;
	priv->sync_count = 0;
	priv->error_parent = spriv->error_parent;
	priv->error_recovery_count = spriv->error_recovery_count;
	if (priv->status==GRO_FORK_ENTRY_ERROR_PARENT) {
		priv->status = GRO_FORK_ENTRY_ERROR_SYNCING;
		priv->error_parent = src_fork_entry;
		priv->error_recovery_count++;
	}
	return result;
}


void gro_fork_entry_set_error_config(GroForkEntry *fork_entry, GroStateAndSymbol *sas, int skip) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);

	gro_unref_ptr(priv->state_and_symbol);
	priv->state_and_symbol = sas;

	int idx;
	for(idx=0; idx<priv->content_stacks->len; idx++) {
		GroContent *content = g_ptr_array_index(priv->content_stacks, idx);
		GroContent *er_content = gro_content_get_linked_at(content, skip);
		GroContent *new_content = gro_content_new(er_content, NULL, -1,-1,-1,-1);
		g_ptr_array_index(priv->content_stacks, idx)=new_content;
	}
	priv->status = GRO_FORK_ENTRY_ERROR_PARENT;
}


void gro_fork_entry_shift(GroForkEntry *fork_entry, void *value, long long left_row, int left, long long right_row, int right) {
	int content_count = gro_fork_entry_content_count(fork_entry);
	GPtrArray *shifted_content_list = g_ptr_array_sized_new(content_count);
	int content_idx;
	for(content_idx=0; content_idx<content_count; content_idx++) {
		GroContent *content = gro_fork_entry_content_at(fork_entry, content_idx);
		GroContent *shifted_content = gro_content_new(content, value, left_row, left, right_row, right);
		g_ptr_array_add(shifted_content_list, shifted_content);
	}

	gro_fork_entry_set_content_list(fork_entry, shifted_content_list);
}

void gro_fork_entry_add_error_message(GroForkEntry *fork_entry, void *value, long long left_row, int left, long long right_row, int right) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	int message_lists_count = priv->message_lists->len;
	GPtrArray *new_message_lists = g_ptr_array_sized_new(message_lists_count);
	int message_lists_idx;
	for(message_lists_idx=0; message_lists_idx<message_lists_count; message_lists_idx++) {
		GroContent *last_message = g_ptr_array_index(priv->message_lists, message_lists_idx);
		GroContent *new_message= gro_content_new(last_message, value, left_row, left, right_row, right);
		g_ptr_array_add(new_message_lists, new_message);
	}


	if (priv->message_lists) {
		GPtrArray *m_list = priv->message_lists;
		priv->message_lists = NULL;
		l_clear_content_list(m_list);
		g_ptr_array_free(m_list, TRUE);
	}
	priv->message_lists = new_message_lists;
}
int gro_fork_entry_message_list_count(GroForkEntry *fork_entry) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	return priv->message_lists->len;
}

GroContent *gro_fork_entry_message_list_at(GroForkEntry *fork_entry, int index) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	return (GroContent *) g_ptr_array_index(priv->message_lists, index);
}

void gro_fork_entry_mark_as_recovered(GroForkEntry *fork_entry) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	priv->status = GRO_FORK_ENTRY_NORMAL;
	priv->sync_count = 0;
	priv->error_parent = NULL;
}


void gro_fork_entry_set_error_parent(GroForkEntry *fork_entry, GroForkEntry *error_parent) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	priv->error_parent = error_parent;
}


gboolean gro_fork_entry_is_child_of(GroForkEntry *fork_entry, GroForkEntry *error_parent) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	return priv->error_parent==error_parent;
}

int gro_fork_entry_get_sync_count(GroForkEntry *fork_entry) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	return priv->sync_count;
}

int gro_fork_entry_get_error_recovery_count(GroForkEntry *fork_entry) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	return priv->error_recovery_count;
}

int gro_fork_entry_increase_and_get_sync_count(GroForkEntry *fork_entry) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	priv->sync_count++;
	return priv->sync_count;
}



GroStateAndSymbol *gro_fork_entry_get_state_and_symbol(GroForkEntry *fork_entry) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	return priv->state_and_symbol;
}

gboolean gro_fork_entry_is_accepted(GroForkEntry *fork_entry) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	return priv->status == GRO_FORK_ENTRY_ACCEPTED || priv->status == GRO_FORK_ENTRY_ACCEPTED_WITH_ERROR;
}

void gro_fork_entry_mark_accepted(GroForkEntry *fork_entry) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	if (priv->status==GRO_FORK_ENTRY_NORMAL) {
		priv->status = GRO_FORK_ENTRY_ACCEPTED;
	} else {
		priv->status = GRO_FORK_ENTRY_ACCEPTED_WITH_ERROR;
	}
}

GroForkEntryState gro_fork_entry_get_status(GroForkEntry *fork_entry) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	return priv->status;
}

int gro_fork_entry_get_and_reset_pending_action(GroForkEntry *fork_entry) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	int result = priv->pending_action;
	priv->pending_action = 0;
	return result;
}

void gro_fork_entry_set_pending_action(GroForkEntry *fork_entry, int pending_action) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	priv->pending_action = pending_action;
}

void gro_fork_entry_set_state_and_symbol(GroForkEntry *fork_entry, GroStateAndSymbol *new_sas) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	gro_unref_ptr(priv->state_and_symbol);
	priv->state_and_symbol = new_sas;
}

void gro_fork_entry_set_content_list(GroForkEntry *fork_entry, GPtrArray *new_content_list) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	if (priv->content_stacks) {
		GPtrArray *content_list = priv->content_stacks;
		priv->content_stacks = NULL;
		l_clear_content_list(content_list);
		g_ptr_array_free(content_list, TRUE);
	}
	priv->content_stacks = new_content_list;
}


int gro_fork_entry_content_count(GroForkEntry *fork_entry) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	return priv->content_stacks->len;
}

GroContent *gro_fork_entry_content_at(GroForkEntry *fork_entry, int index) {
	GroForkEntryPrivate *priv = gro_fork_entry_get_instance_private(fork_entry);
	return (GroContent *) g_ptr_array_index(priv->content_stacks, index);
}
