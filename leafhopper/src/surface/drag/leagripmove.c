/*
   File:    leagripmove.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 22, 2014
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

#include "leagripmove.h"
#include "leaigrip.h"
#include "leadroptotabproposal.h"
#include "leadropcardinalproposal.h"
#include "../leaidrawable.h"
#include "../leasurface.h"
#include "../../leaframe.h"
#include "leaidropproposal.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaGripMove"
#include <logging/catlog.h>

struct _LeaGripMovePrivate {
	LeaSurface *surface;
	int panel_idx;
	LeaIDropProposal *drop_proposal;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_grip_iface_init(LeaIGripInterface *iface);
static void l_drawable_iface_init(LeaIDrawableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaGripMove, lea_grip_move, G_TYPE_OBJECT,
		G_ADD_PRIVATE(LeaGripMove)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IGRIP, l_grip_iface_init)
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IDRAWABLE, l_drawable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_grip_move_class_init(LeaGripMoveClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_grip_move_init(LeaGripMove *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	LeaGripMove *instance = LEA_GRIP_MOVE(object);
//	LeaGripMovePrivate *priv = lea_grip_move_get_instance_private(instance);
	G_OBJECT_CLASS(lea_grip_move_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_grip_move_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaGripMove *lea_grip_move_new(LeaSurface *surface, int panel_idx) {
	LeaGripMove *result = g_object_new(LEA_TYPE_GRIP_MOVE, NULL);
	cat_ref_anounce(result);
	LeaGripMovePrivate *priv = lea_grip_move_get_instance_private(result);
	priv->surface = cat_ref_ptr(surface);
	priv->panel_idx = panel_idx;
	priv->drop_proposal = NULL;

	return result;
}






#define MAGNET_SIZE 30

static LeaCardinalPlacement l_calculate_cardinal_placement(LeaIRegion *region, int xpos, int ypos) {
	const GtkAllocation alloc = lea_iregion_get_allocation(region);
	int left = alloc.x;
	int right = alloc.width + left;
	int top = alloc.y;
	int bottom = alloc.height + top;

	gboolean inside_rect = TRUE;


	int magnet_hor = alloc.width/4;
	if (magnet_hor<MAGNET_SIZE) {
		magnet_hor = MAGNET_SIZE;
	}

	if (ypos>=top && ypos<=bottom) {
		int d = xpos-left;
		if (d>=0 && d<magnet_hor) {
			return LEA_CP_WEST;
		}
		d = right-xpos;
		if (d>=0 && d<magnet_hor) {
			return LEA_CP_EAST;
		}
	} else {
		inside_rect = FALSE;
	}

	int magnet_vert = alloc.height/4;
	if (magnet_vert<MAGNET_SIZE) {
		magnet_vert = MAGNET_SIZE;
	}

	if (xpos>=left && xpos<=right) {
		int d = ypos-top;
		if (d>=0 && d<magnet_vert) {
			return LEA_CP_NORTH;
		}
		d = bottom-ypos;
		if (d>=0 && d<magnet_vert) {
			return LEA_CP_SOUTH;
		}
	} else {
		inside_rect = FALSE;
	}

	return inside_rect ? LEA_CP_COVER : LEA_CP_NONE;
}

static gboolean l_inside_allocation(GtkAllocation *allocation, int xpos, int ypos) {
	return (xpos>=allocation->x) && (xpos<allocation->x+allocation->width)
			&& (ypos>=allocation->y) && (ypos<allocation->y+allocation->height);
}



static LeaDropToTabProposal *l_calculate_proposal_tab(LeaSurface *surface, int xpos, int ypos) {
	LeaSurfaceTabBar *tab_bar = lea_surface_get_tab_bar(surface);
	GtkAllocation tab_bar_alloc;
	gtk_widget_get_allocation(GTK_WIDGET(tab_bar), &tab_bar_alloc);
	cat_log_debug("tab_bar[%d, %d, %d, %d] - mouse[%d, %d]", tab_bar_alloc.x, tab_bar_alloc.y, tab_bar_alloc.width, tab_bar_alloc.height, xpos, ypos);
	if (!l_inside_allocation(&tab_bar_alloc, xpos, ypos)) {
		return NULL;
	}
	LeaDropToTabProposal *result = NULL;
	int tab_x = tab_bar_alloc.x;
	int tab_y = tab_bar_alloc.y;
	xpos -= tab_x;
	ypos -= tab_y;

	cat_log_debug("calc_tab:rel-xpos=%d, rel-ypos=%d", xpos, ypos);

	int panel_idx = lea_surface_tab_bar_get_panel_index_at(tab_bar, xpos, ypos);
	if (panel_idx>=-1) {
		result = lea_drop_to_tab_proposal_new(surface, panel_idx);
	}

	return result;
}


static LeaIDropProposal *l_calculate_best_proposal_for_node(LeaGripMove *grip_move, LeaSurfaceBox *surface_box, LeaIDropProposal *best_proposal, int xpos, int ypos, int level) {
	cat_log_debug("l_calculate_best_proposal_for_node:level=%d, surface_box=%p", level, surface_box);
	LeaGripMovePrivate *priv = lea_grip_move_get_instance_private(grip_move);
	int entry_count = lea_surface_box_entry_count(surface_box);
	int entry_index;
	for(entry_index=0; entry_index<entry_count; entry_index++) {
		LeaIRegion *child = lea_surface_box_get_region_at(surface_box, entry_index);
		cat_log_trace("level=%d, child=%o", level, child);
		if (LEA_IS_SURFACE(child)) {
			LeaDropToTabProposal *tab_proposal = l_calculate_proposal_tab(LEA_SURFACE(child), xpos, ypos);
			if (tab_proposal) {
				cat_unref(best_proposal);
				best_proposal = LEA_IDROP_PROPOSAL(tab_proposal);
				break;
			}
			LeaCardinalPlacement placement = l_calculate_cardinal_placement(child, xpos, ypos);
			cat_log_trace("placement=%d", placement);
			if (placement!=LEA_CP_NONE && (priv->surface!=(LeaSurface *) child || placement!=LEA_CP_COVER)) {
				cat_unref(best_proposal);
				best_proposal = LEA_IDROP_PROPOSAL(lea_drop_cardinal_proposal_new(child, level, placement));
				break;
			}
		}
		if (LEA_IS_SURFACE_BOX(child)) {
			best_proposal = l_calculate_best_proposal_for_node(grip_move, LEA_SURFACE_BOX(child), best_proposal, xpos, ypos, level+1);
		}
	}
	return best_proposal;
}


static void l_set_location(LeaGripMove *grip_move, int xpos, int ypos) {
	cat_log_debug("SET_LOCATION");
	LeaGripMovePrivate *priv = lea_grip_move_get_instance_private(grip_move);
	LeaFrame *frame = lea_surface_get_frame(priv->surface);
	LeaSurfaceHandler *surface_handler = lea_frame_get_surface_handler(frame);
	LeaSurfaceBox *root_box = lea_surface_handler_get_root(surface_handler);
	lea_surface_handler_dump(surface_handler);
	GtkWidget *top_widget = gtk_widget_get_toplevel(GTK_WIDGET(frame));
	GdkWindow *window = gtk_widget_get_window(top_widget);

	LeaIDropProposal *new_proposal = l_calculate_best_proposal_for_node(grip_move, root_box, NULL, xpos, ypos, 0);
	cat_log_debug("new_proposal=%p", new_proposal);
	if (new_proposal) {
		if (lea_idrop_proposal_equal(new_proposal, priv->drop_proposal)) {
			cat_unref(new_proposal);
			return;
		}
		LeaIDropProposal *old_proposal = priv->drop_proposal;
		priv->drop_proposal = new_proposal;

		if (old_proposal) {
			cat_log_detail("invalidating old: old_proposal=%p", old_proposal);
			lea_idrop_proposal_invalidate(old_proposal, window);
			cat_unref(old_proposal);
		}
		lea_idrop_proposal_invalidate(new_proposal, window);
	} else {
		if (priv->drop_proposal) {
			cat_log_detail("invalidating old: priv->drop_proposal=%p", priv->drop_proposal);
			lea_idrop_proposal_invalidate(priv->drop_proposal, window);
			cat_unref_ptr(priv->drop_proposal);
		}
		gdk_window_set_cursor(window, NULL);
	}
	cat_log_detail("_set_location:done");
}






static void l_drop_move_tab_to_leaf(LeaGripMove *grip_move, LeaSurfaceHandler *surface_handler, LeaDropCardinalProposal *proposal) {
	LeaGripMovePrivate *priv = lea_grip_move_get_instance_private(grip_move);
	LeaSurfaceTabModel *tab_model = lea_surface_get_tab_model(priv->surface);

	LeaSurfaceTab *tab = lea_surface_tab_model_get_tab_at(tab_model, priv->panel_idx);
	cat_ref_ptr(tab);
	lea_surface_tab_model_remove_tab_at(tab_model, priv->panel_idx);


	LeaCardinalPlacement placement = lea_drop_cardinal_proposal_get_placement(proposal);
	LeaIRegion *prop_region = lea_drop_cardinal_proposal_get_region(proposal);
	if (placement == LEA_CP_COVER) {
		if (LEA_IS_SURFACE(prop_region)) {
			LeaSurface *surface = (LeaSurface *) prop_region;
			LeaSurfaceTabModel *tab_model = lea_surface_get_tab_model(surface);
			int add_idx = lea_surface_tab_model_count(tab_model);
			lea_surface_tab_model_add_tab(tab_model, tab);
			lea_surface_tab_model_set_selected_tab(tab_model, add_idx);
		}
		cat_unref(tab);
		return;
	}


	LeaSurface *drop_surface = lea_surface_new();
	LeaSurfaceTabModel *drop_tab_model = lea_surface_get_tab_model(drop_surface);
	LeaFrame *frame = lea_surface_get_frame(priv->surface);
	gtk_container_add(GTK_CONTAINER(frame), GTK_WIDGET(drop_surface));
	lea_surface_tab_model_add_tab(drop_tab_model, tab);
	cat_log_debug("drop_surface=%p", drop_surface);

	gtk_widget_show_all(GTK_WIDGET(drop_surface));
	cat_unref(tab);


	LeaIRegion *prop_parent_region = lea_surface_handler_find_parent_region(surface_handler, prop_region);
	gboolean is_horizontal = placement==LEA_CP_EAST || placement==LEA_CP_WEST;
	if (LEA_IS_SURFACE_BOX(prop_parent_region)) {
		LeaSurfaceBox *grip_surface_box = LEA_SURFACE_BOX(prop_parent_region);
		int prop_region_index = lea_surface_box_index_of(grip_surface_box, prop_region);
		cat_log_debug("grip_surface_box=%p, prop_region=%p, prop_region_index=%d", grip_surface_box, prop_region, prop_region_index);
		LeaSurfaceBoxOrientation drop_box_orientation = lea_surface_box_get_orientation(grip_surface_box);
		if ((drop_box_orientation==LEA_SB_HORIZONTAL && is_horizontal)
				|| (drop_box_orientation==LEA_SB_VERTICAL && !is_horizontal)) {
			if (prop_region==(LeaIRegion *) priv->surface && prop_region_index>priv->panel_idx) {
				prop_region_index--;
			}
			/* insert in existing split box */
			if (placement==LEA_CP_EAST || placement==LEA_CP_SOUTH) {
				prop_region_index++;
			}
			cat_log_detail("prop_region=%p, node=%p, prop_region_index=%d", prop_region, drop_surface, prop_region_index);
			lea_surface_handler_dump(surface_handler);


			int add_weight = (int) (LEA_SB_PREFERED_ACCUM_WEIGHT/lea_surface_box_entry_count(grip_surface_box));

			lea_surface_box_add(grip_surface_box, LEA_IREGION(drop_surface), prop_region_index, add_weight);
			cat_log_detail("AFTER EUQAL SPLIT-BOX MOVE:add_weight=%d", add_weight);
			lea_surface_handler_dump(surface_handler);
		} else {
			cat_ref(prop_region);
			int prop_region_weight = lea_surface_box_get_weight_at(grip_surface_box, prop_region_index);
			lea_surface_box_remove_at(grip_surface_box, prop_region_index);
			LeaSurfaceBox *repl_surface_box = lea_surface_box_new(drop_box_orientation==LEA_SB_HORIZONTAL ? LEA_SB_VERTICAL : LEA_SB_HORIZONTAL);
			cat_log_detail("repl_surface_box=%o", repl_surface_box);

			lea_surface_box_add(grip_surface_box, LEA_IREGION(repl_surface_box), prop_region_index, prop_region_weight);
			if (placement==LEA_CP_EAST || placement==LEA_CP_SOUTH) {
				lea_surface_box_add(repl_surface_box, prop_region, 0, 1000);
				lea_surface_box_add(repl_surface_box, LEA_IREGION(drop_surface), 1, 1000);
			} else {
				lea_surface_box_add(repl_surface_box, LEA_IREGION(drop_surface), 0, 1000);
				lea_surface_box_add(repl_surface_box, prop_region, 1, 1000);
			}
			cat_unref(prop_region);
			cat_unref_ptr(repl_surface_box);
			cat_log_detail("AFTER INSERT SPLIT-BOX MOVE");
			lea_surface_handler_dump(surface_handler);
		}
	} else {
		cat_log_error("proposal nodes parent is not a split_box: prop_parent_region=%p :: %s", prop_parent_region, g_type_name_from_instance((GTypeInstance *) prop_parent_region));
	}
	cat_unref(drop_surface);
}



