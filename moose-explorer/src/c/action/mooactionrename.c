/*
   File:    mooactionrename.c
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Apr 3, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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


#include "mooactionrename.h"
#include "../moonodelayout.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooActionRename"
#include <logging/catlog.h>

struct _MooActionRenamePrivate {
	MooService *moo_service;
	CatArrayWo *a_selection;
	LeaFrame *frame;
};


G_DEFINE_TYPE(MooActionRename, moo_action_rename, LEA_TYPE_ACTION)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void moo_action_rename_class_init(MooActionRenameClass *clazz) {
	g_type_class_add_private(clazz, sizeof(MooActionRenamePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void moo_action_rename_init(MooActionRename *instance) {
	MooActionRenamePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_ACTION_RENAME, MooActionRenamePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooActionRename *instance = MOO_ACTION_RENAME(object);
	MooActionRenamePrivate *priv = instance->priv;
	cat_unref_ptr(priv->moo_service);
	cat_unref_ptr(priv->a_selection);
	cat_unref_ptr(priv->frame);
	G_OBJECT_CLASS(moo_action_rename_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_action_rename_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooActionRename *moo_action_rename_new(MooService *moo_service, LeaFrame *frame) {
	MooActionRename *result = g_object_new(MOO_TYPE_ACTION_RENAME, NULL);
	cat_ref_anounce(result);
	MooActionRenamePrivate *priv = result->priv;
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->a_selection = NULL;
	priv->frame = cat_ref_ptr(frame);
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("moose.clipboard.rename"), cat_string_wo_new_with("rename"), NULL);
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_data("F2")));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	return result;
}




void moo_action_rename_set_selection(MooActionRename *action_rename, CatArrayWo *selection) {
	MooActionRenamePrivate *priv = MOO_ACTION_RENAME_GET_PRIVATE(action_rename);
	if (priv->a_selection == selection) {
		return;
	}
	cat_ref_swap(priv->a_selection, selection);
	gboolean has_renameable_node = FALSE;
	if (selection!=NULL && (cat_array_wo_size(selection)==1)) {
		MooNodeLayout *node_layout = (MooNodeLayout *) cat_array_wo_get_first(priv->a_selection);
		MooNodeWo *node = moo_node_layout_get_node(node_layout);
		MooIServicesContent *serivces_content = (MooIServicesContent *) moo_node_wo_get_content(node, moo_iservices_content_key());
		if ((serivces_content!=NULL) && moo_iservices_content_can_rename(serivces_content)) {
			has_renameable_node = TRUE;
		}
	}

	lea_action_set_sensitive_self(LEA_ACTION(action_rename), has_renameable_node);
}



static void l_action_run(LeaAction *self) {
	MooActionRenamePrivate *priv = MOO_ACTION_RENAME_GET_PRIVATE(self);
	if (priv->a_selection) {
		MooNodeLayout *node_layout = (MooNodeLayout *) cat_array_wo_get_first(priv->a_selection);
		MooNodeWo *node = moo_node_layout_get_node(node_layout);
		MooIServicesContent *serivces_content = (MooIServicesContent *) moo_node_wo_get_content(node, moo_iservices_content_key());
		if ((serivces_content!=NULL) && moo_iservices_content_can_rename(serivces_content)) {
			moo_iservices_content_rename(serivces_content, priv->moo_service, node, priv->frame);
		}
//		MooEditableNode *editable_node = moo_editable_node_new(node, FALSE);
//		moo_clipboard_cut_or_copy(priv->moo_clipboard, priv->selection, TRUE);
	} else {
		cat_log_warn("no renameable selection");
		lea_action_set_sensitive_self(LEA_ACTION(self), FALSE);
	}

}



