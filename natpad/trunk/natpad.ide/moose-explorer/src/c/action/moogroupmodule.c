/*
   File:    moogroupmodule.c
   Project: moose
   Author:  Douwe Vos
   Date:    Nov 24, 2013
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


#include "moogroupmodule.h"
#include "mooactionaddproject.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooGroupModule"
#include <logging/catlog.h>

struct _MooGroupModulePrivate {
	void *dummy;
};


G_DEFINE_TYPE(MooGroupModule, moo_group_module, LEA_TYPE_ACTION_GROUP)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_group_module_class_init(MooGroupModuleClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(MooGroupModulePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_group_module_init(MooGroupModule *instance) {
	MooGroupModulePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_GROUP_MODULE, MooGroupModulePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	MooGroupModule *instance = MOO_GROUP_MODULE(object);
//	MooGroupModulePrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void moo_group_module_construct(MooGroupModule *group, MooService *moo_service, LeaFrame *frame) {
//	MooGroupModulePrivate *priv = MOO_GROUP_MODULE_GET_PRIVATE(group);
	LeaActionGroup *lea_group = LEA_ACTION_GROUP(group);
	lea_action_group_construct(lea_group, cat_string_wo_new_with("moo.module.group"), cat_string_wo_new_with("_Module"));
	MooActionAddProject *action = moo_action_add_project_new(moo_service);
	lea_action_group_add((LeaActionGroup *) group, (LeaAction *) action);
	cat_unref_ptr(action);
}


MooGroupModule *moo_group_module_new(MooService *moo_service, LeaFrame *frame) {
	MooGroupModule *result = g_object_new(MOO_TYPE_GROUP_MODULE, NULL);
	cat_ref_anounce(result);
//	MooGroupModulePrivate *priv = result->priv;
	moo_group_module_construct(result, moo_service, frame);
	return result;
}


LeaAction *moo_group_module_get_action_add_project(MooGroupModule *group_module) {
	return lea_action_group_get(LEA_ACTION_GROUP(group_module), CAT_S(moo_s_action_add_project));
}