// TODO: do a lot of checking before casting
static void l_drop(LeaGripMove *grip_move) {
	LeaGripMovePrivate *priv = lea_grip_move_get_instance_private(grip_move);
	LeaFrame *frame = lea_surface_get_frame(priv->surface);
	LeaSurfaceHandler *surface_handler = lea_frame_get_surface_handler(frame);
	lea_surface_handler_dump(surface_handler);

	LeaIDropProposal *proposal = cat_ref_ptr(priv->drop_proposal);
	cat_log_detail("PREDROP:proposal=%o", proposal);
	if (proposal) {
		if (LEA_IS_DROP_CARDINAL_PROPOSAL(proposal)) {
			LeaDropCardinalProposal *moveProposal = LEA_DROP_CARDINAL_PROPOSAL(proposal);
			if (priv->panel_idx>=0) {
				l_drop_move_tab_to_leaf(grip_move, surface_handler, moveProposal);
			}
		} else if (LEA_IS_DROP_TO_TAB_PROPOSAL(proposal)) {
			LeaDropToTabProposal *toTabProposal = LEA_DROP_TO_TAB_PROPOSAL(proposal);

			LeaSurface *source_surface = priv->surface;
			LeaSurface *prop_surface = lea_drop_to_tab_proposal_get_surface(toTabProposal);
			LeaSurfaceTabModel *prop_tab_model = lea_surface_get_tab_model(prop_surface);
			int prop_tab_idx = lea_drop_to_tab_proposal_get_tab_index(toTabProposal);
			LeaSurfaceTabModel *source_tab_model = lea_surface_get_tab_model(source_surface);
			LeaSurfaceTab *source_tab = lea_surface_tab_model_get_tab_at(source_tab_model, priv->panel_idx);
			cat_ref(source_tab);
			if (prop_surface==source_surface && prop_tab_idx>priv->panel_idx) {
				prop_tab_idx--;
			}
			lea_surface_tab_model_remove_tab_at(source_tab_model, priv->panel_idx);
			if (prop_tab_idx<0) {
				prop_tab_idx = lea_surface_tab_model_count(prop_tab_model);
				lea_surface_tab_model_add_tab(prop_tab_model, source_tab);
			} else {
				lea_surface_tab_model_add_tab_at(prop_tab_model, source_tab, prop_tab_idx);
			}
			lea_surface_tab_model_set_selected_tab(prop_tab_model, prop_tab_idx);
			cat_unref(source_tab);
		}

		GdkWindow *window = gtk_widget_get_window(GTK_WIDGET(priv->surface));

		gdk_window_set_cursor(window, NULL);
		cat_log_detail("BEFORE CLEANUP");
		lea_surface_handler_dump(surface_handler);
		lea_surface_handler_cleanup(surface_handler);
		cat_log_detail("AFTER CLEANUP");
		lea_surface_handler_reallocate(surface_handler);
		lea_surface_handler_dump(surface_handler);
	}
	cat_unref(proposal);
}



