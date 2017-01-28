/*
   File:    mooactionrefresh.c
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Mar 14, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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


#include "mooactionrefresh.h"
#include "../moonodelayout.h"
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooActionRefresh"
#include <logging/catlog.h>

struct _MooActionRefreshPrivate {
	CatArrayWo *selection;
	MooService *moo_service;
};


G_DEFINE_TYPE(MooActionRefresh, moo_action_refresh, LEA_TYPE_ACTION)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void moo_action_refresh_class_init(MooActionRefreshClass *clazz) {
	g_type_class_add_private(clazz, sizeof(MooActionRefreshPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_clazz = LEA_ACTION_CLASS(clazz);
	action_clazz->action_run = l_action_run;
}

static void moo_action_refresh_init(MooActionRefresh *instance) {
	MooActionRefreshPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_ACTION_REFRESH, MooActionRefreshPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooActionRefresh *instance = MOO_ACTION_REFRESH(object);
	MooActionRefreshPrivate *priv = instance->priv;
	cat_unref_ptr(priv->moo_service);
	cat_unref_ptr(priv->selection);
	G_OBJECT_CLASS(moo_action_refresh_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_action_refresh_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooActionRefresh *moo_action_refresh_new(MooService *moo_service) {
	MooActionRefresh *result = g_object_new(MOO_TYPE_ACTION_REFRESH, NULL);
	cat_ref_anounce(result);
	MooActionRefreshPrivate *priv = result->priv;
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->selection = NULL;
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("moose.refresh"), cat_string_wo_new_with("Re_fresh"), cat_string_wo_new_with("view-refresh"));
	lea_action_set_sensitive_self((LeaAction *) result, FALSE);
	return result;
}


void moo_action_refresh_set_selection(MooActionRefresh *action_refresh, CatArrayWo *selection) {
	MooActionRefreshPrivate *priv = MOO_ACTION_REFRESH_GET_PRIVATE(action_refresh);
	cat_ref_swap(priv->selection, selection);
	gboolean has_refreshable_selection = FALSE;
	if (selection) {
		CatIIterator *iter = cat_array_wo_iterator(selection);
		while(cat_iiterator_has_next(iter)) {
			MooNodeLayout *node_layout = (MooNodeLayout *) cat_iiterator_next(iter);
			MooNodeWo *node = moo_node_layout_get_node(node_layout);
			MooIServicesContent *serivces_content = (MooIServicesContent *) moo_node_wo_get_content(node, moo_iservices_content_key());
			if ((serivces_content!=NULL) && moo_iservices_content_can_refresh(serivces_content)) {
				has_refreshable_selection = TRUE;
			} else {
				has_refreshable_selection = FALSE;
				break;
			}
		}
		cat_unref_ptr(iter);
	}
	lea_action_set_sensitive_self((LeaAction *) action_refresh, has_refreshable_selection);
}




static void l_action_run(LeaAction *self) {
	MooActionRefreshPrivate *priv = MOO_ACTION_REFRESH_GET_PRIVATE(self);
	CatArrayWo *selection = priv->selection;
	if (selection) {
		CatIIterator *iter = cat_array_wo_iterator(selection);
		while(cat_iiterator_has_next(iter)) {
			MooNodeLayout *node_layout = (MooNodeLayout *) cat_iiterator_next(iter);
			MooNodeWo *node = moo_node_layout_get_node(node_layout);
			MooIServicesContent *serivces_content = (MooIServicesContent *) moo_node_wo_get_content(node, moo_iservices_content_key());
			if ((serivces_content!=NULL) && moo_iservices_content_can_refresh(serivces_content)) {
				moo_iservices_content_refresh(serivces_content, priv->moo_service, node);
			}
		}
		cat_unref_ptr(iter);
	}

//	if (priv->destination == NULL) {
//		return;
//	}
//	MooNodeWo *node = moo_node_layout_get_node(priv->destination);
//	MooResourceContentWo *resource_content = (MooResourceContentWo *) moo_node_wo_get_content(node, moo_resource_content_wo_key());
//	if (resource_content) {
//		CatReadableTreeNode *tree_vip_node = moo_resource_content_wo_get_node(resource_content);
//		moo_clipboard_paste_resources(priv->moo_clipboard, tree_vip_node);
//	}
}




