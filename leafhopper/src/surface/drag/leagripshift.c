/*
   File:    leagripshift.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 18, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#include "leagripshift.h"
#include "leaigrip.h"
#include "../leaidrawable.h"
#include "../../leaframe.h"
#include <gtk/gtk.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaGripShift"
#include <logging/catlog.h>

struct _LeaGripShiftPrivate {
	LeaSurfaceBox *surface_box;
	int entry_index;
	int delta_left;
	int delta_top;
	LeaFrame *frame;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_grip_iface_init(LeaIGripInterface *iface);
static void l_drawable_iface_init(LeaIDrawableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaGripShift, lea_grip_shift, G_TYPE_OBJECT,
		G_ADD_PRIVATE(LeaGripShift)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IGRIP, l_grip_iface_init)
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IDRAWABLE, l_drawable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_grip_shift_class_init(LeaGripShiftClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_grip_shift_init(LeaGripShift *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaGripShift *instance = LEA_GRIP_SHIFT(object);
	LeaGripShiftPrivate *priv = lea_grip_shift_get_instance_private(instance);
	cat_unref_ptr(priv->surface_box);
	G_OBJECT_CLASS(lea_grip_shift_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_grip_shift_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaGripShift *lea_grip_shift_new(LeaSurfaceBox *surface_box, int entry_index, int delta_left, int delta_top) {
	LeaGripShift *result = g_object_new(LEA_TYPE_GRIP_SHIFT, NULL);
	cat_ref_anounce(result);
	LeaGripShiftPrivate *priv = lea_grip_shift_get_instance_private(result);
	priv->surface_box = cat_ref_ptr(surface_box);
	priv->entry_index = entry_index-1;	// TODO: why -1 ???
	priv->delta_left = delta_left;
	priv->delta_top = delta_top;
	priv->frame = NULL;
	return result;
}

/********************* start LeaIGrip implementation *********************/

static void l_drawable_draw(LeaIDrawable *self, cairo_t *cairo) {
	LeaGripShiftPrivate *priv = lea_grip_shift_get_instance_private(LEA_GRIP_SHIFT(self));
	gboolean is_horizontal = lea_surface_box_get_orientation(priv->surface_box) == LEA_SB_HORIZONTAL;
	GtkAllocation box_allocation = lea_iregion_get_allocation(LEA_IREGION(priv->surface_box));
	int x = box_allocation.x;
	int y = box_allocation.y;
	int width = box_allocation.width;
	int height = box_allocation.height;
	cat_log_debug("priv->entry_index: %d", priv->entry_index);
	LeaIRegion *child_region = lea_surface_box_get_region_at(priv->surface_box, priv->entry_index+1);
	GtkAllocation child_allocation = lea_iregion_get_allocation(child_region);


	cairo_set_source_rgb(cairo,0,0,0);
	if (is_horizontal) {
		cairo_set_line_width(cairo, 1);
		double cx = (double) child_allocation.x - 0.5;
		cat_log_debug("draw shift hor: %d,%d - %d,%d", cx,y, cx, (y+height));
		cairo_move_to(cairo, cx, y);
		cairo_line_to(cairo, cx, y+height);
		cairo_stroke(cairo);
	} else {
		cairo_set_line_width(cairo, 1);
		double cy = (double) child_allocation.y - 0.5;
		cat_log_debug("draw shift ver: %d,%d - %d,%d", x,cy, (x+width), cy);
		cairo_move_to(cairo, x, cy);
		cairo_line_to(cairo, x+width, cy);
		cairo_stroke(cairo);
	}
}


