/*
   File:    elkservice.c
   Project: elk
   Author:  Douwe Vos
   Date:    Apr 11, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#include "elkservice.h"
#include "elkiservice.h"
#include "editor/elkiresourceeditorfactory.h"
#include "editor/elkeditorpanel.h"
#include "documentio/elkasyncbulkresult.h"
#include "dialog/elkmultichoosedialog.h"
#include <caterpillar.h>
#include <leafhopper.h>
#include <math.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkService"
#include <logging/catlog.h>

static void l_service_iface_init(ElkIServiceInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ElkService, elk_service, G_TYPE_OBJECT, { // @suppress("Unused static function")
			G_IMPLEMENT_INTERFACE(ELK_TYPE_ISERVICE, l_service_iface_init);
})

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void elk_service_class_init(ElkServiceClass *clazz) {

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void elk_service_init(ElkService *instance) {
}


static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkService *instance = ELK_SERVICE(object);

	if (instance->moo_explorer_service) {
		g_object_run_dispose((GObject *) (instance->moo_explorer_service));
		cat_unref_ptr(instance->moo_explorer_service);
	}

	if (instance->panel_owner) {
		g_object_run_dispose((GObject *) (instance->panel_owner));
	}
	cat_unref_ptr(instance->panel_owner);

	cat_unref_ptr(instance->frame);
	cat_unref_ptr(instance->vip_service);
	cat_unref_ptr(instance->document_io);

	cat_unref_ptr(instance->e_resource_handlers);
	cat_unref_ptr(instance->preferences_service);
	cat_unref_ptr(instance->dialogs);

	G_OBJECT_CLASS(elk_service_parent_class)->dispose(object);

	cat_log_detail("end-dispose:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);

	G_OBJECT_CLASS(elk_service_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}



ElkService *elk_service_new(LeaFrame *frame, MooService *moo_service, ElkPreferencesService *prefences_service, GApplication *application) {
	ElkService *result = g_object_new(ELK_TYPE_SERVICE, NULL);
	cat_ref_anounce(result);

//	VipService *vip_service = moo_service_get_viper_service(moo_service);
//	VipCdProvider *cd_provider = vip_service_get_cd_provider(vip_service);

	result->preferences_service = cat_ref_ptr(prefences_service);
	result->frame = cat_ref_ptr(frame);
	result->application = application;


	result->vip_service = cat_ref_ptr(moo_service_get_viper_service(moo_service));
	result->moo_service = cat_ref_ptr(moo_service);
	result->moo_explorer_service = moo_explorer_service_new(result->moo_service, frame);

	result->panel_owner = elk_panel_owner_new((ElkIService *) result, result->moo_service, result->vip_service, result->frame);
//	result->dragonfly_panel_owner = dra_panel_owner_new(result->frame);

	ChaDocumentManager *document_manager = cha_document_manager_new();

	result->document_io = elk_document_io_new(result->vip_service, document_manager);
	cat_unref_ptr(document_manager);

	result->e_resource_handlers = cat_array_wo_new();


	ElkPreferencesContainer *prefs_container = elk_preferences_service_get_container(result->preferences_service);
	cow_container_add_change_listener((CowContainer *) prefs_container, COW_ICHANGE_LISTENER(result->panel_owner));

	return result;
}


ElkPreferencesService *elk_service_get_preferences_service(ElkService *service) {
	return service->preferences_service;
}



void elk_service_add_resource_handler(ElkService *service, ElkIResourceHandler *resource_handler) {
	cat_array_wo_append(service->e_resource_handlers, G_OBJECT(resource_handler));
}

void elk_service_remove_resource_handler(ElkService *service, ElkIResourceHandler *resource_handler) {
	int index = cat_array_wo_find_index(service->e_resource_handlers, G_OBJECT(resource_handler), -1);
	if (index>=0) {
		cat_array_wo_remove(service->e_resource_handlers, index, NULL);
	}
}

CatArrayWo *elk_service_enlist_editor_factories(ElkService *service, MooNodeWo *moo_node) {
	CatArrayWo *e_result = cat_array_wo_new();
	CatIIterator *iter = cat_array_wo_iterator(service->e_resource_handlers);
	while(cat_iiterator_has_next(iter)) {
		ElkIResourceHandler *res_handler = cat_iiterator_next(iter);
		ElkIResourceHandlerInterface *iface = ELK_IRESOURCE_HANDLER_GET_INTERFACE(res_handler);
		if (iface->enlistEditorFactories) {
			iface->enlistEditorFactories(res_handler, e_result, moo_node);
		}
	}
	cat_unref_ptr(iter);
	return e_result;
}



static gboolean l_service_exit(ElkIService *service) {
	cat_log_debug("exit");
	if (!elk_iservice_close_multiple_resource_editors(service, NULL)) {
		cat_log_debug("no exit");
		// TODO add dialog for forcing exit (and decide if that is necessary)
		return FALSE;
	}
	cat_log_debug("really exit");

	/* allow g_idle_adds as a result of closing the editors to run */
	int idx;
	for(idx=0; idx<100; idx++) {
		if (g_main_context_iteration (NULL, FALSE)) {
			cat_log_debug("iter-exited=%d", idx);
		} else {
			break;
		}
	}
	cat_log_debug("finished");
	ElkService *eservice = ELK_SERVICE(service);
	if (eservice->application) {
		g_application_quit(eservice->application);
	} else {
		gtk_main_quit();
	}
	return TRUE;
}

