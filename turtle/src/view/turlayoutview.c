/*
   File:    turlayoutview.c
   Project: turtle
   Author:  Douwe Vos
   Date:    Feb 13, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#include "turlayoutview.h"
#include "tursurfacepool.h"
#include "turlineview.h"
#include "../model/turinode.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_ALL
#define CAT_LOG_CLAZZ "TurLayoutView"
#include <logging/catlog.h>

struct _TurLayoutViewPrivate {
	GtkWidget *widget;
	PangoContext *pango_context;
	TurLayout *layout;
	int line_height;
	long long int view_y;
	int view_width, view_height;

	CatArrayWo *line_view_cache;
	TurSurfacePool *surface_pool;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TurLayoutView, tur_layout_view, G_TYPE_OBJECT,
		G_ADD_PRIVATE(TurLayoutView)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void tur_layout_view_class_init(TurLayoutViewClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void tur_layout_view_init(TurLayoutView *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	TurLayoutView *instance = TUR_LAYOUT_VIEW(object);
//	TurLayoutViewPrivate *priv = tur_layout_view_get_instance_private(instance);
	G_OBJECT_CLASS(tur_layout_view_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(tur_layout_view_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


TurLayoutView *tur_layout_view_new(GtkWidget *widget, PangoContext *pango_context, TurLayout *layout) {
	TurLayoutView *result = g_object_new(TUR_TYPE_LAYOUT_VIEW, NULL);
	cat_ref_anounce(result);
	TurLayoutViewPrivate *priv = tur_layout_view_get_instance_private(result);
	priv->widget = widget;
	priv->pango_context = cat_ref_ptr(pango_context);
	priv->layout = cat_ref_ptr(layout);
	priv->surface_pool = tur_surface_pool_new();

	PangoLayout *pango_layout = pango_layout_new(priv->pango_context);
	pango_layout_set_text(pango_layout, "0123456789", 10);

	PangoRectangle inkt_rect;
	pango_layout_get_pixel_extents(pango_layout, NULL, &inkt_rect);
	priv->line_height = inkt_rect.height;
	cat_unref_ptr(pango_layout);

	tur_surface_pool_set_image_height(priv->surface_pool, priv->line_height);


	return result;
}



void tur_layout_view_set_view_size(TurLayoutView *view, int view_width, int view_height) {
	TurLayoutViewPrivate *priv = tur_layout_view_get_instance_private(view);
	priv->view_height = view_height;
	priv->view_width = view_width;
	tur_surface_pool_set_image_width(priv->surface_pool, view_width);
}


void tur_layout_view_draw(TurLayoutView *view, cairo_t *cr) {
	TurLayoutViewPrivate *priv = tur_layout_view_get_instance_private(view);

	cairo_translate(cr, 0, (double) -priv->view_y);

	if (priv->line_view_cache) {

		CatIIterator *iter = cat_array_wo_iterator(priv->line_view_cache);
		while(cat_iiterator_has_next(iter)) {
			TurLineView *line_view = (TurLineView *) cat_iiterator_next(iter);
			tur_line_view_draw(line_view, cr);
		}
		cat_unref_ptr(iter);
	}
}

static TurLineView *l_find_existing_line_view(CatArrayWo *line_view_cache, TurLine *line, int *old_lv_idx) {
	int exp_index = *old_lv_idx;
	if (exp_index>=0) {
		TurLineView *test = (TurLineView *) cat_array_wo_get(line_view_cache, exp_index);
		if (test != NULL) {
			if (tur_line_view_defined_by(test, line)) {
				*old_lv_idx = exp_index+1;
				return test;
			}
		}
	}
	cat_log_debug("cache miss:line=%p, exp_index=%d", line, exp_index);
	int c_idx = cat_array_wo_size(line_view_cache)-1;
	for(;c_idx>=0; c_idx--) {
		TurLineView *test = (TurLineView *) cat_array_wo_get(line_view_cache, c_idx);
		if (test != NULL) {
			if (tur_line_view_defined_by(test, line)) {
				*old_lv_idx = c_idx+1;
				return test;
			}
		}
	}
	return NULL;
}


void tur_layout_view_invalidate(TurLayoutView *view) {
	TurLayoutViewPrivate *priv = tur_layout_view_get_instance_private(view);
//	const long long int view_top = priv->view_y-TOP_BOTTOM_CACHED_PIXELS;
//	const long long int view_bottom = priv->view_y+priv->view_height+TOP_BOTTOM_CACHED_PIXELS;
	const long long int view_top = 0;
	const long long int view_bottom = 500;


	TurLine *line = tur_layout_get_root_line(priv->layout);

	long long int output_y = 0;


	CatArrayWo *new_line_view_cache = NULL;
	if (priv->line_view_cache) {
		new_line_view_cache = cat_array_wo_new_size(cat_array_wo_size(priv->line_view_cache)+3);
	} else {
		new_line_view_cache = cat_array_wo_new();
	}


	int old_lv_idx = 0;

	PangoLayout *pango_layout = pango_layout_new(priv->pango_context);
	TurLineViewUpdate line_view_update;
	line_view_update.surface_pool = priv->surface_pool;
	line_view_update.view_width = priv->view_width;
	line_view_update.view_height = priv->view_height;
	line_view_update.line_height = priv->line_height;
	line_view_update.window = gtk_widget_get_window(priv->widget);
	line_view_update.pango_layout = pango_layout;
	while(TRUE) {

		TurLineView *line_view = NULL;
		if (priv->line_view_cache) {
			line_view = l_find_existing_line_view(priv->line_view_cache, line, &old_lv_idx);
		}


		if (line_view==NULL) {
			line_view = tur_line_view_new(line);
		} else {
			cat_ref_ptr(line_view);
		}


		line_view_update.y_pos = output_y;
		gboolean needs_repaint = tur_line_view_update(line_view, line_view_update);
		if (needs_repaint) {
			int yposs=(int) (output_y-priv->view_y);
			gtk_widget_queue_draw_area(priv->widget, 0, yposs, priv->view_width, priv->line_height);
		}

		cat_array_wo_append(new_line_view_cache, (GObject *) line_view);
		cat_unref_ptr(line_view);

//		TurINode *node = tur_line_get_node(line);
//
//		TurINodeInterface *tur_iface = TUR_INODE_GET_INTERFACE(node);
//		CatStringWo *name = tur_iface->name(node);
//
//		pango_layout_set_text(pango_layout, cat_string_wo_getchars(name));
//		cairo_rel_move_to(cr, 0, output_y);


		output_y += priv->line_height;

		break;
	}

	cat_unref_ptr(priv->line_view_cache);
	priv->line_view_cache = new_line_view_cache;

	cat_unref_ptr(pango_layout);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	TurLayoutView *instance = TUR_LAYOUT_VIEW(self);
	TurLayoutViewPrivate *priv = tur_layout_view_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
