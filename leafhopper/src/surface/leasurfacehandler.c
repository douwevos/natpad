/*
   File:    leasurfacehandler.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 17, 2014
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

#include "leasurfacehandlerprivate.h"
#include "leasurfaceboxprivate.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaSurfaceHandler"
#include <logging/catlog.h>

struct _LeaSurfaceHandlerPrivate {
	LeaSurfaceBox *root_box;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaSurfaceHandler, lea_surface_handler, G_TYPE_OBJECT,
		G_ADD_PRIVATE(LeaSurfaceHandler)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_surface_handler_class_init(LeaSurfaceHandlerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_surface_handler_init(LeaSurfaceHandler *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaSurfaceHandler *instance = LEA_SURFACE_HANDLER(object);
	LeaSurfaceHandlerPrivate *priv = lea_surface_handler_get_instance_private(instance);
	cat_unref_ptr(priv->root_box);
	G_OBJECT_CLASS(lea_surface_handler_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_surface_handler_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaSurfaceHandler *lea_surface_handler_new() {
	LeaSurfaceHandler *result = g_object_new(LEA_TYPE_SURFACE_HANDLER, NULL);
	cat_ref_anounce(result);
	LeaSurfaceHandlerPrivate *priv = lea_surface_handler_get_instance_private(result);
	priv->root_box = lea_surface_box_new(LEA_SB_HORIZONTAL);
	return result;
}


LeaSurfaceBox *lea_surface_handler_get_root(LeaSurfaceHandler *surface_handler) {
	LeaSurfaceHandlerPrivate *priv = lea_surface_handler_get_instance_private(surface_handler);
	return priv->root_box;
}

static void l_unparent_surface(LeaSurface *surface) {
	if (surface) {
		GtkWidget *parent_w = gtk_widget_get_parent(GTK_WIDGET(surface));
		if (parent_w) {
			gtk_container_remove(GTK_CONTAINER(parent_w), GTK_WIDGET(surface));
		}
	}
}

static gboolean l_clean_up_node(LeaSurfaceBox *surface_box) {

	gboolean result = FALSE;
	int count = lea_surface_box_entry_count(surface_box);
	int idx;
	for(idx=0; idx<count; idx++) {
		LeaIRegion *child = lea_surface_box_get_region_at(surface_box, idx);
		if (LEA_IS_SURFACE(child)) {
			LeaSurface *surface = LEA_SURFACE(child);
			if (!lea_surface_get_hold(surface)) {
				if (!lea_surface_has_tabs(surface)) {
					cat_ref_ptr(surface);
					lea_surface_box_remove_at(surface_box, idx);
					l_unparent_surface(surface);
					idx--;
					count--;
					result = TRUE;
					continue;
				}
			}
		}

		if (LEA_IS_SURFACE_BOX(child)) {
			LeaSurfaceBox *child_box = LEA_SURFACE_BOX(child);
			int child_count = lea_surface_box_entry_count(child_box);
			/* replace boxes with just 1 child */
			if (child_count==1) {
				LeaIRegion *replacement = lea_surface_box_get_region_at(child_box, 0);
				lea_surface_box_replace_at(surface_box, replacement, idx);
				idx--;
				result = TRUE;
				continue;
			}
			if (lea_surface_box_get_orientation(surface_box)==lea_surface_box_get_orientation(child_box)) {
				lea_surface_box_merge_at(surface_box, child_box, idx);
				idx--;
				count = lea_surface_box_entry_count(surface_box);
				result = TRUE;
				continue;
			}
			if (l_clean_up_node(child_box)) {
				result = TRUE;
			}
		}
	}
	return result;
}

