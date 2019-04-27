/*
   File:    mooactiondelete.c
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Apr 18, 2014
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

#include "mooactiondelete.h"
#include "../moonodelayout.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooActionDelete"
#include <logging/catlog.h>

struct _MooActionDeletePrivate {
	MooService *moo_service;
	LeaFrame *frame;
	CatArrayWo *selection;
};

G_DEFINE_TYPE_WITH_PRIVATE(MooActionDelete, moo_action_delete, LEA_TYPE_ACTION)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void moo_action_delete_class_init(MooActionDeleteClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void moo_action_delete_init(MooActionDelete *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooActionDelete *instance = MOO_ACTION_DELETE(object);
	MooActionDeletePrivate *priv = moo_action_delete_get_instance_private(instance);
	cat_unref_ptr(priv->moo_service);
	cat_unref_ptr(priv->frame);
	cat_unref_ptr(priv->selection);
	G_OBJECT_CLASS(moo_action_delete_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_action_delete_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooActionDelete *moo_action_delete_new(MooService *moo_service, LeaFrame *frame) {
	MooActionDelete *result = g_object_new(MOO_TYPE_ACTION_DELETE, NULL);
	cat_ref_anounce(result);
	MooActionDeletePrivate *priv = moo_action_delete_get_instance_private(result);
	priv->moo_service = cat_ref_ptr(moo_service);
	priv->frame = cat_ref_ptr(frame);
	priv->selection = NULL;
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("moose.delete"), cat_string_wo_new_with("_Delete"), NULL);
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_data("Delete")));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	return result;
}

void moo_action_delete_set_selection(MooActionDelete *action_delete, CatArrayWo *selection) {
	MooActionDeletePrivate *priv = moo_action_delete_get_instance_private(action_delete);
	if (priv->selection == selection) {
		return;
	}
	cat_ref_swap(priv->selection, selection);
	gboolean has_deleteable_node = FALSE;

	if (priv->selection) {
		has_deleteable_node = cat_array_wo_size(priv->selection)>0;
		CatIIterator *iter = cat_array_wo_iterator(priv->selection);
		while(cat_iiterator_has_next(iter)) {
			MooNodeLayout *node_layout = (MooNodeLayout *) cat_iiterator_next(iter);
			MooNodeWo *node = moo_node_layout_get_node(node_layout);
			MooIServicesContent *serivces_content = (MooIServicesContent *) moo_node_wo_get_content(node, moo_iservices_content_key());
			if ((serivces_content==NULL) || !moo_iservices_content_can_delete(serivces_content)) {
				has_deleteable_node = FALSE;
				break;
			}
		}
		cat_unref_ptr(iter);
	}

	lea_action_set_sensitive_self(LEA_ACTION(action_delete), has_deleteable_node);
}


static void l_action_run(LeaAction *self) {
	MooActionDelete *instance = MOO_ACTION_DELETE(self);
	MooActionDeletePrivate *priv = moo_action_delete_get_instance_private(instance);
	if (priv->selection) {
		VipService *vip_service = moo_service_get_viper_service(priv->moo_service);
		CatIIterator *iter = cat_array_wo_iterator(priv->selection);
		while(cat_iiterator_has_next(iter)) {
			MooNodeLayout *node_layout = (MooNodeLayout *) cat_iiterator_next(iter);
			MooNodeWo *node = moo_node_layout_get_node(node_layout);
			MooIServicesContent *serivces_content = (MooIServicesContent *) moo_node_wo_get_content(node, moo_iservices_content_key());
			if ((serivces_content!=NULL) && moo_iservices_content_can_delete(serivces_content)) {
				if (moo_iservices_content_delete(serivces_content, priv->moo_service, node, priv->frame)) {
					vip_service_submit(vip_service, FALSE);
//					MooTransaction *tx = moo_service_create_transaction("action-delete", priv->moo_service);
//					while(TRUE) {
//						moo_transaction_begin(tx);
//						moo_tran
//					}
				}
			}
		}
		cat_unref_ptr(iter);
		if (cat_array_wo_size(priv->selection)>0) {
			vip_service_submit(vip_service, TRUE);
		}
	} else {
		cat_log_warn("no deleteable selection");
		lea_action_set_sensitive_self(LEA_ACTION(self), FALSE);
	}
}
