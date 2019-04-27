/*
   File:    moogroupmain.c
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Dec 23, 2013
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

#include "moogroupmain.h"
#include "moogroupmodule.h"
#include "mooactioncopy.h"
#include "mooactionpaste.h"
#include "mooactionrename.h"
#include "mooactionrefresh.h"
#include "mooactionproperties.h"
#include "mooactiondelete.h"

#include "../moonodelayout.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooGroupMain"
#include <logging/catlog.h>

struct _MooGroupMainPrivate {
	MooClipboard *moo_clipboard;
	MooGroupModule *group_module;
	CatArrayWo *selection;

	MooActionRename *action_rename;
	MooActionRefresh *action_refresh;
	MooActionProperties *action_properties;

	MooActionCopy *action_copy;
	MooActionPaste *action_paste;

	MooActionDelete *action_delete;
};


G_DEFINE_TYPE_WITH_PRIVATE(MooGroupMain, moo_group_main, LEA_TYPE_ACTION_GROUP)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_group_main_class_init(MooGroupMainClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_group_main_init(MooGroupMain *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooGroupMain *instance = MOO_GROUP_MAIN(object);
	MooGroupMainPrivate *priv = moo_group_main_get_instance_private(instance);
	cat_unref_ptr(priv->moo_clipboard);
	cat_unref_ptr(priv->group_module);
	cat_unref_ptr(priv->selection);
	cat_unref_ptr(priv->action_rename);
	cat_unref_ptr(priv->action_refresh);
	cat_unref_ptr(priv->action_properties);
	cat_unref_ptr(priv->action_copy);
	cat_unref_ptr(priv->action_paste);
	cat_unref_ptr(priv->action_delete);
	G_OBJECT_CLASS(moo_group_main_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_group_main_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

MooGroupMain *moo_group_main_new(MooService *moo_service, LeaFrame *frame, MooClipboard *clipboard, LeaKeyContext *key_context) {
	MooGroupMain *result = g_object_new(MOO_TYPE_GROUP_MAIN, NULL);
	cat_ref_anounce(result);
	MooGroupMainPrivate *priv = moo_group_main_get_instance_private(result);
	lea_action_group_construct((LeaActionGroup *) result, cat_string_wo_new_with("moo.group.main"), NULL);

	priv->moo_clipboard = cat_ref_ptr(clipboard);
	priv->selection = NULL;

	priv->group_module = moo_group_module_new(moo_service, frame);
	lea_action_group_add((LeaActionGroup *) result, (LeaAction *) priv->group_module);


	LeaActionGroup *group_file = lea_action_group_create_sub((LeaActionGroup *) result, cat_ref_ptr(lea_prov_file_group_name()), NULL);

	LeaActionGroup *grp_services = lea_action_group_create_sub((LeaActionGroup *) group_file, cat_string_wo_new_with("moo.services.group"), NULL);
	lea_action_set_order((LeaAction *) grp_services, -50);
	cat_unref(group_file);

	priv->action_rename = moo_action_rename_new(moo_service, frame);
	lea_action_set_key_context((LeaAction *) priv->action_rename, key_context);
	lea_action_set_order((LeaAction *) priv->action_rename, -3);
	lea_action_group_add(grp_services, (LeaAction *) priv->action_rename);


	priv->action_refresh = moo_action_refresh_new(moo_service);
	lea_action_set_key_context((LeaAction *) priv->action_refresh, key_context);
	lea_action_set_order((LeaAction *) priv->action_refresh, -2);
	lea_action_group_add(grp_services, (LeaAction *) priv->action_refresh);


	priv->action_properties = moo_action_properties_new(moo_service);
	lea_action_set_key_context((LeaAction *) priv->action_properties, key_context);
	lea_action_set_order((LeaAction *) priv->action_properties, -1);
	lea_action_group_add(grp_services, (LeaAction *) priv->action_properties);

	cat_unref_ptr(grp_services);

	LeaActionGroup *group_edit = lea_action_group_create_sub((LeaActionGroup *) result, cat_ref_ptr(lea_prov_edit_group_name()), NULL);
	LeaActionGroup *grp_clipboard = lea_action_group_create_sub(group_edit, cat_string_wo_new_with("edit.clipboard.group"), NULL);
	cat_unref(group_edit);

	lea_action_set_order((LeaAction *) grp_clipboard, -3);

//	priv->action_cut = cat_ref_sink_ptr(ast_action_cut_new((AstClipboardNotifyCB) ast_group_edit_refresh_clipboard_has_text, result));
//	lea_action_set_order((LeaAction *) priv->action_cut, -3);
//	lea_action_group_add(grp_clipboard, (LeaAction *) priv->action_cut);
//
	priv->action_copy = moo_action_copy_new(priv->moo_clipboard);
	lea_action_set_key_context((LeaAction *) priv->action_copy, key_context);
	lea_action_set_order((LeaAction *) priv->action_copy, -2);
	lea_action_group_add(grp_clipboard, (LeaAction *) priv->action_copy);

	priv->action_paste = moo_action_paste_new(priv->moo_clipboard);
	lea_action_set_key_context((LeaAction *) priv->action_paste, key_context);
	lea_action_set_order((LeaAction *) priv->action_paste, -1);
	lea_action_group_add(grp_clipboard, (LeaAction *) priv->action_paste);

	cat_unref(grp_clipboard);


	LeaActionGroup *grp_delete = lea_action_group_create_sub((LeaActionGroup *) group_edit, cat_string_wo_new_with("edit.delete.group"), NULL);
	lea_action_set_order((LeaAction *) grp_delete, -2);

	priv->action_delete = moo_action_delete_new(moo_service, frame);
	lea_action_set_key_context((LeaAction *) priv->action_delete, key_context);
	lea_action_set_order((LeaAction *) priv->action_delete, -2);
	lea_action_group_add(grp_delete, (LeaAction *) priv->action_delete);
	cat_unref(grp_delete);

	return result;
}

gboolean moo_group_main_set_has_focus(MooGroupMain *group_main, gboolean owner_has_focus) {
	MooGroupMainPrivate *priv = moo_group_main_get_instance_private(group_main);
	gboolean changed = lea_action_set_visible_self((LeaAction *) priv->action_copy, owner_has_focus);
	gboolean changed2 = lea_action_set_visible_self((LeaAction *) priv->action_paste, owner_has_focus);
	gboolean changed3 = lea_action_set_visible_self((LeaAction *) priv->action_delete, owner_has_focus);
	return changed | changed2 | changed3;
}

enum _MooSelectionFlag {
	MOO_SELECTION_FLAG_NONE,
	MOO_SELECTION_FLAG_ONE,
	MOO_SELECTION_FLAG_MULTIPLE,
	MOO_SELECTION_FLAG_MIXED
};

typedef enum _MooSelectionFlag MooSelectionFlag;

/**
 * selection    CatArrayWo<MooNodeLayout *>
 */
