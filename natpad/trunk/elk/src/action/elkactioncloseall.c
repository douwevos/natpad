/*
   File:    elkactioncloseall.c
   Project: elk
   Author:  Douwe Vos
   Date:    Mar 13, 2012
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

#include "elkactioncloseall.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkActionCloseAll"
#include <logging/catlog.h>

struct _ElkActionCloseAllPrivate {
	ElkIService *service;
	CatArrayWo *editor_list;
};

G_DEFINE_TYPE (ElkActionCloseAll, elk_action_close_all, LEA_TYPE_ACTION)

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void elk_action_close_all_class_init(ElkActionCloseAllClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(ElkActionCloseAllPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;

	LeaActionClass *action_clazz = LEA_ACTION_CLASS(clazz);
	action_clazz->action_run = l_action_run;
}

static void elk_action_close_all_init(ElkActionCloseAll *instance) {
	ElkActionCloseAllPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, ELK_TYPE_ACTION_CLOSE_ALL, ElkActionCloseAllPrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkActionCloseAll *instance = ELK_ACTION_CLOSE_ALL(object);
	ElkActionCloseAllPrivate *priv = instance->priv;
	cat_unref_ptr(priv->service);
	cat_unref_ptr(priv->editor_list);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ElkActionCloseAll *elk_action_close_all_new(ElkIService *service) {
	ElkActionCloseAll *result = g_object_new(ELK_TYPE_ACTION_CLOSE_ALL, NULL);
	cat_ref_anounce(result);
	ElkActionCloseAllPrivate *priv = result->priv;
	priv->service = cat_ref_ptr(service);
	priv->editor_list = NULL;
	lea_action_construct(LEA_ACTION(result), cat_string_wo_new_with("elk.close.all"), cat_string_wo_new_with("Close All"), cat_string_wo_new_with("window-close"));
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_with("Ctrl+Shift+W")));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	return result;
}




static void l_action_run(LeaAction *self) {
	ElkActionCloseAll *action = ELK_ACTION_CLOSE_ALL(self);
	ElkActionCloseAllPrivate *priv = ELK_ACTION_CLOSE_ALL_GET_PRIVATE(action);
	elk_iservice_close_multiple_resource_editors(priv->service, NULL);
}


void elk_action_close_all_set_editor_list(ElkActionCloseAll *action, CatArrayWo *a_editor_list) {
	ElkActionCloseAll *elk_action = ELK_ACTION_CLOSE_ALL(action);
	ElkActionCloseAllPrivate *priv = ELK_ACTION_CLOSE_ALL_GET_PRIVATE(elk_action);
	if (priv->editor_list == a_editor_list) {
		return;
	}
	cat_ref_swap(priv->editor_list, a_editor_list);
	gboolean is_active = FALSE;
	if (a_editor_list) {
		if (cat_array_wo_size(a_editor_list)>0) {
			is_active = TRUE;
		}
	}
	lea_action_set_sensitive_self((LeaAction *) action, is_active);
}