static void l_service_show_preferences(ElkIService *service) {
	ElkService *elk_service = ELK_SERVICE(service);

	CowPanelModel *panel_model = elk_preferences_service_get_panel_model(elk_service->preferences_service);
	ElkPreferencesContainer *container = elk_preferences_service_get_container(elk_service->preferences_service);
	ElkPreferencesWo *last_preferences_wo = elk_preferences_container_get(container);
	cat_log_debug("container=%p, last_preferences_wo=%o", container, last_preferences_wo);
	cat_ref_ptr(last_preferences_wo);

	CowDialog *cow_dialog = cow_dialog_new(panel_model, (CowContainer *) container, COW_DIALOG_OK_CANCEL_AND_APPLY);
    gtk_window_set_transient_for(GTK_WINDOW(cow_dialog), (GtkWindow *) gtk_widget_get_toplevel((GtkWidget *) elk_service->frame));

	int response = cow_dialog_show(cow_dialog);
	gboolean keep_open = TRUE;
	while(keep_open) {
		cat_log_debug("response=%d", response);
		switch(response) {
			case GTK_RESPONSE_CLOSE :
			case GTK_RESPONSE_DELETE_EVENT :
			case GTK_RESPONSE_CANCEL : {
				keep_open = FALSE;
				cow_container_set_configuration((CowContainer *) container, last_preferences_wo);
			} break;
			case GTK_RESPONSE_OK : {
				keep_open = FALSE;
				ElkPreferencesContainer *input_container = (ElkPreferencesContainer *) cow_dialog_get_edit_container(cow_dialog);
				ElkPreferencesWo *new_prefs = elk_preferences_container_get_fixed(input_container);
				elk_preferences_container_set(container, new_prefs);
				elk_preferences_service_save(elk_service->preferences_service);
			} break;
			case GTK_RESPONSE_APPLY : {
				ElkPreferencesContainer *input_container = (ElkPreferencesContainer *) cow_dialog_get_edit_container(cow_dialog);
				ElkPreferencesWo *new_prefs = elk_preferences_container_get_fixed(input_container);
				elk_preferences_container_set(container, new_prefs);
			} break;
		}
		if (keep_open) {
			ElkPreferencesContainer *input_container = (ElkPreferencesContainer *) cow_dialog_get_edit_container(cow_dialog);
			ElkPreferencesWo *e = elk_preferences_container_get(input_container);
			if (elk_preferences_wo_is_anchored(e)) {
				elk_preferences_container_set(input_container, e);
				cow_container_dispatch_change_event((CowContainer *) input_container);
			}
			response = gtk_dialog_run((GtkDialog *) cow_dialog);
		}
	}
	gtk_widget_destroy((GtkWidget *) cow_dialog);
	cat_unref_ptr(last_preferences_wo);
}

