/*
   File:    draactionspellsuggest.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 29, 2016
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

#include "draactionspellsuggest.h"
#include "../uow/drauowspellsuggest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraActionSpellSuggest"
#include <logging/catlog.h>

struct _DraActionSpellSuggestPrivate {
	DraEditorPanel *editor_panel;
	CatStringWo *suggest_word;
	DraLineTagWo *spell_tag;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraActionSpellSuggest, dra_action_spell_suggest, LEA_TYPE_ACTION,
		G_ADD_PRIVATE(DraActionSpellSuggest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void dra_action_spell_suggest_class_init(DraActionSpellSuggestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void dra_action_spell_suggest_init(DraActionSpellSuggest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraActionSpellSuggest *instance = DRA_ACTION_SPELL_SUGGEST(object);
	DraActionSpellSuggestPrivate *priv = dra_action_spell_suggest_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	cat_unref_ptr(priv->spell_tag);
	cat_unref_ptr(priv->suggest_word);
	G_OBJECT_CLASS(dra_action_spell_suggest_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_action_spell_suggest_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraActionSpellSuggest *dra_action_spell_suggest_new(DraEditorPanel *editor_panel, CatStringWo *suggest_word, DraLineTagWo *spell_tag) {
	DraActionSpellSuggest *result = g_object_new(DRA_TYPE_ACTION_SPELL_SUGGEST, NULL);
	cat_ref_anounce(result);
	DraActionSpellSuggestPrivate *priv = dra_action_spell_suggest_get_instance_private(result);
	CatStringWo *action_text = cat_string_wo_new();
	cat_string_wo_format(action_text, "change to '%O'", suggest_word);
	priv->editor_panel = cat_ref_ptr(editor_panel);
	priv->suggest_word = cat_ref_ptr(suggest_word);
	priv->spell_tag = cat_ref_ptr(spell_tag);

	action_text = cat_string_wo_anchor(action_text, 1);
	lea_action_construct(LEA_ACTION(result), action_text, cat_ref_ptr(action_text), NULL);
	return result;
}

static void l_action_run(LeaAction *self) {
	DraActionSpellSuggest *instance = DRA_ACTION_SPELL_SUGGEST(self);
	DraActionSpellSuggestPrivate *priv = dra_action_spell_suggest_get_instance_private(instance);
	DraUowSpellSuggest *uow = dra_uow_spell_suggest_new(priv->suggest_word, priv->spell_tag);
	DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
	cha_editor_run((ChaEditor *) editor, (ChaUow *) uow);
	cat_unref_ptr(uow);
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
