/*
   File:    draactionnexteditor.c
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

#include "draactionnexteditor.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraActionNextEditor"
#include <logging/catlog.h>

struct _DraActionNextEditorPrivate {
	DraPanelOwner *panel_owner;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraActionNextEditor, dra_action_next_editor, LEA_TYPE_ACTION,
		G_ADD_PRIVATE(DraActionNextEditor)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void dra_action_next_editor_class_init(DraActionNextEditorClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void dra_action_next_editor_init(DraActionNextEditor *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraActionNextEditor *instance = DRA_ACTION_NEXT_EDITOR(object);
	DraActionNextEditorPrivate *priv = dra_action_next_editor_get_instance_private(instance);
	cat_unref_ptr(priv->panel_owner);
	G_OBJECT_CLASS(dra_action_next_editor_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_action_next_editor_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraActionNextEditor *dra_action_next_editor_new(DraPanelOwner *panel_owner) {
	DraActionNextEditor *result = g_object_new(DRA_TYPE_ACTION_NEXT_EDITOR, NULL);
	cat_ref_anounce(result);
	DraActionNextEditorPrivate *priv = dra_action_next_editor_get_instance_private(result);
	priv->panel_owner = cat_ref_ptr(panel_owner);
	lea_action_construct(LEA_ACTION(result), cat_string_wo_new_with("dragonfly.next.editor"), cat_string_wo_new_with("Next Editor"), NULL);
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_with("Ctrl+F6")));
	return result;
}


static void l_action_run(LeaAction *self) {
	DraActionNextEditorPrivate *priv = dra_action_next_editor_get_instance_private(DRA_ACTION_NEXT_EDITOR(self));
	dra_panel_owner_browse_forward(priv->panel_owner);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraActionNextEditor *instance = DRA_ACTION_NEXT_EDITOR(self);
	DraActionNextEditorPrivate *priv = dra_action_next_editor_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p panel_owner=%p]", iname, self, priv->panel_owner);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