static cairo_region_t *l_drawable_create_region(LeaIDrawable *self) {
	LeaGripShiftPrivate *priv = lea_grip_shift_get_instance_private(LEA_GRIP_SHIFT(self));
	gboolean is_horizontal = lea_surface_box_get_orientation(priv->surface_box) == LEA_SB_HORIZONTAL;
	GtkAllocation box_allocation = lea_iregion_get_allocation(LEA_IREGION(priv->surface_box));
	int x = box_allocation.x;
	int y = box_allocation.y;
	int width = box_allocation.width;
	int height = box_allocation.height;
	cat_log_debug("priv->entry_index: %d", priv->entry_index);
	LeaIRegion *child_region = lea_surface_box_get_region_at(priv->surface_box, priv->entry_index+1);
	GtkAllocation child_allocation = lea_iregion_get_allocation(child_region);

	cairo_region_t *result = NULL;

	if (is_horizontal) {
		double cx = (double) child_allocation.x - 0.5;
		double top = y;
		double bottom = y+height;
		if (top>bottom) {
			double t = top;
			top = bottom;
			bottom = t;
		}
		cairo_rectangle_int_t rect;
		rect.x = cx-1;
		rect.y = top-1;
		rect.width = 2;
		rect.height = 1+bottom-top;
		if (rect.x<0) {
			rect.x = 0;
		}
		if (rect.y<0) {
			rect.y = 0;
		}
		cat_log_debug("rect[%d,%d,%d,%d]", rect.x, rect.y, rect.width, rect.height);
		result = cairo_region_create_rectangle(&rect);
	} else {
		double cy = (double) child_allocation.y - 0.5;
		double left = x;
		double right = x+width;
		if (left>right) {
			double t = left;
			left = right;
			right = t;
		}
		cairo_rectangle_int_t rect;
		rect.x = left-1;
		rect.y = cy-1;
		rect.width = 1+right-left;
		rect.height = 2;
		if (rect.x<0) {
			rect.x = 0;
		}
		if (rect.y<0) {
			rect.y = 0;
		}
		cat_log_debug("rect[%d,%d,%d,%d]", rect.x, rect.y, rect.width, rect.height);
		result = cairo_region_create_rectangle(&rect);
	}
	return result;
}


static void l_drawable_iface_init(LeaIDrawableInterface *iface) {
	iface->draw = l_drawable_draw;
	iface->createRegion = l_drawable_create_region;
}

/********************* end LeaIGrip implementation *********************/


/********************* start LeaIGrip implementation *********************/

static LeaSimilarity l_grip_is_identical(LeaIGrip *self, LeaIGrip *other) {
	if (LEA_IS_GRIP_SHIFT(other)) {
		LeaGripShiftPrivate *priv_a = lea_grip_shift_get_instance_private((LeaGripShift *) self);
		LeaGripShiftPrivate *priv_b = lea_grip_shift_get_instance_private((LeaGripShift *) other);
		if ((priv_a->entry_index == priv_b->entry_index)
				&& (priv_a->surface_box == priv_b->surface_box)) {

			if ((priv_a->delta_left == priv_b->delta_left)
					&& (priv_a->delta_top == priv_b->delta_top)) {
				return LEA_SIMILARITY_COMPLETE;
			}
			return LEA_SIMILARITY_SAME_ITEM;
		}
	}
	return LEA_SIMILARITY_NONE;
}

static void l_grip_add_notify(LeaIGrip *self, LeaFrame *frame) {
	LeaGripShiftPrivate *priv = lea_grip_shift_get_instance_private((LeaGripShift *) self);
	priv->frame = frame;
	GdkWindow *frame_window = gtk_widget_get_window(GTK_WIDGET(frame));

	if (lea_surface_box_get_orientation(priv->surface_box)==LEA_SB_VERTICAL) {
		gdk_window_set_cursor(frame_window, gdk_cursor_new_for_display(gdk_window_get_display(frame_window), GDK_SB_V_DOUBLE_ARROW));
	} else {
		gdk_window_set_cursor(frame_window, gdk_cursor_new_for_display(gdk_window_get_display(frame_window), GDK_SB_H_DOUBLE_ARROW));
	}
}

static void l_grip_remove_notify(LeaIGrip *self) {
	LeaGripShiftPrivate *priv = lea_grip_shift_get_instance_private((LeaGripShift *) self);
	if (priv->frame) {
		GdkWindow *frame_window = gtk_widget_get_window(GTK_WIDGET(priv->frame));
		gdk_window_set_cursor(frame_window, NULL);
	}
	priv->frame = NULL;
}

#define LEA_MIN 40

