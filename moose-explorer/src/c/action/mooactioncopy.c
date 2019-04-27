/*
   File:    mooactioncopy.c
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Dec 25, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#include "mooactioncopy.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooActionCopy"
#include <logging/catlog.h>

struct _MooActionCopyPrivate {
	gboolean has_copyable_selection;
	MooClipboard *moo_clipboard;
	CatArrayWo *selection;
};

G_DEFINE_TYPE_WITH_PRIVATE(MooActionCopy, moo_action_copy, LEA_TYPE_ACTION)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void moo_action_copy_class_init(MooActionCopyClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void moo_action_copy_init(MooActionCopy *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooActionCopy *instance = MOO_ACTION_COPY(object);
	MooActionCopyPrivate *priv = moo_action_copy_get_instance_private(instance);
	cat_unref_ptr(priv->moo_clipboard);
	cat_unref_ptr(priv->selection);
	G_OBJECT_CLASS(moo_action_copy_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_action_copy_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

MooActionCopy *moo_action_copy_new(MooClipboard *moo_clipboard) {
	MooActionCopy *result = g_object_new(MOO_TYPE_ACTION_COPY, NULL);
	cat_ref_anounce(result);
	MooActionCopyPrivate *priv = moo_action_copy_get_instance_private(result);
	priv->has_copyable_selection = FALSE;
	priv->moo_clipboard = cat_ref_ptr(moo_clipboard);
	priv->selection = NULL;
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("moose.clipboard.copy"), cat_string_wo_new_with("_Copy resources"), NULL);
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_data("Ctrl+C")));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	return result;
}

void moo_action_copy_set_has_copyable_selection(MooActionCopy *action_copy, gboolean has_copyable_selection, CatArrayWo *selection) {
	MooActionCopyPrivate *priv = moo_action_copy_get_instance_private(action_copy);
	if (priv->has_copyable_selection == has_copyable_selection && priv->selection == selection) {
		return;
	}
	priv->has_copyable_selection = has_copyable_selection;
	cat_ref_swap(priv->selection, selection);
	lea_action_set_sensitive_self(LEA_ACTION(action_copy), has_copyable_selection);
}

static void l_action_run(LeaAction *self) {
	MooActionCopy *instance = MOO_ACTION_COPY(self);
	MooActionCopyPrivate *priv = moo_action_copy_get_instance_private(instance);
	if (priv->has_copyable_selection) {
		moo_clipboard_cut_or_copy(priv->moo_clipboard, priv->selection, TRUE);
	} else {
		cat_log_warn("no copyable selection");
		lea_action_set_sensitive_self(LEA_ACTION(self), FALSE);
	}
}
