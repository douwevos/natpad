/*
   File:    elkactionpreferences.c
   Project: elk
   Author:  Douwe Vos
   Date:    May 5, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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


#include "elkactionpreferences.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkActionPreferences"
#include <logging/catlog.h>

G_DEFINE_TYPE (ElkActionPreferences, elk_action_preferences, LEA_TYPE_ACTION)

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _action_run(LeaAction *self);

static void elk_action_preferences_class_init(ElkActionPreferencesClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	LeaActionClass *action_clazz = LEA_ACTION_CLASS(clazz);
	action_clazz->action_run = _action_run;
}

static void elk_action_preferences_init(ElkActionPreferences *instance) {
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkActionPreferences *instance = ELK_ACTION_PREFERENCES(object);
	cat_unref_ptr(instance->service);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("end-dispose:%p", object);
}


ElkActionPreferences *elk_action_preferences_new(ElkIService *service) {
	ElkActionPreferences *result = g_object_new(ELK_TYPE_ACTION_PREFERENCES, NULL);
	cat_ref_anounce(result);
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("elk.preferences"), cat_string_wo_new_with("_Preferences"), cat_string_wo_new_with("preferences-system"));
	result->service = cat_ref_ptr(service);
	return result;
}

static void _action_run(LeaAction *self) {
	ElkActionPreferences *action = ELK_ACTION_PREFERENCES(self);
	elk_iservice_show_preferences(action->service);
}