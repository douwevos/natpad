/*
   File:    moolayoutmodel.c
   Project: moose
   Author:  Douwe Vos
   Date:    May 25, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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


#include "moolayoutmodelprivate.h"
#include <moose.h>
#include <math.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooLayoutModel"
#include <logging/catlog.h>

struct _MooLayoutModelPrivate {
	MooNodeRenderRegistry *render_registry;
	CatWeakList *listeners;

	MooNodeWo *node;
	gboolean layout_is_valid, selection_is_valid;
	CatArrayWo *e_node_layouts;
	MooNodeLayout *root;

	int width, height;

	MooNodeLayout *cursor;
	MooNodeLayout *shift_cursor;

	CatArrayWo *selection;
};

G_DEFINE_TYPE (MooLayoutModel, moo_layout_model, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_layout_model_class_init(MooLayoutModelClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(MooLayoutModelPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_layout_model_init(MooLayoutModel *instance) {
	MooLayoutModelPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_LAYOUT_MODEL, MooLayoutModelPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooLayoutModel *instance = MOO_LAYOUT_MODEL(object);
	MooLayoutModelPrivate *priv = instance->priv;

	cat_unref_ptr(priv->cursor);
	cat_unref_ptr(priv->node);
	cat_unref_ptr(priv->root);
	cat_unref_ptr(priv->e_node_layouts);
	cat_unref_ptr(priv->render_registry);
	cat_unref_ptr(priv->shift_cursor);
	cat_unref_ptr(priv->selection);
	cat_unref_ptr(priv->listeners);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooLayoutModel *moo_layout_model_new(MooNodeRenderRegistry *render_registry) {
	MooLayoutModel *result = g_object_new(MOO_TYPE_LAYOUT_MODEL, NULL);
	cat_ref_anounce(result);
	MooLayoutModelPrivate *priv = result->priv;
	priv->render_registry = cat_ref_ptr(render_registry);
	priv->listeners = cat_weak_list_new();

	priv->node = NULL;
	priv->root = moo_node_layout_new(NULL, -1);
	moo_node_layout_toggle_expanded(priv->root);
	priv->layout_is_valid = FALSE;
	priv->e_node_layouts = cat_array_wo_new();

	priv->width = 0;
	priv->height = 0;

	priv->cursor = NULL;
	priv->shift_cursor = NULL;
	priv->selection = cat_array_wo_new();

	return result;
}



void moo_layout_model_add_listener(MooLayoutModel *layout_model, MooILayoutModelListener *model_listener) {
	MooLayoutModelPrivate *priv = MOO_LAYOUT_MODEL_GET_PRIVATE(layout_model);
	cat_weak_list_append(priv->listeners, (GObject *) model_listener);
}

void moo_layout_model_remove_listener(MooLayoutModel *layout_model, MooILayoutModelListener *model_listener) {
	MooLayoutModelPrivate *priv = MOO_LAYOUT_MODEL_GET_PRIVATE(layout_model);
	cat_weak_list_remove(priv->listeners, (GObject *) model_listener);
}


void moo_layout_model_set_root_node(MooLayoutModel *layout_model, MooNodeWo *node) {
	MooLayoutModelPrivate *priv = MOO_LAYOUT_MODEL_GET_PRIVATE(layout_model);
	if (priv->node == node) {
		return;
	}
	cat_log_debug("node set=%o", node);
	priv->layout_is_valid = FALSE;
	cat_ref_swap(priv->node, node);
}


static gint l_node_compare_func(MooNodeWo *node_a, MooNodeWo *node_b) {
	if (node_a==node_b) {
		return 0;
	}
	int za = moo_node_wo_get_z_order(node_a);
	int zb = moo_node_wo_get_z_order(node_b);
	if (za>zb) {
		return 1;
	} else if (za<zb) {
		return -1;
	}

	CatStringWo *name_a = moo_node_wo_get_name(node_a);
	CatStringWo *name_b = moo_node_wo_get_name(node_b);
	return cat_string_wo_compare(name_b, name_a);
}

static gboolean l_same_node_layout(MooNodeLayout *layout_a, MooNodeLayout *layout_b)  {
	if (layout_a==layout_b) {
		return TRUE;
	}
	if (layout_a==NULL || layout_b==NULL) {
		return FALSE;
	}
	return cat_long_equal(moo_node_layout_get_node_key(layout_a), moo_node_layout_get_node_key(layout_b));
}

static void l_set_new_selection(MooLayoutModel *layout_model, CatArrayWo *new_selection) {
	MooLayoutModelPrivate *priv = MOO_LAYOUT_MODEL_GET_PRIVATE(layout_model);
	if (!cat_array_wo_equal(priv->selection, new_selection, (GEqualFunc) l_same_node_layout)) {
		cat_ref_swap(priv->selection, new_selection);
		cat_log_debug("setting new selection=%o", new_selection);

		if (cat_weak_list_size(priv->listeners)>0) {
			CatIIterator *iter = cat_weak_list_iterator(priv->listeners);
			while(cat_iiterator_has_next(iter)) {
				MooILayoutModelListener *listener = (MooILayoutModelListener *) cat_iiterator_next(iter);
				MooILayoutModelListenerInterface *iface = MOO_ILAYOUT_MODEL_LISTENER_GET_INTERFACE(listener);
				cat_log_debug("dispatching to listener=%o", listener);
				if (iface->selectionSet) {
					iface->selectionSet(listener, new_selection);
				}
			}
			cat_unref_ptr(iter);
		}
	}
}


static void l_rebuild_selection(MooLayoutModel *layout_model) {
	MooLayoutModelPrivate *priv = MOO_LAYOUT_MODEL_GET_PRIVATE(layout_model);
	CatArrayWo *selection = cat_array_wo_new();
	CatIIterator *iter = cat_array_wo_iterator(priv->e_node_layouts);
	while(cat_iiterator_has_next(iter)) {
		MooNodeLayout *node_layout = (MooNodeLayout *) cat_iiterator_next(iter);
		if (moo_node_layout_is_selected(node_layout)) {
			cat_array_wo_append(selection, (GObject *) node_layout);
		}
	}
	cat_unref_ptr(iter);

	l_set_new_selection(layout_model, selection);
	cat_unref_ptr(selection);
}


static int l_calculate_entity_height(MooLayoutModelPrivate *priv, PangoContext *pango_context) {
	PangoFontMetrics *font_metrics = pango_context_get_metrics(pango_context, NULL, NULL);
	double font_height = ((double) (pango_font_metrics_get_ascent(font_metrics) + pango_font_metrics_get_descent(font_metrics)))/PANGO_SCALE;
//	int ifont_height = ceil(font_height)+1;
//	ifont_height &= 0xffffe;
	return ceil(font_height)+2;
}

static MooNodeLayout *l_find_layout_by_moose_id(CatArrayWo *layout_children, MooNodeWo *child, int guess_idx) {
	if (layout_children!=NULL) {

		long long unique_id = moo_node_wo_get_unique_id(child);

		if (guess_idx>=0 && guess_idx<cat_array_wo_size(layout_children)) {
			MooNodeLayout *layout = (MooNodeLayout *) cat_array_wo_get(layout_children, guess_idx);
			MooNodeWo *node_wo = moo_node_layout_get_node(layout);
			if (moo_node_wo_get_unique_id(node_wo)==unique_id) {
				return layout;
			}
		}
		for(guess_idx=cat_array_wo_size(layout_children)-1; guess_idx>=0; guess_idx--) {
			MooNodeLayout *layout = (MooNodeLayout *) cat_array_wo_get(layout_children, guess_idx);
			MooNodeWo *node_wo = moo_node_layout_get_node(layout);
			if (moo_node_wo_get_unique_id(node_wo)==unique_id) {
				return layout;
			}
		}
	}
	return NULL;

}

void moo_layout_model_validate(MooLayoutModel *layout_model, cairo_t *cairo, PangoContext *pango_context) {
	MooLayoutModelPrivate *priv = MOO_LAYOUT_MODEL_GET_PRIVATE(layout_model);
	if (cairo==NULL) {
		return;
	}
	if (priv->layout_is_valid) {
		if (!priv->selection_is_valid) {
			l_rebuild_selection(layout_model);
			priv->selection_is_valid = TRUE;
		}
		return;
	}
	priv->selection_is_valid = TRUE;
	priv->layout_is_valid = TRUE;
	/* create map of current NodeLayout Elements */
