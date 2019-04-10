/*
   File:    dragroupmacro.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 21, 2016
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

#include "dragroupmacro.h"
#include "macro/draactionstartrecord.h"
#include "macro/draactionstoprecord.h"
#include "macro/draactionreplay.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraGroupMacro"
#include <logging/catlog.h>

struct _DraGroupMacroPrivate {
	LeaKeyContext *key_context;
	DraActionStartRecord *action_start_record;
	DraActionStopRecord *action_stop_record;
	DraActionReplay *action_replay;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraGroupMacro, dra_group_macro, LEA_TYPE_ACTION_GROUP,
		G_ADD_PRIVATE(DraGroupMacro)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_group_macro_class_init(DraGroupMacroClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_group_macro_init(DraGroupMacro *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraGroupMacro *instance = DRA_GROUP_MACRO(object);
	DraGroupMacroPrivate *priv = dra_group_macro_get_instance_private(instance);
	cat_unref_ptr(priv->key_context);
	cat_unref_ptr(priv->action_start_record);
	cat_unref_ptr(priv->action_stop_record);
	cat_unref_ptr(priv->action_replay);
	G_OBJECT_CLASS(dra_group_macro_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_group_macro_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraGroupMacro *dra_group_macro_new(LeaKeyContext *key_context, DraPanelOwner *panel_owner) {
	DraGroupMacro *result = g_object_new(DRA_TYPE_GROUP_MACRO, NULL);
	cat_ref_anounce(result);
	DraGroupMacroPrivate *priv = dra_group_macro_get_instance_private(result);
	priv->key_context = cat_ref_ptr(key_context);
	lea_action_group_construct((LeaActionGroup *) result, cat_string_wo_new_with("dra.group.macro"), cat_string_wo_new_with("M_acro"));

	DraMacroManager *macro_manager = dra_panel_owner_get_macro_manager(panel_owner);

	priv->action_start_record = dra_action_start_record_new(macro_manager);
	lea_action_set_key_context((LeaAction *) priv->action_start_record, key_context);
	lea_action_set_order((LeaAction *) priv->action_start_record, -3);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->action_start_record);

	priv->action_stop_record = dra_action_stop_record_new(macro_manager);
	lea_action_set_key_context((LeaAction *) priv->action_stop_record, key_context);
	lea_action_set_order((LeaAction *) priv->action_stop_record, -2);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->action_stop_record);

	priv->action_replay = dra_action_replay_new(macro_manager);
	lea_action_set_key_context((LeaAction *) priv->action_replay, key_context);
	lea_action_set_order((LeaAction *) priv->action_replay, -1);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->action_replay);

	return result;
}

void dra_group_macro_set_editor_panel(DraGroupMacro *group_macro, DraEditorPanel *editor_panel) {

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
