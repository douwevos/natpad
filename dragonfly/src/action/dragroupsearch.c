/*
   File:    dragroupsearch.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Sep 2, 2015
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

#include "draactionclipboard.h"
#include "dragroupsearch.h"
#include <logging/catlogdefs.h>

#include "search/draactionfindnext.h"
#include "search/draactionfindprevious.h"
#include "search/draactionmarkclearall.h"
#include "search/draactionmarkcopy.h"
#include "search/draactionmarkcut.h"
#include "search/draactionmarkinvertall.h"
#include "search/draactionmarktoggle.h"

#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraGroupSearch"
#include <logging/catlog.h>

struct _DraGroupSearchPrivate {
	LeaKeyContext *key_context;

	DraActionFindNext *action_find_next;
	DraActionFindPrevious *action_find_previous;

	DraActionMarkToggle *action_mark_toggle;
	DraActionMarkClearAll *action_mark_clear_all;
	DraActionMarkInvertAll *action_mark_invert_all;
	DraActionMarkCopy *action_mark_copy;
	DraActionMarkCut *action_mark_cut;

	gboolean clipboard_has_text;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraGroupSearch, dra_group_search, LEA_TYPE_ACTION_GROUP,
		G_ADD_PRIVATE(DraGroupSearch)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_group_search_class_init(DraGroupSearchClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_group_search_init(DraGroupSearch *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraGroupSearch *instance = DRA_GROUP_SEARCH(object);
	DraGroupSearchPrivate *priv = dra_group_search_get_instance_private(instance);
	cat_unref_ptr(priv->key_context);
	cat_unref_ptr(priv->action_find_next);
	cat_unref_ptr(priv->action_find_previous);

	cat_unref_ptr(priv->action_mark_clear_all);
	cat_unref_ptr(priv->action_mark_copy);
	cat_unref_ptr(priv->action_mark_cut);
	cat_unref_ptr(priv->action_mark_invert_all);
	cat_unref_ptr(priv->action_mark_toggle);
	G_OBJECT_CLASS(dra_group_search_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_group_search_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

DraGroupSearch *dra_group_search_new(LeaKeyContext *key_context, gpointer clipboard_handler) {
	DraGroupSearch *result = g_object_new(DRA_TYPE_GROUP_SEARCH, NULL);
	cat_ref_anounce(result);
	DraGroupSearchPrivate *priv = dra_group_search_get_instance_private(result);
	priv->key_context = cat_ref_ptr(key_context);
	lea_action_group_construct((LeaActionGroup *) result, cat_ref_ptr(lea_prov_search_group_name()), cat_string_wo_new_with("_Search"));


	LeaActionGroup *grp_find = lea_action_group_create_sub((LeaActionGroup *) result, cat_string_wo_new_with("search.find.group"), NULL);
	lea_action_set_order((LeaAction *) grp_find, -2);

	priv->action_find_next = dra_action_find_next_new();
	lea_action_group_add(grp_find, (LeaAction *) priv->action_find_next);

	priv->action_find_previous = dra_action_find_previous_new();
	lea_action_group_add(grp_find, (LeaAction *) priv->action_find_previous);

	cat_unref(grp_find);


	LeaActionGroup *grp_mark = lea_action_group_create_sub((LeaActionGroup *) result, cat_string_wo_new_with("search.mark.group"), NULL);
	lea_action_set_order((LeaAction *) grp_mark, -1);

	priv->action_mark_toggle = dra_action_mark_toggle_new();
	lea_action_group_add(grp_mark, (LeaAction *) priv->action_mark_toggle);

	priv->action_mark_clear_all = dra_action_mark_clear_all_new();
	lea_action_group_add(grp_mark, (LeaAction *) priv->action_mark_clear_all);

	priv->action_mark_invert_all = dra_action_mark_invert_all_new();
	lea_action_group_add(grp_mark, (LeaAction *) priv->action_mark_invert_all);

	priv->action_mark_copy = dra_action_mark_copy_new(clipboard_handler);
	lea_action_group_add(grp_mark, (LeaAction *) priv->action_mark_copy);

	priv->action_mark_cut = dra_action_mark_cut_new();
	lea_action_group_add(grp_mark, (LeaAction *) priv->action_mark_cut);

	cat_unref(grp_mark);

	return result;
}

void dra_group_search_set_editor_panel(DraGroupSearch *group_search, DraEditorPanel *editor_panel) {
	DraGroupSearchPrivate *priv = dra_group_search_get_instance_private(group_search);
	dra_action_find_next_set_editor_panel(priv->action_find_next, editor_panel);
	dra_action_find_previous_set_editor_panel(priv->action_find_previous, editor_panel);

	dra_action_mark_toggle_set_editor(priv->action_mark_toggle, editor_panel);
	dra_action_mark_clear_all_set_editor(priv->action_mark_clear_all, editor_panel);
	dra_action_mark_invert_all_set_editor(priv->action_mark_invert_all, editor_panel);
	dra_action_mark_copy_set_editor(priv->action_mark_copy, editor_panel);
	dra_action_mark_cut_set_editor(priv->action_mark_cut, editor_panel);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraGroupSearch *instance = DRA_GROUP_SEARCH(self);
	DraGroupSearchPrivate *priv = dra_group_search_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p clipboard_has_text=%d]", iname, self, priv->clipboard_has_text);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
