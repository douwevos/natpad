/*
   File:    elkasyncmoonodeupdater.c
   Project: elk
   Author:  Douwe Vos
   Date:    Oct 4, 2015
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

#include "elkasyncmoonodeupdater.h"
#include "elkpanelowner.h"
#include <moose.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkAsyncMooNodeUpdater"
#include <logging/catlog.h>

struct _ElkAsyncMooNodeUpdaterPrivate {
	ElkEditorPanel *editor_panel;
	ChaIOAsync *async_response;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ElkAsyncMooNodeUpdater, elk_async_moo_node_updater, CHA_TYPE_IO_ASYNC,
		G_ADD_PRIVATE(ElkAsyncMooNodeUpdater)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
void l_async_finished(ChaIOAsync *async, gboolean success, GError *error);

static void elk_async_moo_node_updater_class_init(ElkAsyncMooNodeUpdaterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaIOAsyncClass *ioas_class = CHA_IO_ASYNC_CLASS(clazz);
	ioas_class->finished = l_async_finished;
}

static void elk_async_moo_node_updater_init(ElkAsyncMooNodeUpdater *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkAsyncMooNodeUpdater *instance = ELK_ASYNC_MOO_NODE_UPDATER(object);
	ElkAsyncMooNodeUpdaterPrivate *priv = elk_async_moo_node_updater_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(elk_async_moo_node_updater_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_async_moo_node_updater_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ElkAsyncMooNodeUpdater *elk_async_moo_node_updater_new(ElkEditorPanel *editor_panel, ChaIOAsync *async_response) {
	ElkAsyncMooNodeUpdater *result = g_object_new(ELK_TYPE_ASYNC_MOO_NODE_UPDATER, NULL);
	cat_ref_anounce(result);
	ElkAsyncMooNodeUpdaterPrivate *priv = elk_async_moo_node_updater_get_instance_private(result);
	priv->editor_panel = cat_ref_ptr(editor_panel);
	priv->async_response = cat_ref_ptr(async_response);
	return result;
}

void l_async_finished(ChaIOAsync *async, gboolean success, GError *error) {
	ElkAsyncMooNodeUpdater *instance = ELK_ASYNC_MOO_NODE_UPDATER(async);
	ElkAsyncMooNodeUpdaterPrivate *priv = elk_async_moo_node_updater_get_instance_private(instance);

	ElkPanelOwner *panel_owner = (ElkPanelOwner *) lea_panel_get_panel_owner((LeaPanel *) priv->editor_panel);
	MooNodeWo *moo_node = elk_editor_panel_get_moose_node(priv->editor_panel);

	if (moo_node) {
		MooDirectContentWo *direct_content = (MooDirectContentWo *) moo_node_wo_get_content(moo_node, moo_direct_content_wo_key());
		if (direct_content) {
			MooIdNodeMatcher *matcher = moo_id_node_matcher_new(moo_node_wo_get_unique_id(moo_node));
			MooService *moo_service = elk_panel_owner_get_moose_service(panel_owner);
			MooTransaction *tx = moo_service_create_transaction((GObject *) async, moo_service);
			while(TRUE) {
				moo_transaction_begin(tx);
				MooNodeWo *root_node = moo_transaction_get_tx_root_node(tx);
				int idx = moo_node_wo_find_index(root_node, (MooINodeMatcher *) matcher, NULL, -1);
				if (idx<0) {
					break;
				}

				MooNodeWo *e_root_node = moo_node_wo_ensure_editable(root_node, NULL);
				MooNodeWo *e_node = moo_node_wo_get_editable_child_at(e_root_node, idx);
				const CatStringWo *name = dra_editor_panel_get_name((DraEditorPanel *) priv->editor_panel);
				moo_node_wo_set_name(e_node, (CatStringWo *) name);
				if (moo_transaction_commit(tx, e_root_node)) {
					MooNodeWo *a_root_node = moo_transaction_get_commited_root_node(tx);
					MooNodeWo *nn = moo_node_wo_child_at(a_root_node, idx);
					elk_editor_panel_set_moose_node(priv->editor_panel, nn);
					break;
				}
			}
			cat_unref_ptr(tx);
			cat_unref_ptr(matcher);
		}
	}
	if (priv->async_response) {
		cha_io_async_finished(priv->async_response, success, error);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ElkAsyncMooNodeUpdater *instance = ELK_ASYNC_MOO_NODE_UPDATER(self);
	ElkAsyncMooNodeUpdaterPrivate *priv = elk_async_moo_node_updater_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p editor_panel=%p]", iname, self, priv->editor_panel);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
