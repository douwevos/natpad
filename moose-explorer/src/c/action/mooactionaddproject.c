/*
   File:    mooactionaddproject.c
   Project: moose
   Author:  Douwe Vos
   Date:    Nov 24, 2013
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

#include "mooactionaddproject.h"
#include "../dialog/mooaddprojectdialog.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooActionAddProject"
#include <logging/catlog.h>

struct _MooActionAddProjectPrivate {
	MooService *moo_service;
};

G_DEFINE_TYPE_WITH_PRIVATE(MooActionAddProject, moo_action_add_project, LEA_TYPE_ACTION)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

CatS moo_s_action_add_project = CAT_S_DEF("moose.add.project");

static void moo_action_add_project_class_init(MooActionAddProjectClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_clazz = LEA_ACTION_CLASS(clazz);
	action_clazz->action_run = l_action_run;
}

static void moo_action_add_project_init(MooActionAddProject *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooActionAddProject *instance = MOO_ACTION_ADD_PROJECT(object);
	MooActionAddProjectPrivate *priv = moo_action_add_project_get_instance_private(instance);
	cat_unref_ptr(priv->moo_service);
	G_OBJECT_CLASS(moo_action_add_project_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_action_add_project_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

MooActionAddProject *moo_action_add_project_new(MooService *moo_service) {
	MooActionAddProject *result = g_object_new(MOO_TYPE_ACTION_ADD_PROJECT, NULL);
	cat_ref_anounce(result);
	MooActionAddProjectPrivate *priv = moo_action_add_project_get_instance_private(result);
	lea_action_construct((LeaAction *) result, CAT_S(moo_s_action_add_project), cat_string_wo_new_with("Add _Module"), cat_string_wo_new_with("list-add"));
	priv->moo_service = cat_ref_ptr(moo_service);
	return result;
}

VipNodePath *l_open_path(MooService *moo_service, VipPath *fullpath) {
	VipService *vip_service = moo_service_get_viper_service(moo_service);
	VipNodePath *result = NULL;
	VipCreatePathRequest *create_path_request = vip_service_create_path(vip_service, fullpath);
	result = vip_create_path_request_wait_for_path(create_path_request);
	if (result!=NULL) {
		cat_ref_ptr(result);
		VipNode *tail = vip_node_path_get_tail(result);

		cat_log_debug("request=%o", create_path_request);

		vip_node_recursive_up(tail);
		vip_service_scan(vip_service);
	}
	cat_unref_ptr(create_path_request);
	return result;
}

static void l_action_run(LeaAction *self) {
	MooActionAddProject *instance = MOO_ACTION_ADD_PROJECT(self);
	MooActionAddProjectPrivate *priv = moo_action_add_project_get_instance_private(instance);
	MooAddProjectDialog *project_dialog = moo_add_project_dialog_new();
//	while(TRUE) {
		int result = moo_add_project_dialog_run(project_dialog);
//	}
	if (result==1) {
		CatStringWo *base_path = moo_add_project_dialog_get_base_path(project_dialog);
		VipPath *vip_base_path = vip_path_new(cat_ref_ptr(base_path));

		VipNodePath *module_path = l_open_path(priv->moo_service, vip_base_path);
		MooAddModule *add_module = moo_add_module_new(priv->moo_service, module_path);
		cat_unref_ptr(module_path);

		VipService *vip_service = moo_service_get_viper_service(priv->moo_service);
		vip_service_post(vip_service, (WorRequest *) add_module);
		cat_unref_ptr(add_module);
		cat_unref_ptr(vip_base_path);
	}

	gtk_widget_hide(GTK_WIDGET(project_dialog));
	gtk_widget_destroy(GTK_WIDGET(project_dialog));
	cat_log_debug("finished running add_project");
}
