/*
   File:    natpad-window.c
   Project: natpad
   Author:  Douwe Vos
   Date:    Oct 23, 2015
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

#include "natpad-window.h"
//#include <cuscus.h>
#include <leafhopper.h>
#include <jaguar.h>
#include <cheetah.h>
#include <chameleon.h>
#include <spider.h>
#include <grossruntime.h>
#include <grosseditor.h>
#include <tern.h>
#include <mule.h>
#include <jordanella.h>
#include <xantus.h>
#include <moose.h>
#include <mooseexplorer.h>
#include <worm.h>
//#ifdef G_OS_UNIX
//#include <vte/vte.h>
//#endif

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "NatpadWindow"
#include <logging/catlog.h>

struct _NatpadWindowPrivate {
	ElkPreferencesService *preference_service;
	VipCdProvider *cd_provider;
	VipService *vip_service;
	MooService *moo_service;
	ChaDocumentManager *document_manager;
	ElkService *elk_service;
	JagService *jag_service;
	CheService *che_service;
	TerService *ter_service;
	MulService *mul_service;
	JorService *jor_service;
	XanService *xan_service;
//	CusService *cus_service;
	SpiService *spi_service;
	WorService *wor_service;
	GroEdService *groed_service;
	LeaProvGroupMain *group_main;
	LeaFrame *frame;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(NatpadWindow, natpad_window, GTK_TYPE_APPLICATION_WINDOW,
		G_ADD_PRIVATE(NatpadWindow)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void natpad_window_class_init(NatpadWindowClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void natpad_window_init(NatpadWindow *instance) {
}

#define l_force_dispose(field) \
		if (priv->field) { \
			cat_log_trace(#field ":rc=%d", G_OBJECT(priv->field)->ref_count); \
			g_object_run_dispose(G_OBJECT(priv->field)); \
			cat_log_trace(#field ":rc=%d", G_OBJECT(priv->field)->ref_count); \
			cat_unref_ptr(priv->field); \
		}


static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	NatpadWindow *instance = NATPAD_WINDOW(object);
	NatpadWindowPrivate *priv = natpad_window_get_instance_private(instance);
	if (priv->wor_service) {
		wor_service_request_shutdown(priv->wor_service);
	}
	l_force_dispose(document_manager);
	l_force_dispose(elk_service);
	l_force_dispose(moo_service);
	l_force_dispose(groed_service);
//	l_force_dispose(cus_service);
	l_force_dispose(spi_service);
	l_force_dispose(jor_service);
	l_force_dispose(xan_service);
	l_force_dispose(mul_service);
	l_force_dispose(ter_service);
	l_force_dispose(che_service);
	l_force_dispose(jag_service);
	l_force_dispose(vip_service);
	l_force_dispose(wor_service);
	l_force_dispose(preference_service);
	l_force_dispose(cd_provider);

	cat_unref_ptr(priv->group_main);


	G_OBJECT_CLASS(natpad_window_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(natpad_window_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_set_icon(GtkWindow *top);
static gboolean l_close_window_cb(GtkWidget *widget, GdkEvent *eev, gpointer main_ptr);

NatpadWindow *natpad_window_new(GApplication *application, ElkPreferencesService *preference_service, VipCdProvider *cd_provider) {
	NatpadWindow *result = g_object_new(NATPAD_TYPE_WINDOW, "application", application, NULL);
	cat_ref_anounce(result);
	NatpadWindowPrivate *priv = natpad_window_get_instance_private(result);
	priv->preference_service = cat_ref_ptr(preference_service);
	priv->cd_provider = cat_ref_ptr(cd_provider);

	g_signal_connect(G_OBJECT(result), "delete-event", G_CALLBACK(l_close_window_cb), result);

	WorService *wor_service = wor_service_new();
	priv->wor_service = wor_service;

	priv->vip_service = vip_service_new(wor_service, priv->cd_provider);
	VipMapperRegistry *mapper_registry = vip_service_get_mapper_registry(priv->vip_service);

	JagJarFileMapper *jaguar_mapper = jag_jar_file_mapper_new((VipISequence *) priv->vip_service);
	vip_mapper_registry_add_mapper(mapper_registry, (VipIMapper *) jaguar_mapper);
	cat_unref(jaguar_mapper);

	vip_service_start(priv->vip_service);

	priv->moo_service = moo_service_new(priv->wor_service, priv->vip_service);


	ElkPreferencesContainer *prefs_container = elk_preferences_service_get_container(priv->preference_service);

	gtk_widget_set_size_request((GtkWidget *) result, 1040, 350);

	LeaFrame *frame = lea_frame_new((GtkWindow *) result);
	priv->frame = frame;
	gtk_container_add(GTK_CONTAINER(result), GTK_WIDGET(frame));

	priv->group_main = lea_prov_group_main_new();
	lea_frame_merge_action_group(frame, (LeaActionGroup *) priv->group_main);


	LeaSurfaceHandler *surface_handler = lea_frame_get_surface_handler(frame);
	LeaSurfaceBox *surface_box = lea_surface_handler_get_root(surface_handler);


	priv->elk_service = elk_service_new(frame, priv->moo_service, priv->preference_service, application);

	MooPanelOwner *moo_panel_owner = moo_explorer_service_get_panel_owner(priv->elk_service->moo_explorer_service);
	MooNodeRenderRegistry *moo_render_registry = moo_panel_owner_get_render_registry(moo_panel_owner);
	JagNodeRendererFactory *render_factory = jag_node_renderer_factory_new();
	moo_node_render_registry_add_render_factory(moo_render_registry, (MooINodeRendererFactory *) render_factory);
	cat_unref_ptr(render_factory);



	LeaSurface *surface_moose = lea_surface_new();
	gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(surface_moose));
	lea_surface_box_add(surface_box, (LeaIRegion *) surface_moose, 0, 200);

	MooNodeRenderRegistry *registry = moo_panel_owner_get_render_registry(moo_panel_owner);
	ElkExplorer *moo_explorer = elk_explorer_new((ElkIService *) priv->elk_service, priv->moo_service, registry);
	MooPanel *moo_panel = moo_panel_new((LeaIPanelOwner *) moo_panel_owner, frame, priv->moo_service,(MooExplorer *) moo_explorer);
	LeaSurfaceTabModel *tab_model = lea_surface_get_tab_model(surface_moose);
	LeaSurfaceTab *moose_tab = lea_surface_tab_new(GTK_WIDGET(moo_panel), NULL);
	lea_surface_tab_model_add_tab(tab_model, moose_tab);
	cat_unref(moose_tab);
	cat_unref_ptr(surface_moose);
//	cat_unref_ptr(moo_explorer);
//	cat_unref(moo_panel);



	priv->jag_service = jag_service_new(wor_service, priv->elk_service, priv->moo_service);
	priv->che_service = che_service_new(wor_service, priv->elk_service);

	priv->ter_service = ter_service_new(wor_service, priv->elk_service, priv->vip_service);
	priv->mul_service = mul_service_new(wor_service, priv->elk_service);

	priv->jor_service = jor_service_new(wor_service, priv->elk_service);
	priv->xan_service = xan_service_new(wor_service, priv->elk_service);
//	priv->cus_service = cus_service_new(wor_service, priv->elk_service);
	priv->spi_service = spi_service_new(wor_service, priv->elk_service);
	priv->groed_service = groed_service_new(wor_service, priv->elk_service);


	elk_service_add_resource_handler(priv->elk_service, ELK_IRESOURCE_HANDLER(priv->jag_service));
	elk_service_add_resource_handler(priv->elk_service, ELK_IRESOURCE_HANDLER(priv->jor_service));
	elk_service_add_resource_handler(priv->elk_service, ELK_IRESOURCE_HANDLER(priv->xan_service));
	elk_service_add_resource_handler(priv->elk_service, ELK_IRESOURCE_HANDLER(priv->che_service));
	elk_service_add_resource_handler(priv->elk_service, ELK_IRESOURCE_HANDLER(priv->ter_service));
	elk_service_add_resource_handler(priv->elk_service, ELK_IRESOURCE_HANDLER(priv->mul_service));
//	elk_service_add_resource_handler(priv->elk_service, ELK_IRESOURCE_HANDLER(priv->cus_service));
	elk_service_add_resource_handler(priv->elk_service, ELK_IRESOURCE_HANDLER(priv->spi_service));
	elk_service_add_resource_handler(priv->elk_service, ELK_IRESOURCE_HANDLER(priv->groed_service));


//	LeaSurface *surface_ast = lea_surface_new( frame, 400);
//	gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(surface_ast));
//	lea_surface_box_add(surface_box, (LeaIRegion *) surface_ast, 1, 800);
//	cat_unref(surface_ast);


	LeaSurfaceBox *box2 = lea_surface_box_new(LEA_SB_VERTICAL);
	lea_surface_box_add(surface_box, (LeaIRegion *) box2, 1, 800);

	LeaSurface *surface_ast = lea_surface_new();
	gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(surface_ast));
	lea_surface_box_add(box2, (LeaIRegion *) surface_ast, 0, 800);
	cat_unref(surface_ast);


#ifdef G_OS_UNIX

//	LeaSurface *surface_vte = lea_surface_new();
//
//	gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(surface_vte));
//	lea_surface_box_add(box2, (LeaIRegion *) surface_vte, 1, 200);
//
//
//	GtkWidget *term = vte_terminal_new();
//	const char *working_dir = g_get_home_dir ();
//
//
//	GdkColor bg_color;
//	gdk_color_parse("#FFFFFF", &bg_color);
//
//	GdkColor fg_color;
//	gdk_color_parse("#222222", &fg_color);
//
//
//	GdkColor palette[8];
//	gdk_color_parse("#2E3436", &palette[0]);
//	gdk_color_parse("#CC0000", &palette[1]);
//	gdk_color_parse("#4E9A06", &palette[2]);
//	gdk_color_parse("#C4A000", &palette[3]);
//	gdk_color_parse("#1C4E8D", &palette[4]);
//	gdk_color_parse("#75507B", &palette[5]);
//	gdk_color_parse("#06989A", &palette[6]);
//	gdk_color_parse("#D3D7CF", &palette[7]);
//
//	vte_terminal_set_colors((VteTerminal *) term, &fg_color, &bg_color, palette, 8);
//	vte_terminal_fork_command((VteTerminal *) term, "/bin/bash", NULL, NULL, working_dir, FALSE, FALSE, FALSE);
//
//
//
//	LeaSurfaceTabModel *vte_tab_model = lea_surface_get_tab_model(surface_vte);
//	LeaSurfaceTab *vte_tab = lea_surface_tab_new(GTK_WIDGET(term), NULL);
//	lea_surface_tab_model_add_tab(vte_tab_model, vte_tab);
//
//	cat_unref(surface_vte);
//	cat_unref(vte_tab);
#endif

	cat_unref_ptr(box2);

	l_set_icon(GTK_WINDOW(result));

	gtk_window_set_title(GTK_WINDOW(result), "Natpad");

	gtk_widget_show_all((GtkWidget *) frame);


	ElkPreferencesWo *a_elk_prefs = elk_preferences_container_get(prefs_container);

	if (elk_preferences_wo_full_screen(a_elk_prefs)) {
		gtk_window_maximize(GTK_WINDOW(result));
	}

	return result;
}


#ifdef NATPAD_TESTING
static CatStringWo *l_current_work_directory() {
	char *data = g_malloc(4096);
	char *txt = getcwd(data, 4096);
	CatStringWo *a_result = NULL;
	if (txt) {
		a_result= cat_string_wo_new_with(txt);
	}
	g_free(data);
	return a_result;
}

static CatStringWo *l_parent_dir(CatStringWo *ca_in) {
	CatStringWo *e_result = NULL;
	int lidx = cat_string_wo_last_index_of(ca_in, '/');
	if (lidx>0) {
		e_result = cat_string_wo_new_sub(ca_in, 0, lidx);
	} else {
		e_result = cat_string_wo_clone(ca_in, CAT_CLONE_DEPTH_MAIN);
	}
	cat_unref_ptr(ca_in);
	return e_result;
}
#endif

static void l_set_icon(GtkWindow *top) {
//	NatConfigurationService *configuration_service = nat_system_get_configuration_service(system);
	GError *err = NULL;


#ifndef NATPAD_TESTING
	gtk_window_set_icon_from_file(top, "/usr/share/pixmaps/natpad_48.xpm", &err);
#else
	CatStringWo *a_cwd = l_current_work_directory();
	CatStringWo *e_project_home = l_parent_dir(a_cwd);
	cat_string_wo_append_chars(e_project_home, "/natpad_48.xpm");
	const gchar *icon_chrs = cat_string_wo_getchars(e_project_home);
//	printf("icon_chrs=%s\n", icon_chrs);
	gtk_window_set_icon_from_file(top, icon_chrs, &err);
	cat_unref_ptr(e_project_home);
#endif
	if (err==NULL) {
		return;
	}
}


static gboolean l_close_window_cb(GtkWidget *widget, GdkEvent *eev, gpointer main_ptr) {
	cat_log_debug("close window");
	NatpadWindow *instance = NATPAD_WINDOW(main_ptr);
	NatpadWindowPrivate *priv = natpad_window_get_instance_private(instance);
	return !elk_iservice_exit(ELK_ISERVICE(priv->elk_service));
}


void natpad_window_new_open_file_list(NatpadWindow *window, CatArrayWo *file_list) {
	NatpadWindowPrivate *priv = natpad_window_get_instance_private(window);

	CatArrayWo *e_tree_node_array = cat_array_wo_new();
	int docIdx;
	for(docIdx=0; docIdx<cat_array_wo_size(file_list); docIdx++) {
		GFile *doc_file = (GFile *) cat_array_wo_get(file_list, docIdx);
		CatStringWo * a_fname= cat_string_wo_new_with(g_file_get_path(doc_file));
		VipPath *vip_path = vip_path_new(a_fname);
		VipCreatePathRequest *request = vip_service_create_path(priv->vip_service, vip_path);
		cat_unref_ptr(vip_path);
		VipNodePath *node_path = vip_create_path_request_wait_for_path(request);
		CatReadableTreeNode *tree_node = (CatReadableTreeNode *) vip_node_path_get_tail_node(node_path);
		cat_array_wo_append(e_tree_node_array, (GObject*) tree_node);
		cat_unref_ptr(request);
	}

	ELK_ISERVICE_GET_INTERFACE(priv->elk_service)->openResourceList(ELK_ISERVICE(priv->elk_service), e_tree_node_array);
	cat_unref_ptr(e_tree_node_array);
}

void natpad_window_new_open_empty_editor(NatpadWindow *window) {
	NatpadWindowPrivate *priv = natpad_window_get_instance_private(window);
	ELK_ISERVICE_GET_INTERFACE(priv->elk_service)->createEmptyEditor(ELK_ISERVICE(priv->elk_service));
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