//	CatHashMap /*<Long *, NodeLayout *> */ *mapOld = cat_hash_map_wo_new((GHashFunc) cat_long_hash,(GEqualFunc) cat_long_equal);
//	CatIIterator *iter = cat_array_wo_iterator(priv->node_layouts);
//	while(cat_iiterator_has_next(iter)) {
//		MooNodeLayout *node_layout = (MooNodeLayout *) cat_iiterator_next(iter);
//		CatLong *key = moo_node_layout_get_node_key(node_layout);
//		cat_hash_map_wo_put(e_mapOld, (GObject *) key, (GObject *) node_layout);
//	}
//	cat_unref_ptr(iter);
	cat_array_wo_clear(priv->e_node_layouts);

	MooNodeLayout *old_cursor = priv->cursor;
	cat_unref_ptr(priv->cursor);
	if (priv->node==NULL) {
		priv->width = 0;
		priv->height = 0;
//		cat_unref_ptr(mapOld);
		return;
	}

	CatArrayWo *e_selection = cat_array_wo_new();


	int ifont_height = l_calculate_entity_height(priv, pango_context);


	int ypos = 0;
	cat_log_debug("root-node=%o", priv->node);
	CatArrayWo /*<MooNode>*/ *e_stack = cat_array_wo_new();
