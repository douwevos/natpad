/*
   File:    dratagoverviewbar.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 19, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "dratagoverviewbar.h"
#include "draeditorpanel.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "DraTagOverviewBar"
#include <logging/catlog.h>

#define ZONE_SCALE 4096

struct _DraTagOverviewBarPrivate {
	CatIntArrayWo *tagged_lines;
	int total_line_count;

	CatIntArrayWo *zones;
	int zones_total_line_count;
	CatIntArrayWo *zondes_tagged_lines;
	int zones_view_height;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraTagOverviewBar, dra_tag_overview_bar, GTK_TYPE_WIDGET,
		G_ADD_PRIVATE(DraTagOverviewBar)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_widget_realize(GtkWidget *widget);
static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation);
static gboolean l_widget_draw(GtkWidget *widget, cairo_t *cr);
static void l_widget_get_preferred_width(GtkWidget *widget, gint *minimum_width, gint *natural_width);

static void dra_tag_overview_bar_class_init(DraTagOverviewBarClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(clazz);
	widget_class->realize = l_widget_realize;
	widget_class->size_allocate = l_widget_size_allocate;
	widget_class->draw = l_widget_draw;
	widget_class->get_preferred_width = l_widget_get_preferred_width;
}

static void dra_tag_overview_bar_init(DraTagOverviewBar *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraTagOverviewBar *instance = DRA_TAG_OVERVIEW_BAR(object);
//	DraTagOverviewBarPrivate *priv = dra_tag_overview_bar_get_instance_private(instance);
	G_OBJECT_CLASS(dra_tag_overview_bar_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_tag_overview_bar_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static gboolean l_button_press_event_cb(GtkWidget *gwidget, GdkEventButton *eev, gpointer data);
static gboolean l_motion_event_cb(GtkWidget *gwidget, GdkEventMotion *eev, gpointer data);


DraTagOverviewBar *dra_tag_overview_bar_new() {
	DraTagOverviewBar *result = g_object_new(DRA_TYPE_TAG_OVERVIEW_BAR, NULL);
	cat_ref_anounce(result);
	DraTagOverviewBarPrivate *priv = dra_tag_overview_bar_get_instance_private(result);
	priv->tagged_lines = NULL;
	priv->total_line_count = 0;

	priv->zondes_tagged_lines = NULL;
	priv->zones = NULL;
	priv->zones_total_line_count = 0;
	priv->zones_view_height = 0;

	GtkWidget *widget = GTK_WIDGET(result);
	gtk_widget_set_events(widget, gtk_widget_get_events(widget)
	                                             | GDK_BUTTON_PRESS_MASK
	                                             | GDK_BUTTON2_MASK
	                                             | GDK_BUTTON3_MASK
	                                             | GDK_BUTTON4_MASK
	                                             | GDK_BUTTON5_MASK
	                                             | GDK_POINTER_MOTION_MASK
	                                             | GDK_POINTER_MOTION_HINT_MASK
	                                             );

	g_signal_connect(result, "button-press-event", G_CALLBACK(l_button_press_event_cb), result);
	g_signal_connect(result, "motion-notify-event", G_CALLBACK(l_motion_event_cb), result);


	return result;
}


void dra_tag_overview_bar_set_tagged_lines(DraTagOverviewBar *bar, CatIntArrayWo *tagged_lines, int total_line_count) {
	DraTagOverviewBarPrivate *priv = dra_tag_overview_bar_get_instance_private(bar);

	cat_log_debug("new tagged lines: total_line_count=%d, tagged_lines=%O", total_line_count, tagged_lines);

	if (cat_int_array_wo_equal(priv->tagged_lines, tagged_lines) && (priv->total_line_count==total_line_count)) {
		cat_log_debug("no changes");
		return;
	}

	cat_ref_swap(priv->tagged_lines, tagged_lines);
	priv->total_line_count = total_line_count;

	gtk_widget_queue_draw((GtkWidget *) bar);
}



void l_update_zones(DraTagOverviewBar *bar) {
	DraTagOverviewBarPrivate *priv = dra_tag_overview_bar_get_instance_private(DRA_TAG_OVERVIEW_BAR(bar));
	GtkWidget *par = gtk_widget_get_parent((GtkWidget *) bar);
	int hsbheight = 0;
	if (par && GTK_IS_CONTAINER(par)) {
		GList *child_list = gtk_container_get_children((GtkContainer *) par);
		while(child_list) {
			if (GTK_IS_SCROLLED_WINDOW(child_list->data)) {
				GtkWidget *hscrolbar = gtk_scrolled_window_get_hscrollbar((GtkScrolledWindow *) child_list->data);
				hsbheight = gtk_widget_get_allocated_height(hscrolbar);
				break;
			}
			child_list = child_list->next;
		}
	}
	int wheight = gtk_widget_get_allocated_height((GtkWidget *) bar)-hsbheight;

	if ((priv->zones_view_height==wheight) && priv->total_line_count==priv->zones_total_line_count
			&& priv->zondes_tagged_lines==priv->tagged_lines) {
		return;
	}

	priv->zones_view_height = wheight;
	priv->zones_total_line_count = priv->total_line_count;
	priv->zondes_tagged_lines = priv->tagged_lines;


	CatIntArrayWo *zones = NULL;
	if (priv->tagged_lines) {
		long long lastbottom = 0;
		int tcount = cat_int_array_wo_size(priv->tagged_lines);
		cat_log_trace("tcount=%d", tcount);
		int tidx;
		gboolean has_zones = FALSE;
		zones = cat_int_array_wo_new();
		for(tidx=0; tidx<tcount; tidx++) {
			int line_idx = cat_int_array_wo_get(priv->tagged_lines, tidx);
			long long ll = ZONE_SCALE*(long long ) line_idx*wheight / (priv->total_line_count);
			long long top = ll-8000;
			long long bottom = ll+8000;

			if (!has_zones || (top>lastbottom)) {
				if (has_zones) {
					cat_int_array_wo_append(zones, (int) lastbottom);
				}
				cat_log_trace("top=%d", top);
				cat_int_array_wo_append(zones, (int) top);
				has_zones = TRUE;
			}
			lastbottom =  bottom;
		}
		cat_log_detail("has_zones=%s, lastbottom=%d", (has_zones ? "true" : "false"), lastbottom);
		if (has_zones) {
			cat_int_array_wo_append(zones, (int) lastbottom);
		}
	}
	cat_ref_swap(priv->zones, zones);
	cat_unref_ptr(zones);
}


static void l_widget_realize(GtkWidget *widget) {
	gtk_widget_set_realized(widget, TRUE);

	GtkAllocation allocation;
	gtk_widget_get_allocation(widget, &allocation);

	GdkWindowAttr attributes;
	attributes.window_type = GDK_WINDOW_CHILD;
	attributes.x = allocation.x;
	attributes.y = allocation.y;
	attributes.width = allocation.width;
	attributes.height = allocation.height;
	attributes.wclass = GDK_INPUT_OUTPUT;
	attributes.visual = gtk_widget_get_visual (widget);
	attributes.event_mask = GDK_VISIBILITY_NOTIFY_MASK
			| GDK_EXPOSURE_MASK
			| gtk_widget_get_events(widget);

	gint attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL;

	GdkWindow *window = gdk_window_new(gtk_widget_get_parent_window(widget), &attributes, attributes_mask);
	gtk_widget_set_window(widget, window);
	gtk_widget_register_window(widget, window);

}

static void l_widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation) {
	GtkAllocation cur_alloc;
	gtk_widget_get_allocation(widget, &cur_alloc);
	if (cur_alloc.x==allocation->x && cur_alloc.y==allocation->y
			&& cur_alloc.width==allocation->width && cur_alloc.height==allocation->height ) {
		cat_log_debug("breaking !!!");
		return;
	}
	gtk_widget_set_allocation(widget, allocation);

	if (gtk_widget_get_realized(widget)) {
		gdk_window_move_resize(gtk_widget_get_window(widget), allocation->x, allocation->y, allocation->width, allocation->height);
	}
}


static gboolean l_widget_draw(GtkWidget *widget, cairo_t *cr) {
	DraTagOverviewBarPrivate *priv = dra_tag_overview_bar_get_instance_private(DRA_TAG_OVERVIEW_BAR(widget));

	l_update_zones((DraTagOverviewBar *) widget);

	GtkWidget *par = gtk_widget_get_parent(widget);
	int hsbheight = 0;
	if (par && GTK_IS_CONTAINER(par)) {
		GList *child_list = gtk_container_get_children((GtkContainer *) par);
		while(child_list) {
			if (GTK_IS_SCROLLED_WINDOW(child_list->data)) {
				GtkWidget *hscrolbar = gtk_scrolled_window_get_hscrollbar((GtkScrolledWindow *) child_list->data);
				hsbheight = gtk_widget_get_allocated_height(hscrolbar);
				break;
			}
			child_list = child_list->next;
		}
	}
	int wheight = gtk_widget_get_allocated_height(widget)-hsbheight;
	cairo_set_source_rgb(cr, 1.0, 1.0, 1.0);
	cairo_rectangle(cr, 0,0, 20, wheight);
	cairo_fill(cr);

	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
	cat_log_debug("drawwing:priv->tagged_lines=%O", priv->tagged_lines);

	if (priv->zones) {
		int zone_count = cat_int_array_wo_size(priv->zones);
		cat_log_detail("zone_count=%d", zone_count);
		int tidx=0;
		while(tidx<zone_count) {
			int top = cat_int_array_wo_get(priv->zones, tidx++);
			int bottom = cat_int_array_wo_get(priv->zones, tidx++);
			cat_log_trace("top=%d, bottom=%d", top, bottom);
			double ftop = (double) top/ZONE_SCALE;
			double fbottom = (double) bottom/ZONE_SCALE;

			cairo_rectangle(cr, 0, ftop, 20, fbottom-ftop);
			cairo_fill(cr);
		}
	}

	return FALSE;
}

static void l_widget_get_preferred_width(GtkWidget *widget, gint *minimum_width, gint *natural_width) {
	if (minimum_width) {
		*minimum_width = 10;
	}
	if (natural_width) {
		*natural_width = 12;
	}
}


static gboolean l_get_zone(DraTagOverviewBar *bar, int ymouse, int *top_out, int *bottom_out) {
	DraTagOverviewBarPrivate *priv = dra_tag_overview_bar_get_instance_private(bar);
	l_update_zones(bar);
	ymouse = ymouse*ZONE_SCALE;
	if (priv->zones) {
		int zone_count = cat_int_array_wo_size(priv->zones);
		int zone_idx = 0;
		while(zone_idx<zone_count) {
			int top = cat_int_array_wo_get(priv->zones, zone_idx++);
			int bottom = cat_int_array_wo_get(priv->zones, zone_idx++);
			if (ymouse>=top && ymouse<=bottom) {
				*top_out = top;
				*bottom_out = bottom;
				return TRUE;
			}
		}
	}
	return FALSE;
}

static gboolean l_button_press_event_cb(GtkWidget *gwidget, GdkEventButton *eev, gpointer data) {
	DraTagOverviewBar *instance = DRA_TAG_OVERVIEW_BAR(data);
	DraTagOverviewBarPrivate *priv = dra_tag_overview_bar_get_instance_private(instance);
	gint ymouse = eev->y;
	int top=0;
	int bottom;
	if (l_get_zone(instance, ymouse, &top, &bottom)) {
		int mid = (top+bottom);
		long long row = (long long) mid*priv->total_line_count/(priv->zones_view_height*ZONE_SCALE*2);
		GtkWidget *epw = gtk_widget_get_parent(gwidget);
		while(epw) {
			if (DRA_IS_EDITOR_PANEL(epw)) {
				break;
			}
			epw = gtk_widget_get_parent(epw);
		}
		cat_log_debug("show line=%d", row);
		if (epw) {
			DraEditor *editor = dra_editor_panel_get_editor((DraEditorPanel *) epw);

			ChaDocumentView *document_view = cha_editor_get_document_view((ChaEditor *) editor);
			ChaDocument *document = cha_editor_get_document((ChaEditor *) editor);
			ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);
			long long total_line_count = cha_revision_wo_count_lines(e_revision);
			if (row>=total_line_count) {
				row = (int) (total_line_count-1);
			} else if (row<0) {
				row = 0;
			}
			ChaLineLocationWo *nll = cha_revision_wo_calculate_line_location(e_revision, row);
			if (nll) {
				ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);
				cha_cursor_wo_set_line_location(e_cursor, nll);
				cat_unref(nll);
				cha_cursor_wo_set_x_cursor_bytes(e_cursor, 0);
			}
			cha_document_anchor_document(document);
			if (nll) {
				cha_document_view_move_view_to_focus(document_view, TRUE);
				cha_document_view_invalidate_lines(document_view);
			}
		}
	}

	return TRUE;
}

static gboolean l_motion_event_cb(GtkWidget *gwidget, GdkEventMotion *eev, gpointer data) {
	DraTagOverviewBar *instance = DRA_TAG_OVERVIEW_BAR(data);
	gint ymouse = eev->y;
	int top=0;
	int bottom;
	GdkWindow *window = gtk_widget_get_window(gwidget);
	if (l_get_zone(instance, ymouse, &top, &bottom)) {
		gdk_window_set_cursor(window, gdk_cursor_new_for_display(gdk_window_get_display(window), GDK_HAND1));
	} else {
		gdk_window_set_cursor(window, NULL);
	}

	return TRUE;
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
