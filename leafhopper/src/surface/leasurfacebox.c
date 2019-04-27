/*
   File:    leasurfacebox.c
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

#include "leasurfaceboxprivate.h"
#include "leasurface.h"
#include "drag/leagripshift.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaSurfaceBox"
#include <logging/catlog.h>


#define LEA_TYPE_SURFACE_BOX_ENTRY              (lea_surface_box_entry_get_type())
#define LEA_SURFACE_BOX_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_surface_box_entry_get_type(), LeaSurfaceBoxEntry))
#define LEA_SURFACE_BOX_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_SURFACE_BOX_ENTRY, LeaSurfaceBoxEntryClass))
#define LEA_IS_SURFACE_BOX_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_SURFACE_BOX_ENTRY))
#define LEA_IS_SURFACE_BOX_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_SURFACE_BOX_ENTRY))
#define LEA_SURFACE_BOX_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_SURFACE_BOX_ENTRY, LeaSurfaceBoxEntryClass))

typedef struct _LeaSurfaceBoxEntry               LeaSurfaceBoxEntry;
typedef struct _LeaSurfaceBoxEntryClass          LeaSurfaceBoxEntryClass;


struct _LeaSurfaceBoxEntry {
	GObject parent;
	LeaIRegion *region;
	int weight;
};

struct _LeaSurfaceBoxEntryClass {
	GObjectClass parent_class;
};


GType lea_surface_box_entry_get_type();

static void l_entry_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaSurfaceBoxEntry, lea_surface_box_entry, G_TYPE_OBJECT, // @suppress("Unused static function")
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_entry_stringable_iface_init)
);

static void l_entry_dispose(GObject *object);
static void l_entry_finalize(GObject *object);

static void lea_surface_box_entry_class_init(LeaSurfaceBoxEntryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_entry_dispose;
	object_class->finalize = l_entry_finalize;
}

static void lea_surface_box_entry_init(LeaSurfaceBoxEntry *instance) {
}

static void l_entry_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaSurfaceBoxEntry *instance = LEA_SURFACE_BOX_ENTRY(object);
	cat_unref_ptr(instance->region);
	G_OBJECT_CLASS(lea_surface_box_entry_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_entry_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_surface_box_entry_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaSurfaceBoxEntry *lea_surface_box_entry_new(LeaIRegion *region, int weight) {
	LeaSurfaceBoxEntry *result = g_object_new(LEA_TYPE_SURFACE_BOX_ENTRY, NULL);
	cat_ref_anounce(result);
	result->region = cat_ref_ptr(region);
	result->weight = weight;
	return result;
}

static void l_entry_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaSurfaceBoxEntry *entry = (LeaSurfaceBoxEntry *) self;
	const char *iname = g_type_name_from_instance((GTypeInstance *) entry);
	cat_string_wo_format(append_to, "%s[%p weight=%d, region=%o]", iname, entry, entry->weight, entry->region);
}

static void l_entry_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_entry_print;
}









struct _LeaSurfaceBoxPrivate {
	LeaSurfaceBoxOrientation orientation;
	CatArrayWo *e_children;
	GtkAllocation allocation;
	gboolean weights_dirty;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_region_iface_init(LeaIRegionInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaSurfaceBox, lea_surface_box, G_TYPE_OBJECT,
		G_ADD_PRIVATE(LeaSurfaceBox)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
		G_IMPLEMENT_INTERFACE(LEA_TYPE_IREGION, l_region_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_surface_box_class_init(LeaSurfaceBoxClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_surface_box_init(LeaSurfaceBox *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaSurfaceBox *instance = LEA_SURFACE_BOX(object);
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(instance);
	cat_unref_ptr(priv->e_children);
	G_OBJECT_CLASS(lea_surface_box_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_surface_box_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaSurfaceBox *lea_surface_box_new(LeaSurfaceBoxOrientation orientation) {
	LeaSurfaceBox *result = g_object_new(LEA_TYPE_SURFACE_BOX, NULL);
	cat_ref_anounce(result);
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(result);
	priv->orientation = orientation;
	priv->e_children = cat_array_wo_new();
	priv->weights_dirty = TRUE;
	return result;
}


static void l_validate_weights(LeaSurfaceBoxPrivate *priv) {
	if (priv->weights_dirty) {
		priv->weights_dirty = FALSE;

		long long accum_weight = 0;
		CatIIterator *iter = cat_array_wo_iterator(priv->e_children);
		while(cat_iiterator_has_next(iter)) {
			LeaSurfaceBoxEntry * entry = (LeaSurfaceBoxEntry *) cat_iiterator_next(iter);
			cat_log_debug("entry->weight=%d", entry->weight);
			accum_weight += entry->weight;
		}
		cat_unref_ptr(iter);

		long long accum_rounder = accum_weight/2;

		if (accum_weight<=0) {
			accum_weight = 1;
		}

		cat_log_debug("accum_weight=%ld", accum_weight);

		iter = cat_array_wo_iterator(priv->e_children);
		while(cat_iiterator_has_next(iter)) {
			LeaSurfaceBoxEntry * entry = (LeaSurfaceBoxEntry *) cat_iiterator_next(iter);
			cat_log_debug("pre:entry->weight=%d", entry->weight);
			long long weight = entry->weight;
			weight = (accum_rounder + (weight * LEA_SB_PREFERED_ACCUM_WEIGHT))/accum_weight;
			entry->weight = (int) weight;
			cat_log_debug("post:entry->weight=%d", entry->weight);
		}
		cat_unref_ptr(iter);
	}
}


void lea_surface_box_add(LeaSurfaceBox *surface_box, LeaIRegion *region, int index, int weight) {
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(surface_box);
	LeaSurfaceBoxEntry *entry = lea_surface_box_entry_new(region, weight);
	if (index<0 || index>=cat_array_wo_size(priv->e_children)) {
		cat_array_wo_append(priv->e_children, (GObject *) entry);
	} else {
		cat_array_wo_insert(priv->e_children, (GObject *) entry, index);
	}
	cat_unref_ptr(entry);
	priv->weights_dirty = TRUE;
}

int lea_surface_box_entry_count(LeaSurfaceBox *surface_box) {
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(surface_box);
	return cat_array_wo_size(priv->e_children);
}

LeaIRegion *lea_surface_box_get_region_at(LeaSurfaceBox *surface_box, int index) {
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(surface_box);
	LeaIRegion *result = NULL;
	LeaSurfaceBoxEntry *entry = (LeaSurfaceBoxEntry *) cat_array_wo_get(priv->e_children, index);
	if (entry) {
		result = entry->region;
	}
	return result;
}

int lea_surface_box_get_weight_at(LeaSurfaceBox *surface_box, int index) {
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(surface_box);
	l_validate_weights(priv);
	int result = 0;
	LeaSurfaceBoxEntry *entry = (LeaSurfaceBoxEntry *) cat_array_wo_get(priv->e_children, index);
	if (entry) {
		result = entry->weight;
	}
	return result;
}

void lea_surface_box_set_weight_at(LeaSurfaceBox *surface_box, int weight, int index) {
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(surface_box);
	LeaSurfaceBoxEntry *entry = (LeaSurfaceBoxEntry *) cat_array_wo_get(priv->e_children, index);
	if (entry) {
		entry->weight = weight;
		priv->weights_dirty = TRUE;
	}
}


LeaSurfaceBoxOrientation lea_surface_box_get_orientation(LeaSurfaceBox *surface_box) {
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(surface_box);
	return priv->orientation;
}

int lea_surface_box_index_of(LeaSurfaceBox *surface_box, LeaIRegion *region) {
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(surface_box);
	int result = -1;
	int index = 0;
	CatIIterator *iter = cat_array_wo_iterator(priv->e_children);
	while(cat_iiterator_has_next(iter)) {
		LeaSurfaceBoxEntry *entry = (LeaSurfaceBoxEntry *) cat_iiterator_next(iter);
		if (entry->region == region) {
			result = index;
			break;
		}
		index++;
	}
	cat_unref_ptr(iter);
	return result;
}

void lea_surface_box_remove_at(LeaSurfaceBox *surface_box, int index) {
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(surface_box);
	cat_array_wo_remove(priv->e_children, index, NULL);
	priv->weights_dirty = TRUE;
}

void lea_surface_box_replace_at(LeaSurfaceBox *surface_box, LeaIRegion *replacement, int index) {
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(surface_box);
	cat_ref(replacement);
	LeaSurfaceBoxEntry *entry = (LeaSurfaceBoxEntry *) cat_array_wo_get(priv->e_children, index);
	cat_ref_swap(entry->region, replacement);
	cat_unref(replacement);
	priv->weights_dirty = TRUE;
}

void lea_surface_box_merge_at(LeaSurfaceBox *surface_box, LeaSurfaceBox *box_to_merge, int index) {
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(surface_box);
	LeaSurfaceBoxPrivate *mpriv = lea_surface_box_get_instance_private(box_to_merge);
	LeaSurfaceBoxEntry *preserved_entry = NULL;
	cat_array_wo_remove(priv->e_children, index, (GObject **) (&preserved_entry));
	int count = cat_array_wo_size(mpriv->e_children);
	for(count--; count>=0; count--) {
		GObject *entry = cat_array_wo_get(mpriv->e_children, count);
		cat_array_wo_insert(priv->e_children, entry, index);
	}
	cat_unref_ptr(preserved_entry);
	priv->weights_dirty = TRUE;
}


LeaIGrip *lea_surf_box_calculate_locator(LeaSurfaceBox *surface_box, int x, int y, int drag_magnet, LeaIGrip *top_grip) {
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(surface_box);
	LeaIGrip *new_locator;
	GtkAllocation region_allocation = lea_iregion_get_allocation((LeaIRegion *) surface_box);
	int delta_left = region_allocation.x-x;
	int delta_top = region_allocation.y-y;
	int delta_bottom;
	int delta_right;

//	cat_log_debug("running node:%p, x:%d, y:%d   mouse[%d, %d]", node, node->calc_x, node->calc_y, x, y);

	int child_count = cat_array_wo_size(priv->e_children);

	int child_idx;
	LeaSurfaceBoxEntry *entry;
	gboolean is_horizontal = priv->orientation == LEA_SB_HORIZONTAL;
	for(child_idx=0; child_idx<child_count; child_idx++) {
		entry = (LeaSurfaceBoxEntry *) cat_array_wo_get(priv->e_children, child_idx);
		if (LEA_IS_SURFACE_BOX(entry->region)) {
			top_grip = lea_surf_box_calculate_locator(LEA_SURFACE_BOX(entry->region), x, y, drag_magnet, top_grip);
		}
		if (top_grip==NULL && child_idx>0) {
			GtkAllocation entry_allocation = lea_iregion_get_allocation(entry->region);
			delta_left = entry_allocation.x-x;
			delta_right = entry_allocation.x+entry_allocation.width-x;
			delta_top = entry_allocation.y-y;
			delta_bottom = entry_allocation.y+entry_allocation.height-y;

			gboolean is_in_range = FALSE;
			int sqd_delta = 0;
			if (is_horizontal) {
				sqd_delta = delta_left*delta_left;
				if (sqd_delta<drag_magnet*drag_magnet) {
					is_in_range = TRUE;
					if ((delta_top>0) || (delta_bottom<0)) {
						is_in_range = FALSE;
					}
				}
			} else {
				sqd_delta = delta_top*delta_top;
				if (sqd_delta<drag_magnet*drag_magnet) {
					is_in_range = TRUE;
					if ((delta_left>0) || (delta_right<0)) {
						is_in_range = FALSE;
					}
				}
			}

			cat_log_trace("[%d], delta::left=%d,right=%d, top=%d,bottom=%d,  is_in_range=%d", child_idx, delta_left, delta_right, delta_top, delta_bottom, is_in_range);
			if (is_in_range) {
				cat_log_detail("delta_left=%d, child_idx=%d", delta_left, child_idx);
				new_locator = (LeaIGrip *) lea_grip_shift_new(surface_box, child_idx, delta_left, delta_top);
				cat_unref(top_grip);
				top_grip = new_locator;
			}
		}
	}

	return top_grip;
}


/********************* start LeaIRegion implementation *********************/

