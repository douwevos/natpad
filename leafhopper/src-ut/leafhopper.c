/*
 * leafhopper.c
 *
 *  Created on: Nov 18, 2009
 */

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <caterpillar.h>
#include "../src/leafhopper.h"
#include "../src/leaframe.h"
#include "../src/surface/lealayer.h"
#include "../src/menu/leamenuaction.h"
#include "panel-digit/leadigitpanel.h"
#include "panel-digit/leadigitpanelowner.h"
#include "panel-digit/leadigitpanelactiongroup.h"

#include "panel-output/leaoutputpanel.h"
#include "panel-output/leaoutputpanelowner.h"
//#include "panel-output/leaoutputpanelactiongroup.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "Leafhopp"
#include <logging/catlog.h>


static gboolean _close_window(GtkWidget *widget, GdkEvent *eev, gpointer main_ptr) {
	gtk_main_quit();
	return FALSE;
}


static void my_g_log_default_handler(const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data) {
	cat_log_fatal("%s(level:%d) %s", log_domain, log_level, message);
	cat_stacktrace_print();
}

int main(int argc, char **argv) {

	g_log_set_handler("GLib-GObject", G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler("GLib-GObject", G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler("Gdk", G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler("Gdk", G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler("Gtk", G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler(NULL, G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
	g_log_set_handler(NULL, G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);

	gtk_init(&argc, &argv);
	GtkWidget *top_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(top_window), "delete-event", G_CALLBACK(_close_window), NULL);
	gtk_widget_set_size_request(top_window, 640, 400);


	LeaFrame *frame = lea_frame_new();

	LeaDigitPanelOwner *digit_pan_own = lea_digit_panel_owner_new(frame);

	gtk_container_add(GTK_CONTAINER(top_window), (GtkWidget *) frame);

	LeaSurfaceHandler *surface_handler = lea_frame_get_surface_handler(frame);
	LeaSurfaceBox *surface_box = lea_surface_handler_get_root(surface_handler);
	LeaSurface *surface = lea_surface_new();

	gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(surface));
	lea_surface_box_add(surface_box, (LeaIRegion *) surface, 0, 1000);
	cat_unref(surface);

	GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	gtk_container_add(GTK_CONTAINER(surface), box);
	GtkWidget *but = gtk_button_new_with_label("Moinsen");
	gtk_box_pack_start(GTK_BOX(box), but, FALSE, FALSE, 0);
	GtkWidget *sw = gtk_scrolled_window_new(NULL, NULL);
	gtk_box_pack_start(GTK_BOX(box), sw, FALSE, FALSE, 0);
	GtkWidget *text_view = gtk_text_view_new();
	gtk_container_add(GTK_CONTAINER(sw), text_view);



	LeaSurfaceTabModel *model = lea_surface_get_tab_model(surface);
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 1);
	LeaScrolledWindow *windo = lea_scrolled_window_new(box);
	LeaPanel *p = lea_panel_new(LEA_IPANEL_OWNER(digit_pan_own), frame, cat_string_wo_new_with("haha"), (GtkWidget *) windo);

	LeaSurfaceTab *tab = lea_surface_tab_new((GtkWidget *) p, cat_string_wo_new_with("addt"));
	lea_surface_tab_model_add_tab(model, tab);
	cat_unref_ptr(tab);
//	gtk_container_add(GTK_CONTAINER(surface), box);
	but = gtk_button_new_with_label("check");
	gtk_box_pack_start(GTK_BOX(box), but, FALSE, FALSE, 0);
	but = gtk_button_new_with_label("check it");
	gtk_box_pack_start(GTK_BOX(box), but, FALSE, FALSE, 0);




	LeaSurface *surface2 = lea_surface_new();
	gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(surface2));
	lea_surface_box_add(surface_box, (LeaIRegion *) surface2, 1, 1000);
	cat_unref(surface2);

	gtk_widget_set_visible(top_window, TRUE);
	gtk_widget_show_all(top_window);
	gtk_main();

//	gtk_widget_destroy(frame);


#ifdef NATPAD_TESTING

#ifdef MONITOR_CLAZZ
	cat_ref_report();
	cat_ref_summary();
	cat_ref_cleanup();
#endif

	cat_ref_entry_report();
#endif


	return 0;
}


//static gboolean _close_window(GtkWidget *widget, GdkEvent *eev, gpointer main_ptr) {
//	gtk_main_quit();
//	cat_log_debug("done2");
//
//	return FALSE;
//}
//
//
//
//
//int main(int argc, char **argv) {
//
//	g_log_set_handler("GLib-GObject", G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
//	g_log_set_handler("GLib-GObject", G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);
//	g_log_set_handler("Gdk", G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
//	g_log_set_handler("Gdk", G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);
//	g_log_set_handler("Gtk", G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
//	g_log_set_handler("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);
//	g_log_set_handler(NULL, G_LOG_LEVEL_CRITICAL, (GLogFunc) my_g_log_default_handler, NULL);
//	g_log_set_handler(NULL, G_LOG_LEVEL_WARNING, (GLogFunc) my_g_log_default_handler, NULL);
//
//	gtk_init(&argc, &argv);
//
////	g_log_set_always_fatal(G_LOG_LEVEL_WARNING | G_LOG_FLAG_FATAL | G_LOG_LEVEL_CRITICAL
////                    | G_LOG_FLAG_RECURSION);
//
//
//	GtkWidget *top_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
//	g_signal_connect(G_OBJECT(top_window), "delete-event", G_CALLBACK(_close_window), NULL);
//
//	gtk_window_set_focus_on_map(GTK_WINDOW(top_window), TRUE);
//	gtk_widget_set_size_request(top_window, 640, 400);
//
//	LeaFrame *frame = lea_frame_new(GTK_WINDOW(top_window));
//	LeaLayout *layout = lea_frame_get_layout(frame);
//	LeaLayoutConfiguration *configuration = lea_layout_get_configuration(layout);
//////	gtk_container_add(GTK_CONTAINER (top_window), lea);
//
//	LeaWidgetLeaf *leaf = lea_widget_leaf_new(configuration, frame, 100);
//	lea_frame_add_child(frame, leaf);
//	LeaSplitBox *top_split_box = lea_layout_get_root(layout);
//	lea_split_box_add_at(top_split_box, LEA_INODE(leaf), 0);
//	cat_unref(leaf);
//
//
//	LeaDigitPanelOwner *digit_panel_owner = lea_digit_panel_owner_new(frame);
//
//
//	LeaPanel *digit_panel = (LeaPanel *) lea_digit_panel_new(LEA_IPANEL_OWNER(digit_panel_owner), frame);
//	cat_unref(digit_panel_owner);
//
////	LeaDigitPanel *digit_panel = lea_digit_panel_new(frame);
//	lea_widget_leaf_add_panel(leaf, LEA_PANEL(digit_panel), -1);
//
////	gtk_widget_show_all(GTK_WIDGET(LEA_MENU(lea_frame_get_menu_bar(frame))->menu_shell));
//
////	lea_widget_leaf_add_panel(leaf, LEA_PANEL(lea_frame->document_list));
//
//
////#ifdef TESTBIG
//
//	LeaWidgetLeaf *leaf2 = lea_widget_leaf_new(configuration, frame, 100);
//	lea_frame_add_child(frame, leaf2);
//	lea_split_box_add_at(top_split_box, LEA_INODE(leaf2), 1);
//	cat_unref(leaf2);
//
//
//
//	LeaOutputPanelOwner *output_panel_owner = lea_output_panel_owner_new(frame);
//	LeaOutputPanel *output_panel = lea_output_panel_new((LeaIPanelOwner *) output_panel_owner, frame);
//	lea_widget_leaf_add_panel(leaf2, LEA_PANEL(output_panel), -1);
//
//
//	int pidx;
//	for(pidx=0; pidx<4; pidx++) {
//		LeaOutputPanel *output_panel2 = lea_output_panel_new((LeaIPanelOwner *) output_panel_owner, frame);
//		lea_widget_leaf_add_panel(leaf2, LEA_PANEL(output_panel2), -1);
////		cat_unref(output_panel2);
//	}
//
//	cat_unref_ptr(output_panel_owner);
//
////#endif
//
//	lea_layout_dump_tree(layout);
//
//
//
////	output_panel = lea_output_panel_new(frame);
////	lea_widget_leaf_add_panel(leaf2, LEA_PANEL(output_panel));
//
//
//	gtk_widget_show_all(top_window);
////	lea_layout_dump_tree(layout);
//
//	gtk_main();
////	cat_ref_report();
//
////	cat_ref_summary();
////	cat_ref_cleanup();
//	cat_log_debug("done");
//
//#ifdef MONITOR_CLAZZ
//	cat_ref_report();
//	cat_ref_summary();
//	cat_ref_cleanup();
//#endif
//
//
//	cat_ref_entry_report();
//
//	return -1;
//}
