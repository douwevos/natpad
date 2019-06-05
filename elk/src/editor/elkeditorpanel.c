/*
   File:    elkeditorpanel.c
   Project: elk
   Author:  Douwe Vos
   Date:    Jul 5, 2015
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

#include "elkeditorpanel.h"
#include "elkpanelowner.h"
#include "elkasyncmoonodeupdater.h"
#include "../dialog/elkdialogs.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkEditorPanel"
#include <logging/catlog.h>

struct _ElkEditorPanelPrivate {
	MooNodeWo *moo_node;
	ElkDocumentBin *document_bin;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ElkEditorPanel, elk_editor_panel, DRA_TYPE_EDITOR_PANEL,
		G_ADD_PRIVATE(ElkEditorPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static const CatStringWo *l_get_name(const DraEditorPanel *editor_panel);
static GtkResponseType l_do_save_request(DraEditorPanel *editor_panel);
static void l_save(DraEditorPanel *editor_panel, gboolean do_save_as, ChaIOAsync *async_response);
static void l_revert(DraEditorPanel *editor);
static void l_reactivated(DraEditorPanel *editor_panel, gboolean focus_active_and_set);

static void l_panel_closing(LeaPanel *panel);


static void elk_editor_panel_class_init(ElkEditorPanelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	DraEditorPanelClass *dra_class = DRA_EDITOR_PANEL_CLASS(clazz);
	dra_class->getName = l_get_name;
	dra_class->doSaveRequest = l_do_save_request;
	dra_class->save = l_save;
	dra_class->revert = l_revert;
	dra_class->reactivated = l_reactivated;

	LeaPanelClass *lea_class = LEA_PANEL_CLASS(clazz);
	lea_class->closing = l_panel_closing;
}

static void elk_editor_panel_init(ElkEditorPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkEditorPanel *instance = ELK_EDITOR_PANEL(object);
	ElkEditorPanelPrivate *priv = elk_editor_panel_get_instance_private(instance);
	cat_unref_ptr(priv->moo_node);
	cat_unref_ptr(priv->document_bin);
	G_OBJECT_CLASS(elk_editor_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_editor_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void elk_editor_panel_construct(ElkEditorPanel *editor_panel, LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, DraIConnectorRequestFactory *request_factory) {
	ElkEditorPanelPrivate *priv = elk_editor_panel_get_instance_private(editor_panel);
	priv->moo_node = NULL;
	priv->document_bin = cat_ref_ptr(document_bin);
	ChaDocument *document = elk_document_bin_get_or_create_document(document_bin);
	cha_document_set_read_only(document, elk_document_bin_update_read_only(document_bin, FALSE));
	dra_editor_panel_construct((DraEditorPanel *) editor_panel, panel_owner, document, request_factory);
	CatStringWo *document_name = elk_document_bin_get_name(document_bin);
	lea_panel_set_label((LeaPanel *) editor_panel, document_name);

}

ElkEditorPanel *elk_editor_panel_new(LeaIPanelOwner *panel_owner, ElkDocumentBin *document_bin, DraIConnectorRequestFactory *request_factory) {
	ElkEditorPanel *result = g_object_new(ELK_TYPE_EDITOR_PANEL, NULL);
	cat_ref_anounce(result);
	elk_editor_panel_construct(result, panel_owner, document_bin, request_factory);
	return result;
}

void elk_editor_panel_set_moose_node(ElkEditorPanel *editor_panel, MooNodeWo *moo_node) {
	ElkEditorPanelPrivate *priv = elk_editor_panel_get_instance_private(editor_panel);
	cat_ref_swap(priv->moo_node, moo_node);
}

MooNodeWo *elk_editor_panel_get_moose_node(const ElkEditorPanel *editor_panel) {
	const ElkEditorPanelPrivate *priv = elk_editor_panel_get_instance_private((ElkEditorPanel *) editor_panel);
	return priv->moo_node;
}

ElkDocumentBin *elk_editor_panel_get_document_bin(const ElkEditorPanel *editor_panel) {
	const ElkEditorPanelPrivate *priv = elk_editor_panel_get_instance_private((ElkEditorPanel *) editor_panel);
	return priv->document_bin;
}


static const CatStringWo *l_get_name(const DraEditorPanel *editor_panel) {
	const ElkEditorPanelPrivate *priv = elk_editor_panel_get_instance_private((ElkEditorPanel *) editor_panel);
	return elk_document_bin_get_name(priv->document_bin);
}

static GtkResponseType l_do_save_request(DraEditorPanel *editor_panel) {
	ElkPanelOwner *panel_owner = (ElkPanelOwner *) lea_panel_get_panel_owner((LeaPanel *) editor_panel);
	ElkDialogs *dialogs = elk_panel_owner_get_dialogs(panel_owner);
	return elk_dialogs_save_before_close(dialogs);
}

static void l_save(DraEditorPanel *editor_panel, gboolean do_save_as, ChaIOAsync *async_response) {
	ElkEditorPanelPrivate *priv = elk_editor_panel_get_instance_private((ElkEditorPanel *) editor_panel);
	ElkPanelOwner *panel_owner = (ElkPanelOwner *) lea_panel_get_panel_owner((LeaPanel *) editor_panel);
	ElkAsyncMooNodeUpdater *moo_node_updater = elk_async_moo_node_updater_new((ElkEditorPanel *) editor_panel, async_response);
	if (!elk_document_bin_has_file(priv->document_bin) || do_save_as) {
		ElkDialogs *dialogs = elk_panel_owner_get_dialogs(panel_owner);
		ElkSaveDialog save_dialog;
		save_dialog.document = elk_document_bin_get_document(priv->document_bin);
		save_dialog.selected_charset = NULL;
		CatStringWo *save_name = elk_dialogs_save_file_selector(dialogs, &save_dialog);
		if (save_name!=NULL) {
			ChaIConverter *converter = NULL;
			if (save_dialog.selected_charset!=NULL) {
				ChaDocumentManager *doc_manager = cha_document_get_document_manager(save_dialog.document);
				ChaCharsetConverterFactory *converter_factory = cha_document_manager_get_converter_factory(doc_manager);
				converter = cha_charset_converter_factory_get(converter_factory, cat_string_wo_getchars(save_dialog.selected_charset));
			}
			elk_document_bin_store_as(priv->document_bin, save_name, converter, (ChaIOAsync *) moo_node_updater);
		}
	} else {
		elk_document_bin_store(priv->document_bin, (ChaIOAsync *) moo_node_updater);
	}
	cat_unref_ptr(moo_node_updater);
}

static void l_revert(DraEditorPanel *editor_panel) {
	ElkEditorPanelPrivate *priv = elk_editor_panel_get_instance_private((ElkEditorPanel *) editor_panel);
	elk_document_bin_revert(priv->document_bin);
}


static void  l_panel_closing(LeaPanel *panel) {
	LeaPanelClass *p_class = LEA_PANEL_CLASS(elk_editor_panel_parent_class);
	if (p_class->closing) {
		p_class->closing(panel);
	}
	ElkEditorPanel *elk_panel = (ElkEditorPanel *) panel;
	ElkEditorPanelPrivate *priv = elk_editor_panel_get_instance_private(elk_panel);
	if (elk_document_bin_decrease_usage(priv->document_bin)) {

		ElkPanelOwner *panel_owner = (ElkPanelOwner *) lea_panel_get_panel_owner(panel);
		MooNodeWo *moo_node = elk_editor_panel_get_moose_node(elk_panel);

		if (moo_node) {
			MooDirectContentWo *direct_content = (MooDirectContentWo *) moo_node_wo_get_content(moo_node, moo_direct_content_wo_key());
			if (direct_content) {
				MooIdNodeMatcher *matcher = moo_id_node_matcher_new(moo_node_wo_get_unique_id(moo_node));
				MooService *moo_service = elk_panel_owner_get_moose_service(panel_owner);
				MooTransaction *tx = moo_service_create_transaction((GObject *) panel, moo_service);
				while(TRUE) {
					moo_transaction_begin(tx);
					MooNodeWo *root_node = moo_transaction_get_tx_root_node(tx);
					int idx = moo_node_wo_find_index(root_node, (MooINodeMatcher *) matcher, NULL, -1);
					if (idx<0) {
						break;
					}

					MooNodeWo *e_root_node = moo_node_wo_ensure_editable(root_node, NULL);
					MooNodeListWo *children = moo_node_wo_get_editable_children(e_root_node);
					moo_node_list_wo_remove(children, idx);
					if (moo_transaction_commit(tx, e_root_node)) {
						elk_editor_panel_set_moose_node(elk_panel, NULL);
						break;
					}
				}
				cat_unref_ptr(tx);
				cat_unref_ptr(matcher);
			}
		}
	}
}


static void l_reactivated(DraEditorPanel *editor_panel, gboolean focus_active_and_set) {
	cat_log_debug("focus_active_and_set=%d", focus_active_and_set);
	if (focus_active_and_set) {
		ElkEditorPanelPrivate *priv = elk_editor_panel_get_instance_private((ElkEditorPanel *) editor_panel);
		ChaDocument *document = elk_document_bin_get_or_create_document(priv->document_bin);
		cha_document_set_read_only(document, elk_document_bin_update_read_only(priv->document_bin, TRUE));

		LeaSurface *surface = lea_panel_get_surface((LeaPanel *) editor_panel);
		LeaFrame *frame = lea_surface_get_frame(surface);
		GtkWidget *top_level = gtk_widget_get_toplevel((GtkWidget *) frame);

		GdkWindow *window = gtk_widget_get_window(top_level);
		CatStringWo *document_name = elk_document_bin_get_name(priv->document_bin);
		gdk_window_set_title(window, cat_string_wo_getchars(document_name));

	}
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