void lea_surface_handler_cleanup(LeaSurfaceHandler *surface_handler) {
	cat_log_detail("cleaning up layout");
	LeaSurfaceHandlerPrivate *priv = lea_surface_handler_get_instance_private(surface_handler);

	if (priv->root_box==NULL) {
		return;
	}
	int count = 0;
	while(TRUE) {
		if (l_clean_up_node(priv->root_box)) {
			if(count++>200) {
				return;
			}
		} else {
			if (lea_surface_box_entry_count(priv->root_box)==1) {
				LeaIRegion *single_region = lea_surface_box_get_region_at(priv->root_box, 0);
				if (LEA_IS_SURFACE_BOX(single_region)) {
					const GtkAllocation allocation = lea_iregion_get_allocation(LEA_IREGION(priv->root_box));
					lea_iregion_size_allocate(single_region, (GtkAllocation *) (&allocation));
					cat_ref(single_region);
					lea_surface_box_remove_at(priv->root_box, 0);
					cat_ref_swap(priv->root_box, single_region);
					cat_unref(single_region);
					continue;
				}
			}
			break;
		}
	}
}

static LeaIRegion *l_recursive_search_parent(LeaIRegion *base_region, LeaIRegion *region) {
	LeaIRegion *result = NULL;
	if (LEA_IS_SURFACE_BOX(base_region)) {
		LeaSurfaceBox *box = LEA_SURFACE_BOX(base_region);
		int box_idx = lea_surface_box_entry_count(box);
		for(box_idx--; result==NULL && box_idx>=0; box_idx--) {
			LeaIRegion *child = lea_surface_box_get_region_at(box, box_idx);
			if (child == region) {
				result = base_region;
			} else {
				result = l_recursive_search_parent(child, region);
			}
		}
	}
	return result;
}

LeaIRegion *lea_surface_handler_find_parent_region(LeaSurfaceHandler *surface_handler, LeaIRegion *region) {
	LeaSurfaceHandlerPrivate *priv = lea_surface_handler_get_instance_private(surface_handler);
	return l_recursive_search_parent((LeaIRegion *) priv->root_box, region);
}


static LeaSurface *l_find_matching_surface(LeaIRegion *region, LeaMatchFunc match_func, gpointer match_data, LeaSurface *best, int *best_score) {
	if (LEA_IS_SURFACE(region)) {
		int score = match_func(region, match_data);
		if (score>*best_score) {
			*best_score = score;
			return LEA_SURFACE(region);
		}
	} else if (LEA_IS_SURFACE_BOX(region)) {
		LeaSurfaceBox *box = (LeaSurfaceBox *) region;
		int count = lea_surface_box_entry_count(box);
		int idx = 0;
		for(idx=0; idx<count; idx++) {
			LeaIRegion *child = lea_surface_box_get_region_at(box, idx);
			best = l_find_matching_surface(child, match_func, match_data, best, best_score);
		}
	}
	return best;
}

LeaSurface *lea_surface_handler_find_matching_surface(LeaSurfaceHandler *surface_handler, LeaMatchFunc match_func, gpointer match_data) {
	LeaSurfaceHandlerPrivate *priv = lea_surface_handler_get_instance_private(surface_handler);
	LeaIRegion *region = (LeaIRegion *) priv->root_box;
	int bs = 0;
	return l_find_matching_surface(region, match_func, match_data, NULL, &bs);

}