//	moo_node_wo_enlist_children(priv->node, stack);

	moo_node_layout_set_node(priv->root, priv->node);

	cat_array_wo_append(e_stack, (GObject *) priv->root);
//		stack.add(node);
	int maxWidth = 0;
	while(cat_array_wo_size(e_stack)>0) {
		MooNodeLayout *work_node_layout = NULL;
		cat_array_wo_remove_last(e_stack, (GObject **) (&work_node_layout));
		cat_log_debug("worknode=%o", work_node_layout);
		if (moo_node_layout_get_tree_level(work_node_layout)>=0) {
			moo_node_layout_refresh_renderer_list(work_node_layout, priv->render_registry);
			moo_node_layout_update_layout(work_node_layout, cairo, ypos, ifont_height);
			ypos += ifont_height;
			int right = moo_node_layout_get_x(work_node_layout) + moo_node_layout_get_width(work_node_layout);


			if (right>maxWidth) {
				maxWidth = right;
			}
			cat_array_wo_append(priv->e_node_layouts, (GObject *) work_node_layout);
		}


		if (moo_node_layout_is_selected(work_node_layout)) {
			cat_array_wo_append(e_selection, (GObject *) work_node_layout);
		}

		MooNodeWo *work_node = moo_node_layout_get_node(work_node_layout);
		CatArrayWo *e_new_layout_children = NULL;
		if (moo_node_layout_is_expanded(work_node_layout) && work_node!=NULL && moo_node_wo_child_count(work_node)>0) {
			CatArrayWo *children = moo_node_wo_enlist_children(work_node, NULL);

			if (children!=NULL && cat_array_wo_size(children)>0) {
				cat_array_wo_sort(children, (GCompareFunc) l_node_compare_func);
				CatArrayWo *layout_children = moo_node_layout_get_children(work_node_layout);
				int childTreeLevel = moo_node_layout_get_tree_level(work_node_layout)+1;
				e_new_layout_children = cat_array_wo_new();
				int child_idx;
				for(child_idx=0; child_idx<cat_array_wo_size(children); child_idx++) {
					MooNodeWo *child = (MooNodeWo *) cat_array_wo_get(children, child_idx);
					MooNodeLayout *childNodeLayout = l_find_layout_by_moose_id(layout_children, child, child_idx);
					if (childNodeLayout == NULL) {
						childNodeLayout = moo_node_layout_new(child, childTreeLevel);
					} else {
						cat_ref_ptr(childNodeLayout);
						moo_node_layout_set_node(childNodeLayout, child);
						if (childNodeLayout==old_cursor) {
							cat_ref_swap(priv->cursor, childNodeLayout);
						}


					}
					cat_array_wo_append(e_new_layout_children, (GObject *) childNodeLayout);
					cat_array_wo_append(e_stack, (GObject *) childNodeLayout);
					cat_unref_ptr(childNodeLayout);
				}
			}
			cat_unref_ptr(children);
		}
		moo_node_layout_set_children(work_node_layout, e_new_layout_children);
		cat_unref(e_new_layout_children);

		cat_unref_ptr(work_node_layout);
	}

	l_set_new_selection(layout_model, e_selection);
	cat_unref_ptr(e_selection);
	cat_unref_ptr(e_stack);
	priv->width = maxWidth+1;
	priv->height = ypos+1;
	cat_log_detail("width=%d, height=%d", priv->width, priv->height);
}