static void l_service_select_and_open_resources(ElkIService *service) {
	ElkService *elk_service = ELK_SERVICE(service);
	GtkWidget *dialog = gtk_file_chooser_dialog_new((const gchar *) "Select and open file", NULL,
					(GtkFileChooserAction) GTK_FILE_CHOOSER_ACTION_OPEN,
					"_Cancel", GTK_RESPONSE_CANCEL,
					"_OK", GTK_RESPONSE_OK,
					NULL);

	GtkFileChooser *file_chooser = GTK_FILE_CHOOSER(dialog);
	gtk_file_chooser_set_select_multiple(file_chooser, TRUE);

	gtk_widget_show_all(dialog);

	CatArrayWo *e_tree_node_array = cat_array_wo_new();

	gint result = gtk_dialog_run(GTK_DIALOG(dialog));
	cat_log_debug("response=%d", result);
	if (result == GTK_RESPONSE_OK) {
		GSList *files = gtk_file_chooser_get_filenames(file_chooser);
		if (files == NULL || g_slist_length(files)==0) {
			cat_log_info("no files selected");
		} else {
			GSList *iter = files;
			while(iter) {
				CatStringWo *name = cat_string_wo_new_with(iter->data);
				cat_log_debug("selected resource: %s", (char *) iter->data);
//
				VipPath *vip_path = vip_path_new(name);
				VipCreatePathRequest *request = vip_service_create_path(elk_service->vip_service, vip_path);
				VipNodePath *node_path = vip_create_path_request_wait_for_path(request);
				CatReadableTreeNode *tree_node = (CatReadableTreeNode *) vip_node_path_get_tail_node(node_path);
				cat_array_wo_append(e_tree_node_array, (GObject*) tree_node);
				cat_unref_ptr(request);
				cat_unref_ptr(vip_path);

				g_free(iter->data);
				iter = iter->next;
			}
			g_slist_free(files);
		}
	}
	gtk_widget_destroy(dialog);
	if (cat_array_wo_size(e_tree_node_array)>0) {
		ELK_ISERVICE_GET_INTERFACE(service)->openResourceList(service, e_tree_node_array);
	}
	cat_unref_ptr(e_tree_node_array);
}

//
//gboolean l_find_direct_resource(MonNode *node, gpointer user_data) {
//	gboolean result = FALSE;
//	if (MON_IS_RESOURCE_NODE(node)) {
//		if ((MonResourceNode *) node == (MonResourceNode *) user_data) {
//			result = TRUE;
//		}
//	}
//	return result;
//}




static ElkPreferencesWo *l_elk_get_preferences(ElkService *service) {
	ElkPreferencesContainer *prefs_container = elk_preferences_service_get_container(service->preferences_service);
	return elk_preferences_container_get(prefs_container);
}


static void l_service_open_new_file_editor(ElkIService *service, MooNodeWo *moo_node);


CatS moo_s_settings_filename = CAT_S_DEF("natpad-project.sht");

static void l_service_open_resource_list(ElkIService *service, CatArrayWo *e_resource_list) {
	ElkService *elk_service = ELK_SERVICE(service);
	MooService *moo_service = elk_service->moo_service;
////	VipService *vip_service = elk_service->vip_service;
////	ProWorkspaceService *workspace_service = elk_service->editor_panel_owner->workspace_service;
////	ProWorkspace *workspace = workspace_service->workspace;
////	CatArrayWo *e_change_list = cat_array_wo_new();
	gboolean opened_first = FALSE;
//
//	MonModel *mon_model = mon_service_get_model(mon_service);
//	MonChangeBag *change_bag = NULL;
//

//	CatStringWo *settings_fname = CAT_S(moo_s_settings_filename);
	CatIIterator *iterator = cat_array_wo_iterator(e_resource_list);
	while(cat_iiterator_has_next(iterator)) {
		CatReadableTreeNode *tree_node = (CatReadableTreeNode *) cat_iiterator_next(iterator);
		VipNode *vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) tree_node);

		CatStringWo *node_name = vip_node_get_name(vip_node);

