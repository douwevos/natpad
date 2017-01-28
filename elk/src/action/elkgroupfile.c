/*
   File:    elkgroupfile.c
   Project: elk
   Author:  Douwe Vos
   Date:    Dec 23, 2013
   Web:     http://www.natpad.net/
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

#include "elkactionabout.h"
#include "elkgroupfile.h"
#include "elkactionnew.h"
#include "elkactionopen.h"
#include "elkactionrevert.h"
#include "elkactionclose.h"
#include "elkactioncloseall.h"
#include "elkactionsave.h"
#include "elkactionsaveas.h"
#include "elkactionsaveall.h"
#include "elkactionexit.h"

#include "../elkservice.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkGroupFile"
#include <logging/catlog.h>


struct _ElkGroupFilePrivate {
	ElkIService *service;
	DraEditorPanel *focused_editor;

	ElkActionNew *action_new;
	ElkActionOpen *action_open;
	ElkActionRevert *action_revert;

	ElkActionClose *action_close;
	ElkActionCloseAll *action_close_all;

	ElkActionSave *action_save;
	ElkActionSaveAs *action_save_as;
	ElkActionSaveAll *action_save_all;

	ElkActionExit *action_exit;
};

G_DEFINE_TYPE (ElkGroupFile, elk_group_file, LEA_TYPE_ACTION_GROUP)

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);

static void elk_group_file_class_init(ElkGroupFileClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(ElkGroupFilePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;
}

static void elk_group_file_init(ElkGroupFile *instance) {
	ElkGroupFilePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, ELK_TYPE_GROUP_FILE, ElkGroupFilePrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkGroupFile *instance = ELK_GROUP_FILE(object);
	ElkGroupFilePrivate *priv = instance->priv;
	cat_unref_ptr(priv->focused_editor);
	cat_unref_ptr(priv->service);
	cat_unref_ptr(priv->action_new);
	cat_unref_ptr(priv->action_open);
	cat_unref_ptr(priv->action_revert);
	cat_unref_ptr(priv->action_close);
	cat_unref_ptr(priv->action_close_all);
	cat_unref_ptr(priv->action_save);
	cat_unref_ptr(priv->action_save_as);
	cat_unref_ptr(priv->action_save_all);
	cat_unref_ptr(priv->action_exit);

	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ElkGroupFile *elk_group_file_new(ElkIService *service) {
	ElkGroupFile *result = g_object_new(ELK_TYPE_GROUP_FILE, NULL);
	cat_ref_anounce(result);
	ElkGroupFilePrivate *priv = result->priv;
	lea_action_group_construct((LeaActionGroup *) result, cat_ref_ptr(lea_prov_file_group_name()), cat_string_wo_new_with("_File"));
	priv->service = cat_ref_ptr(service);
	priv->focused_editor = NULL;

	priv->action_new = elk_action_new_new(service);
	lea_action_set_order((LeaAction *) priv->action_new, -100);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->action_new);


	LeaActionGroup *grp_open = lea_action_group_create_sub((LeaActionGroup *) result, cat_string_wo_new_with("elk.file.open.group"), NULL);
	lea_action_set_order((LeaAction *) grp_open, -90);

	priv->action_open = elk_action_open_new(service);
	lea_action_set_order((LeaAction *) priv->action_open, -100);
	lea_action_group_add(grp_open, (LeaAction *) priv->action_open);
	priv->action_revert = elk_action_revert_new(service);
	lea_action_set_order((LeaAction *) priv->action_revert, -90);
	lea_action_group_add(grp_open, (LeaAction *) priv->action_revert);

	cat_unref(grp_open);


	LeaActionGroup *grp_close = lea_action_group_create_sub((LeaActionGroup *) result, cat_string_wo_new_with("elk.file.close.group"), NULL);
	lea_action_set_order((LeaAction *) grp_close, -80);

	priv->action_close = elk_action_close_new();
	lea_action_set_order((LeaAction *) priv->action_close, -100);
	lea_action_group_add(grp_close, (LeaAction *) priv->action_close);
	priv->action_close_all = elk_action_close_all_new(service);
	lea_action_set_order((LeaAction *) priv->action_close_all, -90);
	lea_action_group_add(grp_close, (LeaAction *) priv->action_close_all);

	cat_unref(grp_close);



	LeaActionGroup *grp_save = lea_action_group_create_sub((LeaActionGroup *) result, cat_string_wo_new_with("elk.file.save.group"), NULL);
	lea_action_set_order((LeaAction *) grp_save, -70);

	priv->action_save = elk_action_save_new();
	lea_action_set_order((LeaAction *) priv->action_save, -100);
	lea_action_group_add(grp_save, (LeaAction *) priv->action_save);
	priv->action_save_as = elk_action_save_as_new();
	lea_action_set_order((LeaAction *) priv->action_save_as, -90);
	lea_action_group_add(grp_save, (LeaAction *) priv->action_save_as);
	priv->action_save_all = elk_action_save_all_new(service);
	lea_action_set_order((LeaAction *) priv->action_save_all, -80);
	lea_action_group_add(grp_save, (LeaAction *) priv->action_save_all);

	cat_unref(grp_save);


	priv->action_exit = elk_action_exit_new(service);
	lea_action_set_order((LeaAction *) priv->action_exit, 1000);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->action_exit);

	return result;
}



void elk_group_file_set_editor_panel(ElkGroupFile *group, DraEditorPanel *editor_panel) {
	ElkGroupFilePrivate *priv = ELK_GROUP_FILE_GET_PRIVATE(group);
	if (priv->focused_editor == editor_panel) {
		return;
	}

	cat_ref_swap(priv->focused_editor, editor_panel);

	elk_action_save_set_editor_panel(priv->action_save, editor_panel);
	elk_action_save_as_set_editor_panel(priv->action_save_as, editor_panel);
	elk_action_close_set_editor_panel(priv->action_close, editor_panel);
	elk_action_revert_set_editor_panel(priv->action_revert, editor_panel);
}


void elk_group_file_set_editor_list(ElkGroupFile *group, CatArrayWo *a_editor_list) {
	ElkGroupFilePrivate *priv = ELK_GROUP_FILE_GET_PRIVATE(group);
	elk_action_save_all_set_editor_list(priv->action_save_all, a_editor_list);
	elk_action_close_all_set_editor_list(priv->action_close_all, a_editor_list);
}