void moo_layout_model_paint(MooLayoutModel *layout_model, PangoContext *pango_context, cairo_t *cairo) {
	MooLayoutModelPrivate *priv = MOO_LAYOUT_MODEL_GET_PRIVATE(layout_model);
	cat_log_debug("painting layout-model:nodeLayouts.size=%d" , cat_array_wo_size(priv->e_node_layouts));
	cairo_set_source_rgb(cairo, 0.0, 0.0, 0.0);

	double clip_x1, clip_y1, clip_x2, clip_y2;
	cairo_clip_extents(cairo, &clip_x1, &clip_y1, &clip_x2, &clip_y2);
	GdkRectangle clip_bounds;
	clip_bounds.x = floor(clip_x1);
	clip_bounds.y = floor(clip_y1);
	clip_bounds.width = ceil(clip_x2);
	clip_bounds.height = ceil(clip_y2);
	clip_bounds.width -= clip_bounds.x;
	clip_bounds.height -= clip_bounds.y;

	PangoFontMetrics *font_metrics = pango_context_get_metrics(pango_context, NULL, NULL);
	double font_height = ((double) (pango_font_metrics_get_ascent(font_metrics) + pango_font_metrics_get_descent(font_metrics)))/PANGO_SCALE;

	int entity_height = l_calculate_entity_height(priv, pango_context);



	cairo_surface_t *back_image = cairo_image_surface_create(CAIRO_FORMAT_RGB24, clip_bounds.width, entity_height);
	cairo_t *cairo_back = cairo_create(back_image);
	cairo_set_font_face(cairo_back, cairo_get_font_face(cairo));
//	cairo_matrix_t mtx;
//	cairo_get_font_matrix(cairo, &mtx);
//	cairo_set_font_matrix(cairo_back, &mtx);

	cairo_translate(cairo_back, -clip_x1, 0);


	cat_log_debug("clip=[%d,%d - %d,%d]",clip_bounds.x, clip_bounds.y, clip_bounds.width, clip_bounds.height);

	int clip_bottom = clip_bounds.y+clip_bounds.height;

	PangoLayout *pango_layout = pango_layout_new(pango_context);


	int bottom = 0;
	GdkRectangle rect_node_layout;
	int layout_idx = (clip_bounds.y/entity_height)-1;
	if (layout_idx<0) {
		layout_idx = 0;
	}
	int count = cat_array_wo_size(priv->e_node_layouts);
	cat_log_debug("layout_idx=%d, entity_height=%d", layout_idx, entity_height);
	for(;layout_idx<count; layout_idx++) {
		MooNodeLayout *node_layout = (MooNodeLayout *) cat_array_wo_get(priv->e_node_layouts, layout_idx);
		moo_node_layout_get_bounds(node_layout, &rect_node_layout);
		if (rect_node_layout.y>clip_bottom) {
			break;
		}

		bottom = rect_node_layout.height+rect_node_layout.y;

		moo_node_layout_paint(node_layout, pango_context, pango_layout, font_height, cairo_back, node_layout==priv->cursor);

		cairo_set_source_surface(cairo, back_image, clip_x1, rect_node_layout.y);
		cairo_paint(cairo);

	}

	cat_unref_ptr(pango_layout);

	if (clip_bounds.y+clip_bounds.height>=bottom) {
		cairo_set_source_rgb(cairo, 1.0, 1.0, 1.0);
		cairo_rectangle(cairo, clip_bounds.x, bottom, clip_bounds.width, clip_bounds.y+clip_bounds.height-bottom);
		cairo_fill(cairo);
	}


	cairo_destroy(cairo_back);
	cairo_surface_destroy(back_image);

}