//		if (cat_string_wo_endswith(fname, mon_settings_fname)) {
//			mon_service_load_module(mon_service, resource);
//		} else {

		MooTransaction *tx = moo_service_create_transaction((GObject *) service, moo_service);
		moo_transaction_begin(tx);
		MooNodeWo *node = moo_transaction_get_tx_root_node(tx);
		int node_count = moo_node_wo_child_count(node);
		int node_idx;
		MooNodeWo *child_node_it = NULL;
		for(node_idx=0; node_idx<node_count; node_idx++) {
			MooNodeWo *child_node = moo_node_wo_child_at(node, node_idx);
			if (moo_node_wo_get_content(child_node, moo_module_content_wo_key())==NULL) {
				MooResourceContentWo *rc = (MooResourceContentWo *) moo_node_wo_get_content(child_node, moo_resource_content_wo_key());
				VipNode *child_vip_node = moo_resource_content_wo_get_viper_node(rc);
				if (child_vip_node!=NULL) {
					if (vip_node_get_id(child_vip_node)==vip_node_get_id(vip_node)) {
						cat_ref_swap(child_node_it, child_node);
						node_idx = node_count;
					}
				}
			}
		}

		if (child_node_it==NULL) {

			if (cat_string_wo_equal(node_name, CAT_S(moo_s_settings_filename))) {
				VipNodePath *vnp = vip_node_path_from_tree_node((CatTreeNode *) tree_node);
				VipPath *path = vip_node_path_create_path(vnp);
				VipPath *parent_path = vip_path_create_parent_path(path);
				moo_add_module_from_path(moo_service, parent_path);
				cat_log_debug("parent_path=%o", parent_path);
				cat_unref_ptr(parent_path);
				cat_unref_ptr(path);
				cat_unref_ptr(vnp);
			} else {

				MooNodeWo *e_root_node = moo_node_wo_ensure_editable(node, NULL);
				long long nvipid = vip_isequence_next((VipISequence *) moo_service);
				MooContentMapWo *e_content_map = moo_content_map_wo_new();
				MooResourceContentWo *rc_content = moo_resource_content_wo_new(tree_node, NULL);
				moo_content_map_wo_set(e_content_map, (MooIContent *) rc_content);
				moo_content_map_wo_create_link(e_content_map, moo_iservices_content_key(), moo_resource_content_wo_key());
				cat_log_debug("editable_cmap=%o", e_content_map);
				cat_unref_ptr(rc_content);
				child_node_it = moo_node_wo_new(nvipid, cat_ref_ptr(node_name), NULL, e_content_map, 90);
				moo_node_wo_append_child(e_root_node, child_node_it);
	//			cat_unref_ptr(added_moo_node);
				cat_unref_ptr(e_content_map);
				moo_transaction_commit(tx, e_root_node);
				// TODO loop on failing transaction
			}
		}
		cat_unref_ptr(tx);

		if (!opened_first && child_node_it!=NULL) {
			opened_first = TRUE;
			l_service_open_new_file_editor(service, child_node_it);
		}
		cat_unref_ptr(child_node_it);
	}
	cat_unref_ptr(iterator);
}


static int l_editor_region_match_cb(const LeaIRegion *region, gconstpointer data) {
	int score = 0;
	if (LEA_IS_SURFACE(region)) {
		LeaSurface *surface = LEA_SURFACE(region);
		GtkAllocation alloc;
		gtk_widget_get_allocation((GtkWidget *) surface, &alloc);
		cat_log_debug("region=%o, alloc.width=%d, alloc.height=%d", region, alloc.width, alloc.height);
		score = (int) (sqrt((double) alloc.width*(double) alloc.width*(double) alloc.height))/100;
		LeaSurfaceTabModel *tab_model = lea_surface_get_tab_model(surface);
		CatIIterator *iter = lea_surface_tab_model_iterator(tab_model);
		if (iter) {
			while(cat_iiterator_has_next(iter)) {
				LeaSurfaceTab *tab = (LeaSurfaceTab *) cat_iiterator_next(iter);
				GtkWidget *widget_on_tab = lea_surface_tab_get_widget(tab);
				if (ELK_IS_EDITOR_PANEL(widget_on_tab)) {
					score+=5;
				}
			}
			cat_unref_ptr(iter);
		}
	}
	cat_log_debug("region=%o, score=%d", region, score);
	return score;
}

static gboolean l_grb_foc(gpointer user_data) {
	gtk_widget_grab_focus((GtkWidget *) user_data);
	return FALSE;
}