static void l_grip_motion_event(LeaIGrip *self, GdkEventMotion *event) {
	LeaGripShiftPrivate *priv = lea_grip_shift_get_instance_private((LeaGripShift *) self);
	cat_log_debug("priv->frame=%p, x=%f, y=%f", priv->frame, event->x, event->y);
	if (priv->frame) {
		int xpos = (int) event->x;
		int ypos = (int) event->y;

		gboolean is_horizontal = lea_surface_box_get_orientation(priv->surface_box) == LEA_SB_HORIZONTAL;

		int new_abs_pos = is_horizontal ? priv->delta_left+xpos : priv->delta_top+ypos;
	//	cat_log_debug("x=%d : new_abs_pos=%d : locator->delta_x=%d", x, new_abs_pos, locator->delta_x);

		int pre_weight = 0;
		int post_weight = 0;
		int min_pre_phy = 0;
		int min_post_phy = 0;

		int entry_count = lea_surface_box_entry_count(priv->surface_box);
		int entry_index;
		int *cached_weights = g_malloc(sizeof(int) * entry_count);
		for(entry_index=0; entry_index<entry_count; entry_index++) {
			int child_weight = lea_surface_box_get_weight_at(priv->surface_box, entry_index);
			cat_log_debug("child_weight[%d]=%d", entry_index, child_weight);

			cached_weights[entry_index] = child_weight;
		    if (entry_index<=priv->entry_index) {
		    	pre_weight += child_weight;
		    	min_pre_phy += LEA_MIN;
		    } else {
		    	post_weight += child_weight;
		    	min_post_phy += LEA_MIN;
		    }
		}

		int64_t total_weight = pre_weight+post_weight;

		int64_t depth, mouse_phy;
		GtkAllocation box_allocation = lea_iregion_get_allocation((LeaIRegion *) priv->surface_box);
		if (is_horizontal) {
			depth = box_allocation.width;
			mouse_phy = new_abs_pos-box_allocation.x;
		} else {
			depth = box_allocation.height;
			mouse_phy = new_abs_pos-box_allocation.y;
		}

		cat_log_debug("depth=%d, mouse_phy=%d", depth, mouse_phy);

		// clip physical mouse position
		if (mouse_phy > depth-min_post_phy) {
			mouse_phy = depth-min_post_phy;
		}
		if (mouse_phy<min_pre_phy) {
			mouse_phy = min_pre_phy;
		}

		total_weight = LEA_SB_PREFERED_ACCUM_WEIGHT;

		// calculate new weights
		int64_t new_pre_weight = (total_weight*mouse_phy)/depth;
		int64_t new_post_weight = (total_weight*(depth-mouse_phy))/depth;

		cat_log_debug("new_pre_weight=%d, new_post_weight=%d, pre_weight=%d, post_weight=%d", new_pre_weight, new_post_weight, pre_weight, post_weight);



		for(entry_index=0; entry_index<entry_count; entry_index++) {
			int new_child_weight=0;
			int64_t child_weight = cached_weights[entry_index];
			if (entry_index<=priv->entry_index) {
				new_child_weight = (child_weight*new_pre_weight)/pre_weight;
			} else {
				new_child_weight = (child_weight*new_post_weight)/post_weight;
			}
			cat_log_debug("[%d]=%d, child_weight=%d", entry_index, new_child_weight, child_weight);
			lea_surface_box_set_weight_at(priv->surface_box, (int) new_child_weight, entry_index);
		}

		cat_free_ptr(cached_weights);
		lea_iregion_size_allocate(LEA_IREGION(priv->surface_box), &box_allocation);
	}
}

static void l_grip_release_event(LeaIGrip *self, GdkEventButton *event) {
}

static void l_grip_iface_init(LeaIGripInterface *iface) {
	iface->isIdentical = l_grip_is_identical;
	iface->addNotify = l_grip_add_notify;
	iface->removeNotify = l_grip_remove_notify;
	iface->motionEvent = l_grip_motion_event;
	iface->releaseEvent = l_grip_release_event;
}

/********************* end LeaIGrip implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaGripShift *instance = LEA_GRIP_SHIFT(self);
	LeaGripShiftPrivate *priv = lea_grip_shift_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p surface_box=%p, index=%d, d-left=%d, d-top=%d]", iname, self, priv->surface_box, priv->entry_index, priv->delta_left, priv->delta_top);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
