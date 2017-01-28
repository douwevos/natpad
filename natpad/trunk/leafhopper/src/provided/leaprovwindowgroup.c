/*
   File:    leaprovwindowgroup.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Dec 7, 2013
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

#include "leaprovwindowgroup.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaProvWindowGroup"
#include <logging/catlog.h>

struct _LeaProvWindowGroupPrivate {
	void *dummy;
};


G_DEFINE_TYPE(LeaProvWindowGroup, lea_prov_window_group, LEA_TYPE_ACTION_GROUP)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_prov_window_group_class_init(LeaProvWindowGroupClass *clazz) {
	g_type_class_add_private(clazz, sizeof(LeaProvWindowGroupPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_prov_window_group_init(LeaProvWindowGroup *instance) {
	LeaProvWindowGroupPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, LEA_TYPE_PROV_WINDOW_GROUP, LeaProvWindowGroupPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	LeaProvWindowGroup *instance = LEA_PROV_WINDOW_GROUP(object);
//	LeaProvWindowGroupPrivate *priv = instance->priv;
	G_OBJECT_CLASS(lea_prov_window_group_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_prov_window_group_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaProvWindowGroup *lea_prov_window_group_new() {
	LeaProvWindowGroup *result = g_object_new(LEA_TYPE_PROV_WINDOW_GROUP, NULL);
	cat_ref_anounce(result);
//	LeaProvWindowGroupPrivate *priv = result->priv;
	lea_action_group_construct((LeaActionGroup *) result, cat_string_wo_new_with("lea.prov.window.group"), cat_string_wo_new_with("_Window"));

	LeaProvShowViewGroup *show_view_group = lea_prov_show_view_group_new();

	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) show_view_group);

	return result;
}

