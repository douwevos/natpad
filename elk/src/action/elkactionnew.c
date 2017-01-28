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
};

G_DEFINE_TYPE (ElkActionNew, elk_action_new, LEA_TYPE_ACTION)

static gpointer parent_class = NULL;

static void _dispose(GObject *object);
static void _finalize(GObject *object);
static void _action_run(LeaAction *self);

static void elk_action_new_class_init(ElkActionNewClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(ElkActionNewPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
	object_class->finalize = _finalize;

	LeaActionClass *action_clazz = LEA_ACTION_CLASS(clazz);
	action_clazz->action_run = _action_run;
}

static void elk_action_new_init(ElkActionNew *instance) {
	ElkActionNewPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, ELK_TYPE_ACTION_NEW, ElkActionNewPrivate);
	instance->priv = priv;
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkActionNew *instance = ELK_ACTION_NEW(object);
	ElkActionNewPrivate *priv = instance->priv;
	cat_unref_ptr(priv->service);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void _finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ElkActionNew *elk_action_new_new(ElkIService *service) {
	ElkActionNew *result = g_object_new(ELK_TYPE_ACTION_NEW, NULL);
	cat_ref_anounce(result);
	ElkActionNewPrivate *priv = result->priv;
	priv->service = cat_ref_ptr(service);
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("elk.new.empty"), cat_string_wo_new_with("_New"), cat_string_wo_new_with("document-new"));
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_with("Ctrl+N")));
	return result;
}


static void _action_run(LeaAction *self) {
	ElkActionNew *action = ELK_ACTION_NEW(self);
	cat_log_debug("calling select_and_open_resources");
	elk_iservice_create_empty_editor(action->priv->service);
}




