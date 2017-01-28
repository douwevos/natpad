/*
 * leascrolledwindowtest.c
 *
 *  Created on: Apr 22, 2011
 *      Author: superman
 */

#include <gdk/gdk.h>
#include <gtk/gtk.h>
#include <caterpillar.h>
#include "../src/leafhopper.h"

int main2(int argc, char **argv) {
	gtk_init(&argc, &argv);

	GtkWidget *top_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_focus_on_map(GTK_WINDOW(top_window), TRUE);
	gtk_widget_set_size_request(top_window, 640, 400);

	GtkWidget *text_view = gtk_text_view_new();
	LeaScrolledWindow *scrolled_window = lea_scrolled_window_new(text_view);
	GtkAllocation alloc;
	alloc.x = 0;
	alloc.y = 0;
	alloc.width = 640;
	alloc.height = 400;
	gtk_widget_set_allocation(GTK_WIDGET(scrolled_window), &alloc);

	gtk_container_add(GTK_CONTAINER(top_window), GTK_WIDGET(scrolled_window));

	gtk_widget_show_all(top_window);

	gtk_main();
	return 0;
}