/********************* start LeaIGrip implementation *********************/

static void l_drawable_draw(LeaIDrawable *self, cairo_t *cairo) {
	LeaGripMovePrivate *priv = lea_grip_move_get_instance_private((LeaGripMove *) self);

	if (priv->drop_proposal) {
		lea_idrop_proposal_paint(priv->drop_proposal, cairo);
	}

}


static cairo_region_t *l_drawable_create_region(LeaIDrawable *self) {
//	LeaGripMovePrivate *priv = lea_grip_move_get_instance_private((LeaGripMove *) self);
	cairo_region_t *result = NULL;
	// TODO implement
	return result;
}


static void l_drawable_iface_init(LeaIDrawableInterface *iface) {
	iface->draw = l_drawable_draw;
	iface->createRegion = l_drawable_create_region;
}

/********************* end LeaIGrip implementation *********************/


/********************* start LeaIGrip implementation *********************/

static LeaSimilarity l_grip_is_identical(LeaIGrip *self, LeaIGrip *other) {
	return LEA_SIMILARITY_NONE;
}

static void l_grip_add_notify(LeaIGrip *self, LeaFrame *frame) {
}

static void l_grip_remove_notify(LeaIGrip *self) {
}


static void l_grip_motion_event(LeaIGrip *self, GdkEventMotion *event) {
	int wmx = (int) event->x;
	int wmy = (int) event->y;
	l_set_location(LEA_GRIP_MOVE(self), wmx, wmy);
}

static void l_grip_release_event(LeaIGrip *self, GdkEventButton *event) {
	LeaGripMove *grip_move = LEA_GRIP_MOVE(self);
	l_drop(grip_move);
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
	LeaGripMove *instance = LEA_GRIP_MOVE(self);
	LeaGripMovePrivate *priv = lea_grip_move_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p panel-idx=%d]", iname, self, priv->panel_idx);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