static void l_region_size_allocate(LeaIRegion *self, GtkAllocation *allocation) {
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(LEA_SURFACE_BOX(self));
	priv->allocation = *allocation;
	l_validate_weights(priv);

	gboolean horizontal = priv->orientation == LEA_SB_HORIZONTAL;

	int accum_weight = 0;
	CatIIterator *iter = cat_array_wo_iterator(priv->e_children);
	while(cat_iiterator_has_next(iter)) {
		LeaSurfaceBoxEntry * entry = (LeaSurfaceBoxEntry *) cat_iiterator_next(iter);
		accum_weight += entry->weight;
	}
	cat_unref_ptr(iter);

	int accum_rounder = accum_weight/2;


	float pos = 0.f;
	float depth = 0.f;
	if (horizontal) {
		depth = allocation->width;
	} else {
		depth = allocation->height;
	}

	GtkAllocation child_allocation;
	iter = cat_array_wo_iterator(priv->e_children);
	while(cat_iiterator_has_next(iter)) {
		LeaSurfaceBoxEntry * entry = (LeaSurfaceBoxEntry *) cat_iiterator_next(iter);

		float child_depth = (accum_rounder + (depth*entry->weight))/accum_weight;
		int ipos = (int) (pos);
		int idepth = (int) child_depth;

		if (horizontal) {
			child_allocation.x = allocation->x+ipos;
			child_allocation.width = idepth;

			child_allocation.y = allocation->y;
			child_allocation.height = allocation->height;
		} else {
			child_allocation.y = allocation->y+ipos;
			child_allocation.height = idepth;

			child_allocation.x = allocation->x;
			child_allocation.width = allocation->width;
		}

		cat_log_debug("calling size_allocate on region:%o, allocation[%d,%d,%d,%d]", entry->region, child_allocation.x, child_allocation.y, child_allocation.width, child_allocation.height);

		lea_iregion_size_allocate(entry->region, &child_allocation);
		pos += child_depth;
	}
	cat_unref_ptr(iter);
}

static const GtkAllocation l_region_get_allocation(LeaIRegion *self) {
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(LEA_SURFACE_BOX(self));
	return priv->allocation;
}


static void l_region_iface_init(LeaIRegionInterface *iface) {
	iface->sizeAllocate = l_region_size_allocate;
	iface->getAllocation = l_region_get_allocation;
}

/********************* end LeaIRegion implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	LeaSurfaceBox *instance = LEA_SURFACE_BOX(self);
	LeaSurfaceBoxPrivate *priv = lea_surface_box_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p orientation=%s, alloc=[%d,%d,%d,%d]]", iname, self, priv->orientation==LEA_SB_HORIZONTAL ? "horizontal" : "vertical", priv->allocation.x, priv->allocation.y, priv->allocation.width, priv->allocation.height);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
