/*
   File:    dragroupnavigate.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 12, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "dragroupnavigate.h"
#include "navigate/draactionpreviouseditor.h"
#include "navigate/draactionnexteditor.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraGroupNavigate"
#include <logging/catlog.h>

struct _DraGroupNavigatePrivate {
	LeaKeyContext *key_context;

	DraActionPreviousEditor *action_previous_editor;
	DraActionNextEditor *action_next_editor;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraGroupNavigate, dra_group_navigate, LEA_TYPE_ACTION_GROUP,
		G_ADD_PRIVATE(DraGroupNavigate)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_group_navigate_class_init(DraGroupNavigateClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_group_navigate_init(DraGroupNavigate *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraGroupNavigate *instance = DRA_GROUP_NAVIGATE(object);
	DraGroupNavigatePrivate *priv = dra_group_navigate_get_instance_private(instance);
	cat_unref_ptr(priv->key_context);
	cat_unref_ptr(priv->action_previous_editor);
	cat_unref_ptr(priv->action_next_editor);
	G_OBJECT_CLASS(dra_group_navigate_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_group_navigate_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraGroupNavigate *dra_group_navigate_new(LeaKeyContext *key_context, DraPanelOwner *panel_owner) {
	DraGroupNavigate *result = g_object_new(DRA_TYPE_GROUP_NAVIGATE, NULL);
	cat_ref_anounce(result);
	DraGroupNavigatePrivate *priv = dra_group_navigate_get_instance_private(result);
	priv->key_context = cat_ref_ptr(key_context);
	lea_action_group_construct((LeaActionGroup *) result, cat_ref_ptr(lea_prov_navigate_group_name()), cat_string_wo_new_with("_Navigate"));

	priv->action_next_editor = dra_action_next_editor_new(panel_owner);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->action_next_editor);

	priv->action_previous_editor = dra_action_previous_editor_new(panel_owner);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->action_previous_editor);

	return result;
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
