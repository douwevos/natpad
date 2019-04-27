/*
   File:    leaoutputactionreset.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Dec 1, 2013
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

#include "leaoutputactionreset.h"
#include "leaoutputpanelowner.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaOutputActionReset"
#include <logging/catlog.h>

struct _LeaOutputActionResetPrivate {
	LeaOutputPanelOwner *owner;
};


G_DEFINE_TYPE_WITH_PRIVATE(LeaOutputActionReset, lea_output_action_reset, LEA_TYPE_ACTION)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self) {
	LeaOutputActionReset *instance = LEA_OUTPUT_ACTION_RESET(self);
	LeaOutputActionResetPrivate *priv = lea_output_action_reset_get_instance_private(instance);
	lea_frame_unmerge_action_group(priv->owner->frame, priv->owner->action_group);
}


static void lea_output_action_reset_class_init(LeaOutputActionResetClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;


	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void lea_output_action_reset_init(LeaOutputActionReset *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	LeaOutputActionReset *instance = LEA_OUTPUT_ACTION_RESET(object);
//	LeaOutputActionResetPrivate *priv = lea_output_action_reset_get_instance_private(instance);
	G_OBJECT_CLASS(lea_output_action_reset_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_output_action_reset_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaOutputActionReset *lea_output_action_reset_new(LeaIPanelOwner *owner) {
	LeaOutputActionReset *result = g_object_new(LEA_TYPE_OUTPUT_ACTION_RESET, NULL);
	cat_ref_anounce(result);
	LeaOutputActionResetPrivate *priv = lea_output_action_reset_get_instance_private(result);
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("do reset"), cat_string_wo_new_with("Do a reset"), NULL);
	LeaKeySequence *seq = lea_key_sequence_from_string(cat_string_wo_new_with("Ctrl+X"));
	cat_log_debug("seq=%o", seq);
	lea_action_set_default_key_sequence((LeaAction *) result, seq);
	priv->owner = (LeaOutputPanelOwner *) owner;
	return result;
}
