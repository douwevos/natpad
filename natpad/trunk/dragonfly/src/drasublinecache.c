/*
   File:    drasublinecache.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Mar 28, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "drasublinecache.h"
#include "dralinelayout.h"
#include "dralinetagwo.h"
#include <layout/chasublinecacheprivate.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraSubLineCache"
#include <logging/catlog.h>

struct _DraSubLineCachePrivate {
	void *dummy;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraSubLineCache, dra_sub_line_cache, CHA_TYPE_SUB_LINE_CACHE,
		G_ADD_PRIVATE(DraSubLineCache)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_update(ChaSubLineCache *sub_line_cache, ChaSubLineCacheUpdate *update_ctx);

static void dra_sub_line_cache_class_init(DraSubLineCacheClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaSubLineCacheClass *cha_class = CHA_SUB_LINE_CACHE_CLASS(clazz);
	cha_class->update = l_update;
}

static void dra_sub_line_cache_init(DraSubLineCache *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(dra_sub_line_cache_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_sub_line_cache_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

DraSubLineCache *dra_sub_line_cache_new(ChaSurfacePool *surface_pool, ChaLineLayout *line_layout, int sub_line_index) {
	DraSubLineCache *result = g_object_new(DRA_TYPE_SUB_LINE_CACHE, NULL);
	cat_ref_anounce(result);
//	DraSubLineCachePrivate *priv = dra_sub_line_cache_get_instance_private(result);
	cha_sub_line_cache_construct((ChaSubLineCache *) result, surface_pool, line_layout, sub_line_index);
	return result;
}



static void l_draw_curlies(cairo_t *cairo, int view_x, int start_x, int width, int ypos) {
	int end_x = start_x+width;
	end_x = end_x + 3 - (end_x % 3);
	start_x = start_x - (start_x % 3);
	width = end_x - start_x;
	cairo_move_to(cairo, start_x-view_x, ypos);
	double ydelta = 1.5;
	if (start_x%6==0) {
		ydelta = -ydelta;
	}
	int x;
	for(x=0; x<width; x+=3) {
		double xp = start_x+x+1.5;
		double yp = ypos+ydelta;
//		cairo_curve_to(cairo, xp, yp, xp, yp, start_x+x+3, ypos);
		cairo_line_to(cairo, xp-view_x, yp);
		ydelta = -ydelta;
	}
}

static gboolean l_update(ChaSubLineCache *sub_line_cache, ChaSubLineCacheUpdate *update_ctx) {

	gboolean result = CHA_SUB_LINE_CACHE_CLASS(dra_sub_line_cache_parent_class)->update(sub_line_cache, update_ctx);
	if (update_ctx->cache_dirty) {
		ChaCairoSurfaceWrapper *surface_wrapper = cha_sub_line_cache_get_surface((ChaSubLineCache *) sub_line_cache);
		cairo_t *surface_cr = cha_cairo_surface_wrapper_get_context(surface_wrapper);
		int sub_line_idx = cha_sub_line_cache_get_sub_line_index(sub_line_cache);
		PangoLayoutLine *pango_line = pango_layout_get_line_readonly(update_ctx->pango_layout, sub_line_idx);

		DraLineLayout *line_layout = (DraLineLayout *) cha_sub_line_cache_get_line_layout(sub_line_cache);
		CatArrayWo *a_line_tags = dra_line_layout_get_line_tags(line_layout);
		cat_log_debug("a_line_tags=%o, line_layout=%p",a_line_tags, line_layout);
		if (a_line_tags) {
			cairo_save(surface_cr);


			/* draw curlies for tagged text */
			int text_left = update_ctx->line_nr_view_width + update_ctx->sub_line_height;
			cairo_translate(surface_cr, text_left, 0);
			cairo_rectangle(surface_cr, 0, 0, update_ctx->view_width-text_left, update_ctx->sub_line_height);
			cairo_clip(surface_cr);
			cairo_set_line_width(surface_cr, 1.5);
			CatIIterator *iter = cat_array_wo_iterator(a_line_tags);
			int start_idx, end_idx;
			DraTagType best_type = 1000;
			while(cat_iiterator_has_next(iter)) {
				DraLineTagWo *line_tag_wo = (DraLineTagWo *) cat_iiterator_next(iter);
				dra_line_tag_wo_get_start_and_end_index(line_tag_wo, &start_idx, &end_idx);
				if ((start_idx>=pango_line->start_index+pango_line->length) ||
						end_idx<=pango_line->start_index) {
					continue;
				}

				DraTagType ltt = dra_line_tag_wo_get_tag_type(line_tag_wo);
				if (ltt < best_type) {
					best_type = ltt;
				}

				double red, green, blue;
				dra_line_tag_wo_get_color(line_tag_wo, &red, &green, &blue);
				cairo_set_source_rgba(surface_cr, red, green, blue, 0.8);

				if (start_idx<pango_line->start_index) {
					start_idx = pango_line->start_index;
				}
				if (end_idx>pango_line->start_index+pango_line->length) {
					end_idx = pango_line->start_index+pango_line->length;
				}

				int x_left, x_right;
				pango_layout_line_index_to_x(pango_line, start_idx, FALSE, &x_left);
				pango_layout_line_index_to_x(pango_line, end_idx, FALSE, &x_right);
				cat_log_debug("start_idx=%d, end_idx=%d", start_idx, end_idx);

				x_left = x_left/PANGO_SCALE;
				x_right = x_right/PANGO_SCALE;
				l_draw_curlies(surface_cr, update_ctx->view_x, x_left, x_right-x_left, update_ctx->sub_line_height-3);
				cairo_stroke(surface_cr);
			}
			cat_unref_ptr(iter);
			cairo_restore(surface_cr);


			gboolean is_printer_mode = cha_cairo_surface_wrapper_get(surface_wrapper)==NULL;
			if (!is_printer_mode) {
				cairo_save(surface_cr);
				/* draw red bugs on the marker line */
				int font_height = update_ctx->sub_line_height;

				switch(best_type) {
					case DRA_TAG_TYPE_SPELL_ERROR : {
						cairo_set_source_rgb(surface_cr, 0.6f, 0.6f, 0.8f);
					} break;
					case DRA_TAG_TYPE_SCANNER_ERROR : {
						cairo_set_source_rgb(surface_cr, 1.0f, 0.8f, 0.3f);
					} break;
					default : {
						cairo_set_source_rgb(surface_cr, 1.0f, 0.0f, 0.0f);
					}
				}

				cairo_arc(surface_cr, update_ctx->line_nr_view_width+font_height/2, font_height/2, font_height*0.25, 0., 2 * 3.14159265359);
				cairo_fill(surface_cr);
				cairo_restore(surface_cr);
			}
		}
	}
	return result;
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