static void l_service_open_new_file_editor(ElkIService *service, MooNodeWo *moo_node) {
	ElkService *elk_service = ELK_SERVICE(service);
	GtkWidget *editor = NULL;


	/* test if editor isn't already open */
	ElkPreferencesWo *a_elk_prefs = l_elk_get_preferences(elk_service);
	if (!elk_preferences_wo_multiple_resource_editors(a_elk_prefs)) {
		CatArrayWo *all_panels = dra_panel_owner_enlist_all((DraPanelOwner *) elk_service->panel_owner, NULL);
		CatIIterator *iter = cat_array_wo_iterator(all_panels);
		while(cat_iiterator_has_next(iter)) {
			GObject *find_editor = cat_iiterator_next(iter);
			if (ELK_IS_EDITOR_PANEL(find_editor)) {
				ElkEditorPanel *editor_panel = (ElkEditorPanel *) find_editor;
				if (elk_editor_panel_get_moose_node(editor_panel) == moo_node) {
					editor = cat_ref_ptr(find_editor);
					break;
				}
			}
		}
		cat_unref_ptr(iter);
		cat_unref_ptr(all_panels);
	}

	GtkWidget *outline = NULL;

	if (editor==NULL) {
		CatArrayWo *e_editor_list = elk_service_enlist_editor_factories(elk_service, moo_node);
		cat_log_debug("editor_list=%o, moo_node=%o", e_editor_list, moo_node);
		ElkIResourceEditorFactory *factory = (ElkIResourceEditorFactory *) cat_array_wo_get_first(e_editor_list);
		if (factory) {
			cat_log_debug("factory=%o", factory);
			ElkIResourceEditorFactoryInterface *factory_class = ELK_IRESOURCE_EDITOR_FACTORY_GET_INTERFACE(factory);
			editor = factory_class->createEditor(factory, moo_node);
			if (factory_class->createOutline) {
				outline = factory_class->createOutline(factory, moo_node, editor);
			}
		} else {
			MooResourceContentWo *moo_res_content = (MooResourceContentWo *) moo_node_wo_get_content(moo_node, moo_resource_content_wo_key());
			cat_log_debug("moo_res_content=%o", moo_res_content);
			if (moo_res_content) {
				VipNode *vip_node = moo_resource_content_wo_get_viper_node(moo_res_content);
				if (vip_node!=NULL) {
					VipIResource *vip_res =  vip_node_get_content(vip_node);
					if (VIP_IS_IFILE(vip_res)) {
						ElkDocumentBin *document_bin = elk_document_io_open_document_for_file(elk_service->document_io, VIP_IFILE(vip_res));
						editor = (GtkWidget *) elk_editor_panel_new((LeaIPanelOwner *) elk_service->panel_owner, document_bin, NULL);
						elk_editor_panel_set_moose_node((ElkEditorPanel *) editor, moo_node);
					}
				}
			}
		}
		cat_unref_ptr(e_editor_list);
	}

	if (editor) {
		LeaSurface *surface = lea_panel_get_surface(LEA_PANEL(editor));
		if (surface==NULL) {
			LeaFrame *frame = elk_service->frame;
			LeaSurfaceHandler *surface_handler = lea_frame_get_surface_handler(frame);
			surface = lea_surface_handler_find_matching_surface(surface_handler, (LeaMatchFunc) l_editor_region_match_cb, NULL);
			if (surface == NULL) {
				surface = lea_surface_handler_create_surface(surface_handler);
				if (gtk_widget_get_parent(GTK_WIDGET(surface)) == NULL) {
					gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(surface));
				}
			}
			LeaSurfaceTabModel *tab_model = lea_surface_get_tab_model(surface);
			LeaSurfaceTab *new_tab = lea_surface_tab_new(editor, NULL);
			lea_surface_tab_model_add_tab(tab_model, new_tab);
			cat_unref_ptr(new_tab);

			if (outline) {
				new_tab = lea_surface_tab_new(outline, cat_string_wo_new("outline"));
				lea_surface_tab_model_add_tab(tab_model, new_tab);
				cat_unref_ptr(new_tab);
			}


		}
		lea_surface_set_selected_tab(surface, editor);
		gtk_widget_grab_focus(editor);

		g_idle_add((GSourceFunc) l_grb_foc, editor);
	}
}