void moo_group_main_set_selection(MooGroupMain *group_main, VipSnapshot *vip_snapshot, CatArrayWo *selection) {
	MooGroupMainPrivate *priv = moo_group_main_get_instance_private(group_main);
	if (priv->selection==selection) {
		return;
	}
	cat_ref_swap(priv->selection, selection);
	gboolean has_resource = FALSE;
	gboolean resources_have_same_base = TRUE;
	CatReadableTreeNode *base_node = NULL;

	if (selection) {
		CatIIterator *iter = cat_array_wo_iterator(selection);
		while(cat_iiterator_has_next(iter)) {
			MooNodeLayout *node_layout = (MooNodeLayout *) cat_iiterator_next(iter);
			MooNodeWo *node = moo_node_layout_get_node(node_layout);
			MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content(node, moo_resource_content_wo_key());
			if (resource_content) {
				has_resource = TRUE;

				CatReadableTreeNode *tree_vip_node = moo_resource_content_wo_get_node(resource_content);
				if (tree_vip_node) {
					CatReadableTreeNode *tree_parent_node = (CatReadableTreeNode *) cat_tree_node_get_parent_node((CatTreeNode *) tree_vip_node);

					cat_log_debug("vip_node=%o, par_node=%o", tree_vip_node, tree_parent_node);
					if (tree_parent_node) {
						if (base_node==NULL) {
							base_node = cat_ref_ptr(tree_parent_node);
						} else if (cat_tree_node_get_location((CatTreeNode *) base_node)!=cat_tree_node_get_location((CatTreeNode *) tree_parent_node)) {
							resources_have_same_base = FALSE;
						}
					} else {
						resources_have_same_base = FALSE;
					}
					cat_unref_ptr(tree_parent_node);
				} else {
					resources_have_same_base = FALSE;
				}
			}
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(base_node)
	}
//
//	cat_log_debug("has_resource=%d, resources_have_same_base=%d", has_resource, resources_have_same_base);
//
	moo_action_copy_set_has_copyable_selection(priv->action_copy, has_resource & resources_have_same_base, selection);

	moo_action_paste_set_selection(priv->action_paste, has_resource, selection);

	moo_action_rename_set_selection(priv->action_rename, selection);
	moo_action_refresh_set_selection(priv->action_refresh, selection);
	moo_action_properties_set_selection(priv->action_properties, selection);
	moo_action_delete_set_selection(priv->action_delete, selection);
}
