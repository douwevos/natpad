/*
   File:    chameleontest.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 22, 2015
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

#include <gtk/gtk.h>
#include "../src/chaeditor.h"
#include "../src/io/chadocumentmanager.h"


#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChameleonTest"
#include <logging/catlog.h>

static gboolean l_close_window(GtkWidget *widget, GdkEvent *eev, gpointer main_ptr) {
	gtk_main_quit();
	return FALSE;
}


static void my_g_log_default_handler(const gchar *log_domain, GLogLevelFlags log_level, const gchar *message, gpointer user_data) {
	cat_log_fatal("%s(level:%d) %s", log_domain, log_level, message);
	cat_stacktrace_print();

#ifdef NATPAD_TESTING

#ifdef MONITOR_CLAZZ
	cat_ref_report();
	cat_ref_summary();
	cat_ref_cleanup();
#endif

	cat_ref_entry_report();
#endif
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

//	ChaEditorService *editor_service = cha_editor_service_new();

	GtkWidget *top_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(G_OBJECT(top_window), "delete-event", G_CALLBACK(l_close_window), NULL);
	gtk_widget_set_size_request(top_window, 512, 368);


	GtkScrolledWindow *scrolled_window = (GtkScrolledWindow *) gtk_scrolled_window_new(NULL, NULL);
	gtk_container_add((GtkContainer *) top_window, (GtkWidget *) scrolled_window);

	ChaDocumentManager *document_manager = cha_document_manager_new();

	ChaDocument *document = cha_document_new(document_manager);
	ChaEditor *cha_editor = cha_editor_new(document);
	gtk_container_add((GtkContainer *) scrolled_window, (GtkWidget *) cha_editor);



//	GFile *file = g_file_new_for_path("/home/superman/Downloads/gtkimagemenuitem.c");
//	GFile *file = g_file_new_for_path("/media/superman/e1fd92b0-e55b-4d39-8ac5-c7e3a37578dc/home/superman/cpp-workspace/trunk/natpad.ide/chameleon/src/chaeditorservice.c");
//	GFile *file = g_file_new_for_path("/home/superman/work/mimetex.c");
	GFile *file = g_file_new_for_path("/home/superman/cpp-workspace/trunk/natpad.ide/chameleon/src/layout/chadocumentview.c");


	cha_document_manager_load(document_manager, document, file);
	cat_unref_ptr(document);


	gtk_widget_set_visible(top_window, TRUE);
	gtk_widget_show_all(top_window);
	gtk_widget_grab_focus((GtkWidget *) cha_editor);
	gtk_main();


	cat_unref_ptr(document_manager);


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