MooNodeLayout *moo_layout_model_node_at(MooLayoutModel *layout_model, int xpos, int ypos) {
	MooLayoutModelPrivate *priv = MOO_LAYOUT_MODEL_GET_PRIVATE(layout_model);
	MooNodeLayout *result = NULL;
	if (cat_array_wo_size(priv->e_node_layouts)>0) {
		MooNodeLayout *node_layout = (MooNodeLayout *) cat_array_wo_get_first(priv->e_node_layouts);
		int entry_height = moo_node_layout_get_height(node_layout);
		int offset = ypos/entry_height;
		if (offset>=0 && cat_array_wo_size(priv->e_node_layouts)) {
			result = (MooNodeLayout *) cat_array_wo_get(priv->e_node_layouts, offset);
		}
	}
	return result;
}


void moo_layout_model_set_selection(MooLayoutModel *layout_model, int initPos, int endPos) {
	MooLayoutModelPrivate *priv = MOO_LAYOUT_MODEL_GET_PRIVATE(layout_model);
	if (endPos<initPos) {
		initPos = initPos - endPos;
		endPos = endPos + initPos;
		initPos = endPos - initPos;
	}

	int idx;
	for(idx = 0; idx < cat_array_wo_size(priv->e_node_layouts); idx++) {
		MooNodeLayout *nodeLayout = (MooNodeLayout *) cat_array_wo_get(priv->e_node_layouts, idx);
		gboolean isInSelection = (idx >= initPos && idx <= endPos);
		if (moo_node_layout_set_selected(nodeLayout, isInSelection)) {
			priv->selection_is_valid = FALSE;
		}
	}
}



gboolean moo_layout_model_select(MooLayoutModel *layout_model, MooNodeLayout *nodeAt, gboolean tryToggleFirst, gboolean ctrlPressed, gboolean shiftPressed) {
	MooLayoutModelPrivate *priv = MOO_LAYOUT_MODEL_GET_PRIVATE(layout_model);
	if (tryToggleFirst) {

		if (moo_node_layout_is_a_map(nodeAt)) {
			moo_node_layout_toggle_expanded(nodeAt);
			priv->layout_is_valid = FALSE;
			return TRUE;
		}
	}
	if (priv->cursor!=nodeAt) {
		if (priv->cursor!=NULL) {
			moo_node_layout_mark_for_repaint(priv->cursor);
		}
		cat_ref_swap(priv->cursor, nodeAt);
		if (priv->cursor!=NULL) {
			moo_node_layout_mark_for_repaint(priv->cursor);
		}
		if (!shiftPressed) {
			cat_ref_swap(priv->shift_cursor, priv->cursor);
		}
	}

	cat_log_debug("ctrlPressed=%d, shiftPressed=%d", ctrlPressed, shiftPressed);
	if (ctrlPressed) {
		moo_node_layout_toggle_selected(nodeAt);
		priv->selection_is_valid = FALSE;
	} else if (shiftPressed) {
		if (priv->shift_cursor!=NULL) {
			int entry_height = moo_node_layout_get_height(priv->cursor);
			int initPos = moo_node_layout_get_y(priv->shift_cursor)/entry_height;
			int endPos = moo_node_layout_get_y(priv->cursor)/entry_height;
			moo_layout_model_set_selection(layout_model, initPos, endPos);
		}
	} else {
		int entry_height = moo_node_layout_get_height(priv->cursor);
		int endPos = moo_node_layout_get_y(priv->cursor)/entry_height;
		moo_layout_model_set_selection(layout_model, endPos, endPos);
	}
	return TRUE;
}

void moo_layout_model_get_view_size(MooLayoutModel *layout_model, int *width, int *height) {
	MooLayoutModelPrivate *priv = MOO_LAYOUT_MODEL_GET_PRIVATE(layout_model);
	*width = priv->width;
	*height = priv->height;
}


