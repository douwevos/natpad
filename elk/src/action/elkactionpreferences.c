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

struct _ElkActionPreferencesPrivate {
	ElkIService *service;
};

G_DEFINE_TYPE_WITH_PRIVATE(ElkActionPreferences, elk_action_preferences, LEA_TYPE_ACTION)

static void l_dispose(GObject *object);
static void l_action_run(LeaAction *action);

static void elk_action_preferences_class_init(ElkActionPreferencesClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	LeaActionClass *action_clazz = LEA_ACTION_CLASS(clazz);
	action_clazz->action_run = l_action_run;
}

static void elk_action_preferences_init(ElkActionPreferences *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkActionPreferences *instance = ELK_ACTION_PREFERENCES(object);
	ElkActionPreferencesPrivate *priv = elk_action_preferences_get_instance_private(instance);
	cat_unref_ptr(priv->service);
	G_OBJECT_CLASS(elk_action_preferences_parent_class)->dispose(object);
	cat_log_detail("end-dispose:%p", object);
}

ElkActionPreferences *elk_action_preferences_new(ElkIService *service) {
	ElkActionPreferences *result = g_object_new(ELK_TYPE_ACTION_PREFERENCES, NULL);
	cat_ref_anounce(result);
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("elk.preferences"), cat_string_wo_new_with("_Preferences"), cat_string_wo_new_with("preferences-system"));
	ElkActionPreferencesPrivate *priv = elk_action_preferences_get_instance_private(result);
	priv->service = cat_ref_ptr(service);
	return result;
}

static void l_action_run(LeaAction *self) {
	ElkActionPreferences *action = ELK_ACTION_PREFERENCES(self);
	ElkActionPreferencesPrivate *priv = elk_action_preferences_get_instance_private(action);
	elk_iservice_show_preferences(priv->service);
}
