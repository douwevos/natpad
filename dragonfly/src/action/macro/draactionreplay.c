/*
   File:    draactionreplay.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 23, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "draactionreplay.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraActionReplay"
#include <logging/catlog.h>

struct _DraActionReplayPrivate {
	DraMacroManager *macro_manager;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraActionReplay, dra_action_replay, LEA_TYPE_ACTION,
		G_ADD_PRIVATE(DraActionReplay)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void dra_action_replay_class_init(DraActionReplayClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void dra_action_replay_init(DraActionReplay *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraActionReplay *instance = DRA_ACTION_REPLAY(object);
	DraActionReplayPrivate *priv = dra_action_replay_get_instance_private(instance);
	cat_unref_ptr(priv->macro_manager);
	G_OBJECT_CLASS(dra_action_replay_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_action_replay_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraActionReplay *dra_action_replay_new(DraMacroManager *macro_manager) {
	DraActionReplay *result = g_object_new(DRA_TYPE_ACTION_REPLAY, NULL);
	cat_ref_anounce(result);
	DraActionReplayPrivate *priv = dra_action_replay_get_instance_private(result);
	lea_action_construct(LEA_ACTION(result), cat_string_wo_new_with("dragonfly.replay"), cat_string_wo_new_with("Replay"), NULL);
	priv->macro_manager = cat_ref_ptr(macro_manager);
	return result;
}


static void l_action_run(LeaAction *self) {
	DraActionReplay *action = DRA_ACTION_REPLAY(self);
	DraActionReplayPrivate *priv = dra_action_replay_get_instance_private(action);
	dra_macro_manager_replay(priv->macro_manager);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
