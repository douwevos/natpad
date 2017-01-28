/*
 * gtkwidgetdump.c
 *
 *  Created on: Apr 21, 2010
 */

#include <gtk/gtk.h>
#include <string.h>

static void _dump_widget(GtkWidget *widget, char *indent);


static void _dump_container(GtkContainer *container, char *indent) {
	GList *children = gtk_container_get_children(container);
	int idx = 0;

	int l = strlen(indent)+4;
	char *nw_indent = g_malloc(l);
	int idx2;
	for(idx2=0; idx2<l; idx2++) {
		nw_indent[idx2] = ' ';
	}
	nw_indent[l-1] = 0;

	while(children) {
		GtkWidget *child = children->data;
		printf("%s[%d]\n", indent, idx++);
		_dump_widget(child, nw_indent);
		children = children->next;
	}
	g_free(nw_indent);
}


static void _dump_widget(GtkWidget *widget, char *indent) {
	const char *name = g_type_name_from_instance((GTypeInstance *) widget);
//	gboolean is_toplevel = gtk_widget_is_toplevel(widget);
	gboolean is_focus = gtk_widget_is_focus(widget);
//	gboolean is_composited = gtk_widget_is_composited(widget);

	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);
	printf("%s%s[%d,%d,%d,%d]\n", indent, name, allocation.x, allocation.y, allocation.width, allocation.height);


//	printf("%s-is_top_level        %d\n", indent, is_toplevel);
	printf("%s-is_focus            %d\n", indent, is_focus);
//	printf("%s-is_composited       %d\n", indent, is_composited);

	if (GTK_IS_CONTAINER(widget)) {
		GtkContainer *container = GTK_CONTAINER(widget);
		_dump_container(container, indent);
	}
}

void gtk_widget_dump(GtkWidget *widget) {
	_dump_widget(widget, "");
}
