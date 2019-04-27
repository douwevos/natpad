/*
   File:    elkgroupmain.c
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

#include "elkgroupmain.h"
#include "elkgroupfile.h"
#include "elkactionabout.h"
#include "elkactionpreferences.h"
#include "../elkservice.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkGroupMain"
#include <logging/catlog.h>

struct _ElkGroupMainPrivate {
	ElkIService *service;
	ElkGroupFile *group_file;
	LeaActionGroup *group_help;
	ElkActionAbout *action_about;

	LeaActionGroup *group_window;
	ElkActionPreferences *action_preferences;
};

G_DEFINE_TYPE_WITH_PRIVATE(ElkGroupMain, elk_group_main, LEA_TYPE_ACTION_GROUP)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void elk_group_main_class_init(ElkGroupMainClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void elk_group_main_init(ElkGroupMain *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkGroupMain *instance = ELK_GROUP_MAIN(object);
	ElkGroupMainPrivate *priv = elk_group_main_get_instance_private(instance);
	cat_unref_ptr(priv->service);
	cat_unref_ptr(priv->group_file);
	cat_unref_ptr(priv->group_help);
	cat_unref_ptr(priv->group_window);
	cat_unref_ptr(priv->action_about);
	cat_unref_ptr(priv->action_preferences);
	G_OBJECT_CLASS(elk_group_main_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_group_main_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ElkGroupMain *elk_group_main_new(ElkIService *service, VipService *vip_service, LeaFrame *frame) {
	ElkGroupMain *result = g_object_new(ELK_TYPE_GROUP_MAIN, NULL);
	cat_ref_anounce(result);
	ElkGroupMainPrivate *priv = elk_group_main_get_instance_private(result);
	lea_action_group_construct((LeaActionGroup *) result, cat_string_wo_new_with("elk.action.group"), NULL);


	/* Menu 'File' */
	priv->group_file = elk_group_file_new(service);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->group_file);



	/* Menu 'Help' */
	priv->group_help = lea_action_group_new(cat_ref_ptr(lea_prov_help_group_name()), cat_string_wo_new_with("_Help"));
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->group_help);

	priv->action_about = elk_action_about_new(frame, vip_service);
	lea_action_group_add((LeaActionGroup *) priv->group_help, (LeaAction *) priv->action_about);


	/* Menu 'Window' */
	priv->group_window = lea_action_group_new(cat_ref_ptr(lea_prov_window_group_name()), cat_string_wo_new_with("_Window"));
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->group_window);

	priv->action_preferences = elk_action_preferences_new(service);
	lea_action_group_add((LeaActionGroup *) priv->group_window, (LeaAction *) priv->action_preferences);

	return result;
}

void elk_group_main_set_editor_panel(ElkGroupMain *group, DraEditorPanel *editor_panel) {
	ElkGroupMainPrivate *priv = elk_group_main_get_instance_private(group);
	elk_group_file_set_editor_panel(priv->group_file, editor_panel);
}

void elk_group_main_set_editor_list(ElkGroupMain *group, CatArrayWo *a_editor_list) {
	ElkGroupMainPrivate *priv = elk_group_main_get_instance_private(group);
	elk_group_file_set_editor_list(priv->group_file, a_editor_list);
}
