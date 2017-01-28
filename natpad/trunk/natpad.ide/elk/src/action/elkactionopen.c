/*
   File:    elkactionopen.c
   Project: elk
   Author:  Douwe Vos
   Date:    Mar 8, 2012
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



#include "elkactionopen.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkActionOpen"
#include <logging/catlog.h>

struct _ElkActionOpenPrivate {
	ElkIService *service;
};

G_DEFINE_TYPE (ElkActionOpen, elk_action_open, LEA_TYPE_ACTION)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void elk_action_open_class_init(ElkActionOpenClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(ElkActionOpenPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_clazz = LEA_ACTION_CLASS(clazz);
	action_clazz->action_run = l_action_run;
}

static void elk_action_open_init(ElkActionOpen *instance) {
	ElkActionOpenPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, ELK_TYPE_ACTION_OPEN, ElkActionOpenPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkActionOpen *instance = ELK_ACTION_OPEN(object);
	ElkActionOpenPrivate *priv = instance->priv;
	cat_unref_ptr(priv->service);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ElkActionOpen *elk_action_open_new(ElkIService *service) {
	ElkActionOpen *result = g_object_new(ELK_TYPE_ACTION_OPEN, NULL);
	cat_ref_anounce(result);
	ElkActionOpenPrivate *priv = result->priv;
	priv->service = cat_ref_ptr(service);
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("elk.open.resource"), cat_string_wo_new_with("Open _File..."), cat_string_wo_new_with("document-open"));
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_with("Ctrl+O")));
	return result;
}



static void l_action_run(LeaAction *self) {
	ElkActionOpen *action = ELK_ACTION_OPEN(self);
	cat_log_debug("calling select_and_open_resources");
	elk_iservice_select_and_open_resources(action->priv->service);
}


