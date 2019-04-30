/*
   File:    leafullmenuitem.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    May 19, 2010
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


#include "leafullmenuitem.h"
#include "gtkmenuitemprivate.h"
#include <caterpillar.h>

#include "logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaFullMenuItem"
#include "logging/catlog.h"


G_DEFINE_TYPE(LeaFullMenuItem, lea_full_menu_item, GTK_TYPE_MENU_ITEM) // @suppress("Unused static function")

static void _dispose(GObject *object);
static void _menu_item_toggle_size_request(GtkMenuItem *menu_item, gint *requisition);
static void _container_forall(GtkContainer *container, gboolean include_internals, GtkCallback callback, gpointer callback_data);

static void _widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation);

static gboolean _widget_draw(GtkWidget *widget, cairo_t *cairo);

static void l_widget_get_preferred_height(GtkWidget *widget, gint *minimum_size, gint *natural_size);
static void l_widget_get_preferred_height_for_width(GtkWidget *widget, gint for_size, gint *minimum_size, gint *natural_size);
static void l_widget_get_preferred_width(GtkWidget *widget, gint *minimum_size, gint *natural_size);


static GtkMenuItemClass *parent_class = NULL;

static void lea_full_menu_item_class_init(LeaFullMenuItemClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;

	GtkContainerClass *container_class = GTK_CONTAINER_CLASS(clazz);
	container_class->forall = _container_forall;

	GtkMenuItemClass *menu_class = GTK_MENU_ITEM_CLASS(clazz);
	menu_class->toggle_size_request = _menu_item_toggle_size_request;



	GtkWidgetClass *widget_class = GTK_WIDGET_CLASS(clazz);
	widget_class->size_allocate = _widget_size_allocate;

	widget_class->draw = _widget_draw;

	widget_class->get_preferred_width = l_widget_get_preferred_width;
	widget_class->get_preferred_height = l_widget_get_preferred_height;
	widget_class->get_preferred_height_for_width = l_widget_get_preferred_height_for_width;

}

static void lea_full_menu_item_init(LeaFullMenuItem *node) {
}

static void _dispose(GObject *object) {
	LeaFullMenuItem *instance = LEA_FULL_MENU_ITEM(object);
	cat_unref_ptr(instance->key_sequence);
	if (instance->image) {
		instance->image = NULL;
	}

	if (instance->label_widget) {
		gtk_container_remove(GTK_CONTAINER(instance), GTK_WIDGET(instance->label_widget));
		instance->label_widget = NULL;
	}

	cat_unref_ptr(instance->a_stock_id);
	cat_log_debug("dispose wdget=%p", object);
	G_OBJECT_CLASS(parent_class)->dispose(object);

}

LeaFullMenuItem *lea_full_menu_item_new() {
	LeaFullMenuItem *result = g_object_new(LEA_TYPE_FULL_MENU_ITEM, NULL);
	cat_ref_anounce(result);
	lea_full_menu_item_construct(result);
	return result;
}

void lea_full_menu_item_construct(LeaFullMenuItem *item) {
	item->key_sequence = NULL;
	item->key_pixel_width = 0;
	item->label_widget = gtk_label_new_with_mnemonic("New Item");
	item->image = NULL;
	item->a_stock_id = NULL;

	GValue gvalue = { 0.f, };
	g_value_init( &gvalue, G_TYPE_FLOAT );
	g_value_set_float( &gvalue, 0.0f );
	g_object_set_property( G_OBJECT(item->label_widget), "xalign", &gvalue );
	g_value_unset( &gvalue );

	g_value_init( &gvalue, G_TYPE_FLOAT );
	g_value_set_float( &gvalue, 0.5f );
	g_object_set_property( G_OBJECT(item->label_widget), "yalign", &gvalue );
	g_value_unset( &gvalue );

	gtk_widget_set_visible(item->label_widget, TRUE);
	gtk_container_add(GTK_CONTAINER(item), item->label_widget);
	cat_log_debug("new menu widget=%o", item);

}



static void _update_image(LeaFullMenuItem *item) {

	if (item->image) {
		gtk_widget_unparent(GTK_WIDGET(item->image));
//		gtk_widget_destroy(GTK_WIDGET(item->image));
		item->image = NULL;
	}

	if (item->toggable) {
		if (item->is_toggled) {
//			GtkWidget *image = gtk_image_new();
//			item->image = GTK_IMAGE(image);
//
//			cairo_surface_t *surface = cairo_image_surface_create(CAIRO_FORMAT_ARGB32, 120, 120);
//			cairo_t *cr = cairo_create(surface);
//
//			cairo_set_source_rgb(cr, 0,0,0);
//			cairo_move_to(cr, 20,50);
//			cairo_line_to(cr, 50,80);
//			cairo_set_line_width(cr, 4);
//			cairo_stroke(cr);
//
//			gtk_image_set_from_surface(GTK_IMAGE(image), surface);
//			gtk_widget_set_parent(image, GTK_WIDGET(item));
//			gtk_widget_show(image);
		}

	} else if (item->a_stock_id!=NULL) {
		GtkWidget *image = gtk_image_new();
		const char *stock_id = cat_string_wo_getchars(item->a_stock_id);
		cat_log_debug("setting stock_id=%s", stock_id);
		gtk_image_set_from_icon_name(GTK_IMAGE(image), stock_id, GTK_ICON_SIZE_MENU);
		item->image = GTK_IMAGE(image);
		gtk_widget_set_parent(image, GTK_WIDGET(item));
		gtk_widget_show(image);
	}

}

static void _container_forall(GtkContainer *container, gboolean include_internals, GtkCallback callback, gpointer callback_data) {
	LeaFullMenuItem *this = LEA_FULL_MENU_ITEM(container);

	(*GTK_CONTAINER_CLASS(parent_class)->forall) (container, include_internals, callback, callback_data);

	if (this->image) {
	    (* callback) (GTK_WIDGET(this->image), callback_data);
	}
}


static void _menu_item_toggle_size_request(GtkMenuItem *menu_item, gint *requisition) {
	LeaFullMenuItem *this = LEA_FULL_MENU_ITEM(menu_item);
	*requisition = 0;

	if (this->toggable) {
		guint toggle_spacing;
		gtk_widget_style_get(GTK_WIDGET(menu_item), "toggle_spacing", &toggle_spacing, NULL);
		PangoLayout *pango_layout = gtk_widget_create_pango_layout(GTK_WIDGET(menu_item), "✓");
		PangoRectangle log_rect;
		pango_layout_get_extents(pango_layout, NULL, &log_rect);
		int check_mark_width = log_rect.width/PANGO_SCALE;
		cat_unref_ptr(pango_layout);
		*requisition = check_mark_width + toggle_spacing;

	} else if (this->image) {
		int image_width;

		gtk_widget_get_preferred_width(GTK_WIDGET(this->image), NULL, &image_width);

		if (image_width > 0) {
			guint toggle_spacing;
			gtk_widget_style_get(GTK_WIDGET(menu_item), "toggle_spacing", &toggle_spacing, NULL);

			*requisition = image_width + toggle_spacing;
		}
	}
}


static void _widget_size_allocate(GtkWidget *widget, GtkAllocation *allocation) {

	cat_log_debug("widget=%p, allocation=[%d, %d, %d, %d]", widget, allocation->x, allocation->y, allocation->width, allocation->height);

	LeaFullMenuItem *this = LEA_FULL_MENU_ITEM(widget);
	allocation->height=20;

	if (GTK_WIDGET_CLASS(parent_class)->size_allocate) {
		GTK_WIDGET_CLASS(parent_class)->size_allocate(widget, allocation);
	}


	if (this->image && gtk_widget_get_visible(GTK_WIDGET(this->image))) {
		// Part below is copied somewhere from the GTK library
		// since GTK is GPL and this class is GPL there should be no problem :)
		gint x, y, offset;
		GtkRequisition child_requisition;
		GtkAllocation child_allocation;
		guint horizontal_padding, toggle_spacing;

		gtk_widget_style_get (widget,
	                            "horizontal_padding", &horizontal_padding,
	                            "toggle_spacing", &toggle_spacing,
	                            NULL);


		GtkBorder padding;
		GtkStyleContext *context = gtk_widget_get_style_context(widget);
		GtkStateFlags state = gtk_widget_get_state_flags(widget);
		gtk_style_context_get_padding(context, state, &padding);


		gtk_widget_get_preferred_size(GTK_WIDGET(this->image), NULL, &child_requisition);

		offset = gtk_container_get_border_width(GTK_CONTAINER(widget));


		GtkMenuItem *menu_item = GTK_MENU_ITEM(this);
		int toggle_size = 0;
		gtk_menu_item_toggle_size_request(menu_item, &toggle_size);
		x = offset + padding.left + horizontal_padding + (toggle_size - toggle_spacing - child_requisition.width) / 2;
		cat_log_debug("toggle_size=%d, x=$d", toggle_size, x);

		GtkAllocation allocation;
		gtk_widget_get_allocation(widget, &allocation);

		y = (allocation.height - child_requisition.height) / 2;

		child_allocation.width = child_requisition.width;
		child_allocation.height = child_requisition.height;
		child_allocation.x = allocation.x + MAX (x, 0);
		child_allocation.y = allocation.y + MAX (y, 0);

		cat_log_debug("child_allocation=[%d, %d, %d, %d]", child_allocation.x, child_allocation.y, child_allocation.width, child_allocation.height);

		gtk_widget_size_allocate(GTK_WIDGET(this->image), &child_allocation);
	}
}



static gboolean _widget_draw(GtkWidget *widget, cairo_t *cairo) {
	LeaFullMenuItem *this = LEA_FULL_MENU_ITEM(widget);
	if (gtk_widget_is_drawable(widget)) {
		if (GTK_WIDGET_CLASS(parent_class)->draw) {
			GTK_WIDGET_CLASS(parent_class)->draw(widget, cairo);
		}
		GtkAllocation allocation;
		gtk_widget_get_allocation(widget, &allocation);

		GtkWidget *child = gtk_bin_get_child(GTK_BIN(widget));
		if (child) {
			GtkLabel *label = GTK_LABEL(child);
			int label_y;
			gtk_label_get_layout_offsets(label, NULL, &label_y);


			if (this->key_sequence) {
				CatStringWo *a_accel = lea_key_sequence_to_string(this->key_sequence);
				PangoLayout *pango_layout = gtk_widget_create_pango_layout(widget, cat_string_wo_getchars(a_accel));
				int y = label_y - allocation.y;
				int x = allocation.x + allocation.width - this->key_pixel_width;
				cat_log_debug("x=%d, y=%d", x, y);
				gtk_render_layout(gtk_widget_get_style_context(child), cairo, x, y, pango_layout);

				cat_unref(pango_layout);
				cat_unref(a_accel);
			}

			if (this->is_toggled && this->toggable) {
				PangoLayout *pango_layout = gtk_widget_create_pango_layout(widget, "✓");
				int y = label_y - allocation.y;
				int x = allocation.x;
				cat_log_debug("x=%d, y=%d", x, y);
				gtk_render_layout(gtk_widget_get_style_context(child), cairo, x, y, pango_layout);
				cat_unref(pango_layout);
			}
		}
	}
	return FALSE;
}


void lea_full_menu_item_set_key_sequence(LeaFullMenuItem *item, LeaKeySequence *key_sequence) {
	cat_ref_sink_swap(item->key_sequence, key_sequence);
}

void lea_full_menu_item_set_stock_id(LeaFullMenuItem *item, CatStringWo *a_stock_id) {
	if (item->a_stock_id == a_stock_id) {
		return;
	}
	cat_ref_swap(item->a_stock_id, a_stock_id);
	_update_image(item);
}


void lea_full_menu_item_set_toggable(LeaFullMenuItem *item, gboolean toggable) {
	item->toggable = toggable;
	_update_image(item);
}

gboolean lea_full_menu_item_is_toggled(LeaFullMenuItem *item) {
	return item->is_toggled;
}

void lea_full_menu_item_set_toggled(LeaFullMenuItem *item, gboolean toggled) {
	item->is_toggled = toggled;
	_update_image(item);
}



static void l_get_accel_dimensions(LeaFullMenuItem *full_menu_item, GtkRequisition *requisition) {
	GtkWidget *widget = GTK_WIDGET(full_menu_item);
	requisition->width = 0;
	requisition->height = 0;
	full_menu_item->key_pixel_width = 0;

	if (full_menu_item->key_sequence) {
		CatStringWo *a_accel = lea_key_sequence_to_string(full_menu_item->key_sequence);
		PangoLayout *pango_layout = gtk_widget_create_pango_layout(widget, cat_string_wo_getchars(a_accel));
		int width = 0;
		int height;
		pango_layout_get_pixel_size(pango_layout, &width, &height);
		cat_unref(pango_layout);
		cat_unref_ptr(a_accel);

		width += 10;

		requisition->width = width;
		requisition->height = height;

		full_menu_item->key_pixel_width = width;
	}

}



static void l_widget_get_preferred_width(GtkWidget *widget, gint *minimum_size, gint *natural_size) {
	LeaFullMenuItem *full_menu_item = LEA_FULL_MENU_ITEM(widget);

	guint horizontal_padding;
	GtkPackDirection pack_dir;
	GtkPackDirection child_pack_dir;
	gint  min_width, nat_width;


	GtkBorder padding;

	min_width = nat_width = 0;

	gtk_widget_style_get(widget, "horizontal-padding", &horizontal_padding, NULL);

	GtkBin *bin = GTK_BIN(widget);
	GtkWidget *parent = gtk_widget_get_parent(widget);

	if (GTK_IS_MENU_BAR(parent)) {
		pack_dir = gtk_menu_bar_get_pack_direction(GTK_MENU_BAR (parent));
		child_pack_dir = gtk_menu_bar_get_child_pack_direction(GTK_MENU_BAR(parent));
	} else {
		pack_dir = GTK_PACK_DIRECTION_LTR;
		child_pack_dir = GTK_PACK_DIRECTION_LTR;
	}

	guint border_width = gtk_container_get_border_width(GTK_CONTAINER(widget));

	GtkStyleContext *context = gtk_widget_get_style_context(widget);
	GtkStateFlags state = gtk_widget_get_state_flags(widget);
	gtk_style_context_get_padding(context, state, &padding);


	min_width = (border_width * 2) + padding.left + padding.right;

	cat_log_debug("%p: border_width=%d, horizontal_padding=%d, padding.left=%d, padding.right=%d", widget, border_width, horizontal_padding, padding.left, padding.right);


	if ((pack_dir == GTK_PACK_DIRECTION_LTR || pack_dir == GTK_PACK_DIRECTION_RTL) &&
				(child_pack_dir == GTK_PACK_DIRECTION_LTR || child_pack_dir == GTK_PACK_DIRECTION_RTL)) {
		min_width += 2 * horizontal_padding;
	}

	nat_width = min_width;

	GtkWidget *child = gtk_bin_get_child(bin);

	if (child != NULL && gtk_widget_get_visible(child)) {
		gint child_min, child_nat;

		gtk_widget_get_preferred_width(child, &child_min, &child_nat);

		min_width += child_min;
		nat_width += child_nat;
	}

	GtkRequisition accel_req;
	l_get_accel_dimensions(full_menu_item, &accel_req);
	full_menu_item->key_pixel_width = accel_req.width;
	GTK_MENU_ITEM(widget)->priv->accelerator_width = accel_req.width;


	cat_log_debug("%p: min_width=%d, nat_width=%d, accel_req.width=%d", widget, min_width, nat_width, accel_req.width);


	if (minimum_size) {
		*minimum_size = min_width;
	}

	if (natural_size) {
		*natural_size = nat_width;
	}
}




static void l_widget_get_preferred_height(GtkWidget *widget, gint *minimum_size, gint *natural_size) {
	LeaFullMenuItem *full_menu_item = LEA_FULL_MENU_ITEM(widget);
	GtkBorder padding;
	guint vertical_padding;
	GtkPackDirection pack_dir;
	GtkPackDirection child_pack_dir;
	gint min_height, nat_height;

	min_height = nat_height = 0;

	GtkStyleContext *context = gtk_widget_get_style_context(widget);
	GtkStateFlags state = gtk_widget_get_state_flags(widget);
	gtk_style_context_get_padding(context, state, &padding);

	gtk_widget_style_get(widget, "horizontal-padding", &vertical_padding, NULL);

	GtkBin *bin = GTK_BIN (widget);
	GtkWidget *parent = gtk_widget_get_parent(widget);

	if (GTK_IS_MENU_BAR (parent)) {
		pack_dir = gtk_menu_bar_get_pack_direction(GTK_MENU_BAR (parent));
		child_pack_dir = gtk_menu_bar_get_child_pack_direction(GTK_MENU_BAR (parent));
	} else {
		pack_dir = GTK_PACK_DIRECTION_LTR;
		child_pack_dir = GTK_PACK_DIRECTION_LTR;
	}

	guint border_width = gtk_container_get_border_width(GTK_CONTAINER (widget));
	min_height = (border_width * 2) + padding.top + padding.bottom;

	cat_log_debug("%p: border_width=%d, min_height=%d, border.top=%d", widget, border_width, min_height);

	if ((pack_dir == GTK_PACK_DIRECTION_TTB || pack_dir == GTK_PACK_DIRECTION_BTT) && (child_pack_dir == GTK_PACK_DIRECTION_TTB
	                                || child_pack_dir == GTK_PACK_DIRECTION_BTT)) {
		min_height += 2 * vertical_padding;
	}

	nat_height = min_height;

	GtkWidget *child = gtk_bin_get_child(bin);

	if (child != NULL && gtk_widget_get_visible(child)) {
		gint child_min, child_nat;

		gtk_widget_get_preferred_height(child, &child_min, &child_nat);

		min_height += child_min;
		nat_height += child_nat;

	} else  {
		gboolean wide_separators;
		gint separator_height;

		gtk_widget_style_get(widget, "wide-separators", &wide_separators, "separator-height", &separator_height, NULL);

		if (wide_separators) {
			min_height += separator_height + padding.top;
		} else {
			min_height += padding.top + padding.bottom;
		}

		nat_height = min_height;
	}




	GtkRequisition accel_req;
	l_get_accel_dimensions(full_menu_item, &accel_req);
	full_menu_item->key_pixel_width = accel_req.width;
	GTK_MENU_ITEM(widget)->priv->accelerator_width = accel_req.width;

	cat_log_debug("%p: min_height=%d, nat_height=%d, accel_req.width=%d", widget, min_height, nat_height, accel_req.width);

	if (minimum_size) {
		*minimum_size = min_height;
	}

	if (natural_size) {
		*natural_size = nat_height;
	}
}

static void l_widget_get_preferred_height_for_width(GtkWidget *widget, gint for_size, gint *minimum_size, gint *natural_size) {
	GtkBin *bin;
	GtkStyleContext *context;
	GtkStateFlags state;
	GtkBorder padding;
	GtkWidget *child;
	GtkWidget *parent;
	guint horizontal_padding;
	guint border_width;
	GtkPackDirection pack_dir;
	GtkPackDirection child_pack_dir;
	gint min_height, nat_height;
	gint avail_size;

	cat_log_debug("%p: for_size=%d", widget, for_size);

	min_height = nat_height = 0;

	context = gtk_widget_get_style_context(widget);
	state = gtk_widget_get_state_flags(widget);
	gtk_style_context_get_padding(context, state, &padding);

	gtk_widget_style_get(widget, "horizontal-padding", &horizontal_padding, NULL);

	bin = GTK_BIN (widget);
	parent = gtk_widget_get_parent(widget);

	if (GTK_IS_MENU_BAR (parent)) {
		pack_dir = gtk_menu_bar_get_pack_direction(GTK_MENU_BAR (parent));
		child_pack_dir = gtk_menu_bar_get_child_pack_direction(GTK_MENU_BAR (parent));
	} else {
		pack_dir = GTK_PACK_DIRECTION_LTR;
		child_pack_dir = GTK_PACK_DIRECTION_LTR;
	}

	border_width = gtk_container_get_border_width(GTK_CONTAINER (widget));
	min_height = (border_width * 2) + padding.top + padding.bottom;

	avail_size = for_size;
	avail_size -= (border_width * 2) + padding.left + padding.right;

	cat_log_debug("%p: border_width=%d, min_height=%d, avail_size=%d", widget, border_width, min_height, avail_size);


	if ((pack_dir == GTK_PACK_DIRECTION_TTB || pack_dir == GTK_PACK_DIRECTION_BTT) && (child_pack_dir == GTK_PACK_DIRECTION_TTB
	                                || child_pack_dir == GTK_PACK_DIRECTION_BTT)) {
		min_height += 2 * horizontal_padding;
	}

	if ((pack_dir == GTK_PACK_DIRECTION_LTR || pack_dir == GTK_PACK_DIRECTION_RTL) && (child_pack_dir == GTK_PACK_DIRECTION_LTR
	                                || child_pack_dir == GTK_PACK_DIRECTION_RTL)) {
		avail_size -= 2 * horizontal_padding;
	}

	nat_height = min_height;

	child = gtk_bin_get_child(bin);

	if (child != NULL && gtk_widget_get_visible(child)) {
		gint child_min, child_nat;

		gtk_widget_get_preferred_height_for_width(child, avail_size, &child_min, &child_nat);

		min_height += child_min;
		nat_height += child_nat;

	} else  {
		gboolean wide_separators;
		gint separator_height;

		gtk_widget_style_get(widget, "wide-separators", &wide_separators, "separator-height", &separator_height, NULL);

		if (wide_separators) {
			min_height += separator_height + padding.top;
		} else {
			min_height += padding.top + padding.bottom;
		}

		nat_height = min_height;
	}


	cat_log_debug("%p: min_height=%d, nat_height=%d", widget, min_height, nat_height);

	if (minimum_size) {
		*minimum_size = min_height;
	}

	if (natural_size) {
		*natural_size = nat_height;
	}
}
