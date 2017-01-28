/*
   File:    leaprovgroupmain.c
   Project: leafhopper
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


#include "leaprovgroupmain.h"
#include "leaprovided.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaProvGroupMain"
#include <logging/catlog.h>

struct _LeaProvGroupMainPrivate {
	void *dummy;
};

G_DEFINE_TYPE(LeaProvGroupMain, lea_prov_group_main, LEA_TYPE_ACTION_GROUP)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_prov_group_main_class_init(LeaProvGroupMainClass *clazz) {
	g_type_class_add_private(clazz, sizeof(LeaProvGroupMainPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_prov_group_main_init(LeaProvGroupMain *instance) {
	LeaProvGroupMainPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, LEA_TYPE_PROV_GROUP_MAIN, LeaProvGroupMainPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	LeaProvGroupMain *instance = LEA_PROV_GROUP_MAIN(object);
//	LeaProvGroupMainPrivate *priv = instance->priv;
	G_OBJECT_CLASS(lea_prov_group_main_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_prov_group_main_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaProvGroupMain *lea_prov_group_main_new() {
	LeaProvGroupMain *result = g_object_new(LEA_TYPE_PROV_GROUP_MAIN, NULL);
	cat_ref_anounce(result);
//	LeaProvGroupMainPrivate *priv = result->priv;
	lea_action_group_construct((LeaActionGroup *) result, cat_string_wo_new_with("lea.prov.group.main"), NULL);

	LeaActionGroup *group_file = lea_action_group_new(cat_ref_ptr(lea_prov_file_group_name()), cat_string_wo_new_with("_File"));
	lea_action_set_order((LeaAction *) group_file, -100);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) group_file);
	cat_unref(group_file);

	LeaActionGroup *group_edit = lea_action_group_new(cat_ref_ptr(lea_prov_edit_group_name()), cat_string_wo_new_with("_Edit"));
	lea_action_set_order((LeaAction *) group_edit, -90);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) group_edit);
	cat_unref(group_edit);


	LeaActionGroup *group_search = lea_action_group_new(cat_ref_ptr(lea_prov_search_group_name()), cat_string_wo_new_with("_Search"));
	lea_action_set_order((LeaAction *) group_search, -80);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) group_search);
	cat_unref(group_search);


	LeaActionGroup *group_navigate = lea_action_group_new(cat_ref_ptr(lea_prov_navigate_group_name()), cat_string_wo_new_with("_Navigate"));
	lea_action_set_order((LeaAction *) group_navigate, -70);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) group_navigate);
	cat_unref(group_navigate);


	LeaActionGroup *group_window = lea_action_group_new(cat_ref_ptr(lea_prov_window_group_name()), cat_string_wo_new_with("_Window"));
	lea_action_set_order((LeaAction *) group_window, 50);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) group_window);
	cat_unref(group_window);


	LeaActionGroup *group_help = lea_action_group_new(cat_ref_ptr(lea_prov_help_group_name()), cat_string_wo_new_with("_Help"));
	lea_action_set_order((LeaAction *) group_help, 100);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) group_help);
	cat_unref(group_help);

	return result;
}