static GtkWidget *l_service_create_empty_editor(ElkIService *service) {
	ElkService *elk_service = ELK_SERVICE(service);
	LeaFrame *frame = elk_service->frame;

	ElkDocumentBin *document_bin = elk_document_io_open_new_document(elk_service->document_io);
	ElkEditorPanel *editor_panel = elk_editor_panel_new((LeaIPanelOwner *) elk_service->panel_owner, document_bin, NULL);

	LeaSurface *surface = lea_panel_get_surface(LEA_PANEL(editor_panel));
	if (surface==NULL) {
		LeaSurfaceHandler *surface_handler = lea_frame_get_surface_handler(frame);
		surface = lea_surface_handler_find_matching_surface(surface_handler, (LeaMatchFunc) l_editor_region_match_cb, NULL);
		if (surface == NULL) {
			surface = lea_surface_handler_create_surface(surface_handler);
			if (gtk_widget_get_parent(GTK_WIDGET(surface)) == NULL) {
				gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(surface));
			}
		}
		LeaSurfaceTabModel *tab_model = lea_surface_get_tab_model(surface);
		LeaSurfaceTab *new_tab = lea_surface_tab_new((GtkWidget *) editor_panel, NULL);
		lea_surface_tab_model_add_tab(tab_model, new_tab);
		cat_unref_ptr(new_tab);
	}
//	cat_unref(editor_panel);
	MooTransaction *tx = moo_service_create_transaction((GObject *) elk_service, elk_service->moo_service);


	long long next_moo_id = vip_isequence_next((VipISequence *) elk_service->moo_service);

	MooContentMapWo *e_content_map = moo_content_map_wo_new();
	MooDirectContentWo *direct_content = moo_direct_content_wo_new();
	moo_content_map_wo_set(e_content_map, (MooIContent *) direct_content);
	cat_unref_ptr(direct_content);

	MooResourceContentWo *rc_content = moo_resource_content_wo_new(NULL, NULL);
	moo_content_map_wo_set(e_content_map, (MooIContent *) rc_content);
	moo_content_map_wo_create_link(e_content_map, moo_iservices_content_key(), moo_resource_content_wo_key());
	cat_log_debug("editable_cmap=%o", e_content_map);
	cat_unref_ptr(rc_content);

	MooNodeWo *new_child = moo_node_wo_new(next_moo_id, elk_document_bin_get_name(document_bin), NULL, e_content_map, 120);
	cat_unref_ptr(e_content_map);

	while(TRUE) {
		moo_transaction_begin(tx);
		MooNodeWo *root_node = moo_transaction_get_tx_root_node(tx);

		MooNodeWo *e_root_node = moo_node_wo_ensure_editable(root_node, NULL);
		moo_node_wo_append_child(e_root_node, new_child);
		if (moo_transaction_commit(tx, e_root_node)) {
			break;
		}
	}
	cat_unref_ptr(tx);

	elk_editor_panel_set_moose_node(editor_panel, new_child);

	lea_surface_set_selected_tab(surface, (GtkWidget *) editor_panel);
	cat_unref_ptr(new_child);

	return (GtkWidget *) editor_panel;
}