LeaSurface *lea_surface_handler_create_surface(LeaSurfaceHandler *surface_handler) {
	LeaSurfaceHandlerPrivate *priv = lea_surface_handler_get_instance_private(surface_handler);
	LeaSurface *result = NULL;
	LeaSurfaceBox *root_box = priv->root_box;
	int child_count = lea_surface_box_entry_count(root_box);
	if (lea_surface_box_get_orientation(root_box) == LEA_SB_HORIZONTAL) {
		if (child_count==1) {
			int child_weight = lea_surface_box_get_weight_at(root_box, 0);
			result = lea_surface_new();
			lea_surface_box_add(root_box, LEA_IREGION(result), 1, child_weight*5);
			cat_unref(result);
		} else {
			int idx;
			int best_weight = -1;
			int best_index = -1;
			for(idx=0; idx<child_count; idx++) {
				LeaIRegion *child_node = lea_surface_box_get_region_at(root_box, idx);
				int child_weight = lea_surface_box_get_weight_at(root_box, idx);
				if (LEA_IS_SURFACE(child_node)) {
					if (best_index==-1 || best_weight<child_weight) {
						best_weight = child_weight;
						best_index = idx;
					}
				}
			}

			if (best_index>=0) {
				result = LEA_SURFACE(lea_surface_box_get_region_at(root_box, best_index));
			} else {
				LeaSurfaceBox *new_box = lea_surface_box_new(LEA_SB_VERTICAL);
				const GtkAllocation alloc = lea_iregion_get_allocation((LeaIRegion *) priv->root_box);
				lea_iregion_size_allocate((LeaIRegion *) new_box, (GtkAllocation *) &alloc);

				result = lea_surface_new();
				lea_surface_box_add(new_box, LEA_IREGION(result), 0, 500);
				lea_surface_box_add(new_box, LEA_IREGION(root_box), 1, 100);

				cat_ref_swap(priv->root_box, new_box);
				cat_unref_ptr(new_box);
				cat_unref(result);
			}
		}
	} else {
		LeaSurfaceBox *new_root = lea_surface_box_new(LEA_SB_HORIZONTAL);
		result = lea_surface_new();
		lea_surface_box_add(new_root, LEA_IREGION(result), 0, 500);
		lea_surface_box_add(new_root, LEA_IREGION(root_box), 1, 500);
		cat_ref_swap(priv->root_box, new_root);
		cat_unref_ptr(new_root);
		cat_unref(result);
	}

	gtk_widget_show_all(GTK_WIDGET(result));

	lea_surface_set_hold(result, TRUE);
	lea_surface_handler_cleanup(surface_handler);
	lea_surface_set_hold(result, FALSE);
	return result;
}




#define DRAG_MAGNET  10



LeaIGrip *lea_surface_handler_calculate_locator(LeaSurfaceHandler *surface_handler, int x, int y) {
	LeaSurfaceHandlerPrivate *priv = lea_surface_handler_get_instance_private(surface_handler);
	return lea_surf_box_calculate_locator(priv->root_box, x, y, DRAG_MAGNET, NULL);
}

void lea_surface_handler_allocate(LeaSurfaceHandler *surface_handler, GtkAllocation *allocation) {
	LeaSurfaceHandlerPrivate *priv = lea_surface_handler_get_instance_private(surface_handler);
	lea_iregion_size_allocate((LeaIRegion *) priv->root_box, allocation);
}


void lea_surface_handler_reallocate(LeaSurfaceHandler *surface_handler) {
	LeaSurfaceHandlerPrivate *priv = lea_surface_handler_get_instance_private(surface_handler);
	const GtkAllocation root_alloc = lea_iregion_get_allocation((LeaIRegion *) priv->root_box);
	lea_iregion_size_allocate((LeaIRegion *) priv->root_box, (GtkAllocation *) &root_alloc);

}

//
//static void l_recursive_dump(CatStringWo *e_buf, int weight, LeaIRegion *region, int level) {
//	cat_string_wo_clear(e_buf);
//	int idx;
//	for(idx=0; idx<level; idx++) {
//		cat_string_wo_append_chars(e_buf, "  ");
//	}
//	cat_string_wo_format(e_buf, "%d : %o", weight, region);
//	cat_log_print("DUMP", "%s", cat_string_wo_getchars(e_buf));
//	if (LEA_IS_SURFACE_BOX(region)) {
//		LeaSurfaceBox *box = (LeaSurfaceBox *) region;
//		int entry_count = lea_surface_box_entry_count(box);
//		level++;
//		for(idx=0; idx<entry_count; idx++) {
//			LeaIRegion *child = lea_surface_box_get_region_at(box, idx);
//			int child_weight = lea_surface_box_get_weight_at(box, idx);
//			l_recursive_dump(e_buf, child_weight, child, level);
//		}
//	}
//}


void lea_surface_handler_dump(LeaSurfaceHandler *surface_handler) {
//	LeaSurfaceHandlerPrivate *priv = lea_surface_handler_get_instance_private(surface_handler);
//	CatStringWo *e_buf = cat_string_wo_new();
//	l_recursive_dump(e_buf, 100, (LeaIRegion *) priv->root_box, 0);
//	cat_unref_ptr(e_buf);
//	cat_log_print("DUMP", "done");
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaSurfaceHandler *instance = LEA_SURFACE_HANDLER(self);
	LeaSurfaceHandlerPrivate *priv = lea_surface_handler_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p root-box=%p]", iname, self, priv->root_box);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

