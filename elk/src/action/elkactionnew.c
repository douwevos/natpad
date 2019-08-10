/*
   File:    elkactionnew.c
   Project: elk
   Author:  Douwe Vos
   Date:    Mar 10, 2012
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

#include "elkactionnew.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkActionNew"
#include <logging/catlog.h>

struct _ElkActionNewPrivate {
	ElkIService *service;
	ElkIResourceEditorFactory *factory;
};

G_DEFINE_TYPE_WITH_PRIVATE(ElkActionNew, elk_action_new, LEA_TYPE_ACTION)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *action);

static void elk_action_new_class_init(ElkActionNewClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_clazz = LEA_ACTION_CLASS(clazz);
	action_clazz->action_run = l_action_run;
}

static void elk_action_new_init(ElkActionNew *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkActionNew *instance = ELK_ACTION_NEW(object);
	ElkActionNewPrivate *priv = elk_action_new_get_instance_private(instance);
	cat_unref_ptr(priv->service);
	cat_unref_ptr(priv->factory);
	G_OBJECT_CLASS(elk_action_new_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_action_new_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ElkActionNew *elk_action_new_new(ElkIService *service) {
	ElkActionNew *result = g_object_new(ELK_TYPE_ACTION_NEW, NULL);
	cat_ref_anounce(result);
	ElkActionNewPrivate *priv = elk_action_new_get_instance_private(result);
	priv->service = cat_ref_ptr(service);
	priv->factory = NULL;
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("elk.new.empty"), cat_string_wo_new_with("_New"), cat_string_wo_new_with("document-new"));
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_with("Ctrl+N")));
	return result;
}

ElkActionNew *elk_action_new_for_new(ElkIService *service, CatStringWo *name, ElkIResourceEditorFactory *factory) {
	ElkActionNew *result = g_object_new(ELK_TYPE_ACTION_NEW, NULL);
	cat_ref_anounce(result);
	ElkActionNewPrivate *priv = elk_action_new_get_instance_private(result);
	priv->service = cat_ref_ptr(service);
	priv->factory = cat_ref_ptr(factory);
	CatStringWo *n = cat_string_wo_new_with("elk.new.empty.for.");
	cat_string_wo_append(n, name);
	n = cat_string_wo_anchor(n,0);
	lea_action_construct((LeaAction *) result, n, name, cat_string_wo_new_with("document-new"));
	return result;
}


static void l_action_run(LeaAction *self) {
	ElkActionNew *action = ELK_ACTION_NEW(self);
	ElkActionNewPrivate *priv = elk_action_new_get_instance_private(action);
	cat_log_debug("calling select_and_open_resources");
	if (priv->factory) {
		elk_iservice_create_empty_editor_for(priv->service, priv->factory);
	} else {
		elk_iservice_create_empty_editor(priv->service);
	}
}
