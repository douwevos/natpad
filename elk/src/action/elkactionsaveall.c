/*
   File:    elkactionsaveall.c
   Project: elk
   Author:  Douwe Vos
   Date:    Mar 12, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "elkactionsaveall.h"
#include <caterpillar.h>
#include <dragonfly.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkActionSaveAll"
#include <logging/catlog.h>

struct _ElkActionSaveAllPrivate {
	ElkIService *service;
	CatArrayWo *a_editor_list;
};

G_DEFINE_TYPE (ElkActionSaveAll, elk_action_save_all, LEA_TYPE_ACTION)

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);
static void l_action_run(LeaAction *self);
static void l_action_refresh(LeaAction *self);

static void elk_action_save_all_class_init(ElkActionSaveAllClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(ElkActionSaveAllPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;

	LeaActionClass *action_clazz = LEA_ACTION_CLASS(clazz);
	action_clazz->action_run = l_action_run;
	action_clazz->refresh = l_action_refresh;
}

static void elk_action_save_all_init(ElkActionSaveAll *instance) {
	ElkActionSaveAllPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, ELK_TYPE_ACTION_SAVE_ALL, ElkActionSaveAllPrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkActionSaveAll *instance = ELK_ACTION_SAVE_ALL(object);
	ElkActionSaveAllPrivate *priv = instance->priv;
	cat_unref_ptr(priv->service);
	cat_unref_ptr(priv->a_editor_list);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ElkActionSaveAll *elk_action_save_all_new(ElkIService *service) {
	ElkActionSaveAll *result = g_object_new(ELK_TYPE_ACTION_SAVE_ALL, NULL);
	cat_ref_anounce(result);
	ElkActionSaveAllPrivate *priv = result->priv;
	priv->service = cat_ref_ptr(service);
	priv->a_editor_list = NULL;
	lea_action_construct(LEA_ACTION(result), cat_string_wo_new_with("elk.save.all"), cat_string_wo_new_with("Save _All"), cat_string_wo_new_with("document-save"));
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_with("Ctrl+Shift+S")));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	return result;
}



static void l_action_run(LeaAction *self) {
	ElkActionSaveAll *action = ELK_ACTION_SAVE_ALL(self);
	ElkActionSaveAllPrivate *priv = ELK_ACTION_SAVE_ALL_GET_PRIVATE(action);
	elk_iservice_save_all_resource_editors(priv->service, NULL);
}


static void l_recheck_list(ElkActionSaveAll *action) {
	ElkActionSaveAllPrivate *priv = ELK_ACTION_SAVE_ALL_GET_PRIVATE(action);
	gboolean has_unsaved = FALSE;
	if (priv->a_editor_list) {
		CatIIterator *iterator = cat_array_wo_iterator(priv->a_editor_list);
		while(cat_iiterator_has_next(iterator)) {
			gpointer *next = cat_iiterator_next(iterator);
			if (DRA_IS_EDITOR_PANEL(next)) {
				DraEditorPanel *editor = (DraEditorPanel *) next;
				if (!DRA_EDITOR_PANEL_GET_CLASS(editor)->isSaved(editor)) {
					has_unsaved = TRUE;
					break;
				}
			}
		}
		cat_unref_ptr(iterator);
	}
	lea_action_set_sensitive_self(LEA_ACTION(action), has_unsaved);
}

static void l_action_refresh(LeaAction *self) {
	l_recheck_list((ELK_ACTION_SAVE_ALL(self)));
}


void elk_action_save_all_set_editor_list(ElkActionSaveAll *action, CatArrayWo *a_editor_list) {
	ElkActionSaveAll *elk_action = ELK_ACTION_SAVE_ALL(action);
	ElkActionSaveAllPrivate *priv = ELK_ACTION_SAVE_ALL_GET_PRIVATE(elk_action);
	if (priv->a_editor_list == a_editor_list) {
		return;
	}
	cat_ref_swap(priv->a_editor_list, a_editor_list);
	l_recheck_list(elk_action);
}