static gboolean l_service_select_and_save_resource_editors(ElkIService *service, CatArrayWo *selected_editors) {
	gboolean result = TRUE;
	ElkService *elk_service = ELK_SERVICE(service);

	cat_log_debug("selected_editors=%o", selected_editors);
	if (selected_editors==NULL) {
		selected_editors = dra_panel_owner_enlist_all((DraPanelOwner *) elk_service->panel_owner, NULL);
	} else {
		cat_ref_ptr(selected_editors);
	}

	CatArrayWo *e_unsaved_list = cat_array_wo_new();
	CatIIterator *iterator = cat_array_wo_iterator(selected_editors);
	while(cat_iiterator_has_next(iterator)) {
		GObject *editor = (GObject *) cat_iiterator_next(iterator);
		if (DRA_IS_EDITOR_PANEL(editor)) {
			DraEditorPanelClass *pan_class = DRA_EDITOR_PANEL_GET_CLASS(editor);
			if (!pan_class->isSaved((DraEditorPanel *) editor)) {
				cat_array_wo_append(e_unsaved_list, editor);
			}
		}
	}
	cat_unref_ptr(iterator);

	int count = cat_array_wo_size(e_unsaved_list);
	cat_log_debug("count=%d", count);

	if (count==0) {
		result = TRUE;
		cat_unref_ptr(e_unsaved_list);
//	} else if (count==1) {
	} else {
		CatArrayWo *a_unsaved_list = cat_array_wo_anchor(e_unsaved_list, 0);
		LeaFrame *frame = elk_service->frame;
		GtkWidget *top_window = gtk_widget_get_toplevel(GTK_WIDGET(frame));
		cat_log_debug("top_window=%o", top_window);
		ElkMultiChooseDialog *choose_dialog = elk_multi_choose_dialog_new(top_window, a_unsaved_list);
		cat_unref_ptr(a_unsaved_list);
		int response = elk_multi_choose_dialog_run(choose_dialog);
		CatArrayWo *e_save_list = elk_multi_choose_dialog_get_selected(choose_dialog);
		cat_ref_ptr(e_save_list);
		gtk_widget_destroy(GTK_WIDGET(choose_dialog));

		if (response==GTK_RESPONSE_OK) {

			ElkAsyncBulkResult *bulk_result = elk_async_bulk_result_new(cat_array_wo_size(e_save_list));
			CatIIterator *iterator = cat_array_wo_iterator(e_save_list);
			result = TRUE;
			while(result && cat_iiterator_has_next(iterator)) {
				DraEditorPanel *editor = (DraEditorPanel *) cat_iiterator_next(iterator);
				DraEditorPanelClass *pan_clss = DRA_EDITOR_PANEL_GET_CLASS(editor);
				pan_clss->save(editor, FALSE, (ChaIOAsync *) bulk_result);
			}
			cat_unref_ptr(iterator);
			result = elk_async_bulk_result_wait(bulk_result);
			cat_unref_ptr(bulk_result);
		} else {
			result = FALSE;
		}
		cat_unref_ptr(e_save_list);
	}
	cat_unref_ptr(selected_editors);
	cat_log_debug("result=%d", result);
	return result;
}


static gboolean l_service_close_multiple_resource_editors(ElkIService *service, CatArrayWo *selected_editors) {
	gboolean result = FALSE;
	ElkService *elk_service = ELK_SERVICE(service);
	if (selected_editors==NULL) {
		selected_editors = dra_panel_owner_enlist_all((DraPanelOwner *) elk_service->panel_owner, NULL);
	} else {
		cat_ref_ptr(selected_editors);
	}

	gboolean did_select = l_service_select_and_save_resource_editors(service, selected_editors);
	cat_log_debug("did_select=%d", did_select);

	result = dra_panel_owner_close_editors((DraPanelOwner *) elk_service->panel_owner, selected_editors, did_select);
	cat_unref_ptr(selected_editors);
	cat_log_debug("result=%d", result);
	return result;
}

static gboolean l_service_save_all_resource_editors(ElkIService *service, CatArrayWo *selected_editors) {
	ElkService *elk_service = ELK_SERVICE(service);
	DraPanelOwner *dra_panel_owner =(DraPanelOwner *) elk_service->panel_owner;
	if (selected_editors==NULL) {
		selected_editors = dra_panel_owner_enlist_all(dra_panel_owner, NULL);
	} else {
		cat_ref_ptr(selected_editors);
	}

	gboolean result = TRUE;
	if (selected_editors) {
		dra_panel_owner_save_editors(dra_panel_owner, selected_editors);
		cat_unref_ptr(selected_editors);
	}

	return result;
}



static void l_service_iface_init(ElkIServiceInterface *iface) {
	iface->exit = l_service_exit;
	iface->showPreferences = l_service_show_preferences;
	iface->selectAndOpenResources = l_service_select_and_open_resources;
	iface->openResourceList = l_service_open_resource_list;
//	iface->openResourceEditor = l_service_open_resource_editor;
	iface->openNewFileEditor = l_service_open_new_file_editor;
	iface->createEmptyEditor = l_service_create_empty_editor;
	iface->closeMultipleResourceEditors = l_service_close_multiple_resource_editors;
	iface->saveAllResourceEditors = l_service_save_all_resource_editors;
}



