/*
   File:    chasublinecache.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Mar 4, 2015
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

#include "chasublinecacheprivate.h"
#include "../document/chaformwo.h"
#include "../document/chaformfieldwo.h"
#include "chamarking.h"
#include <math.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaSubLineCache"
#include <logging/catlog.h>

struct _ChaSubLineCachePrivate {
	ChaSurfacePool *surface_pool;
	ChaLineLayout *line_layout;
	int sub_line_index;
	long long int last_drawn_ypos;
	int view_x;
	PangoLayout *pango_layout;
	ChaCairoSurfaceWrapper *surface;
	int surface_width;
	int surface_height;
	int attributes_version;
	int form_hash;
	gboolean mark_painted;
	gboolean show_whitespace;
	gboolean current_line_highlighted;
	int cursor_x_pos;
	int cursor_width;
	long long int line_nr;
	int line_nr_view_width;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaSubLineCache, cha_sub_line_cache, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaSubLineCache)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_update(ChaSubLineCache *sub_line_cache, ChaSubLineCacheUpdate *update_ctx);
static void l_draw(ChaSubLineCache *sub_line_cache, cairo_t *cr, double clipy, double clipheight);

static void cha_sub_line_cache_class_init(ChaSubLineCacheClass *clazz) {
	clazz->update = l_update;
	clazz->draw = l_draw;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_sub_line_cache_init(ChaSubLineCache *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaSubLineCache *instance = CHA_SUB_LINE_CACHE(object);
	ChaSubLineCachePrivate *priv = cha_sub_line_cache_get_instance_private(instance);
	cat_unref_ptr(priv->pango_layout);
	if (priv->surface && priv->surface_pool) {
		cha_surface_pool_release(priv->surface_pool, priv->surface);
	}
	cat_unref_ptr(priv->surface);
	cat_unref_ptr(priv->surface_pool);
	if (priv->line_layout) {
		cha_line_layout_release(priv->line_layout);
		cat_unref_ptr(priv->line_layout);
	}
	G_OBJECT_CLASS(cha_sub_line_cache_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_sub_line_cache_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void cha_sub_line_cache_construct(ChaSubLineCache *sub_cache, ChaSurfacePool *surface_pool, ChaLineLayout *line_layout, int sub_line_index) {
	ChaSubLineCachePrivate *priv = cha_sub_line_cache_get_instance_private(sub_cache);
	priv->surface_pool = cat_ref_ptr(surface_pool);
	priv->line_layout = cat_ref_ptr(line_layout);
	priv->sub_line_index = sub_line_index;
	priv->last_drawn_ypos = -1;
	priv->view_x = -1;
	priv->pango_layout = NULL;
	priv->surface = NULL;
	priv->surface_width = -1;
	priv->form_hash = 0;
	priv->surface_height = -1;
	priv->line_nr_view_width = 0;
	priv->line_nr = 0;
	priv->show_whitespace = FALSE;
	priv->cursor_width = 0;
	cha_line_layout_hold(line_layout);
}


ChaSubLineCache *cha_sub_line_cache_new(ChaSurfacePool *surface_pool, ChaLineLayout *line_layout, int sub_line_index) {
	ChaSubLineCache *result = g_object_new(CHA_TYPE_SUB_LINE_CACHE, NULL);
	cat_ref_anounce(result);
	cha_sub_line_cache_construct(result, surface_pool, line_layout, sub_line_index);
	return result;
}

ChaCairoSurfaceWrapper *cha_sub_line_cache_get_surface(ChaSubLineCache *sub_line_cache) {
	ChaSubLineCachePrivate *priv = cha_sub_line_cache_get_instance_private(sub_line_cache);
	return priv->surface;
}

ChaLineLayout *cha_sub_line_cache_get_line_layout(ChaSubLineCache *sub_line_cache) {
	ChaSubLineCachePrivate *priv = cha_sub_line_cache_get_instance_private(sub_line_cache);
	return priv->line_layout;
}


int cha_sub_line_cache_get_bottom(ChaSubLineCache *sub_line_cache) {
	ChaSubLineCachePrivate *priv = cha_sub_line_cache_get_instance_private(sub_line_cache);
	return priv->last_drawn_ypos + priv->surface_height;
}


int cha_sub_line_cache_get_sub_line_index(ChaSubLineCache *sub_line_cache) {
	ChaSubLineCachePrivate *priv = cha_sub_line_cache_get_instance_private(sub_line_cache);
	return priv->sub_line_index;
}


gboolean cha_sub_line_cache_defined_by(ChaSubLineCache *sub_line_cache, ChaLineLayout *line_layout, int sub_line_index) {
	ChaSubLineCachePrivate *priv = cha_sub_line_cache_get_instance_private(sub_line_cache);
	return priv->line_layout==line_layout && priv->sub_line_index==sub_line_index;
}

gboolean cha_sub_line_cache_update(ChaSubLineCache *sub_line_cache, ChaSubLineCacheUpdate *update_ctx) {
	update_ctx->cache_dirty = FALSE;
	return CHA_SUB_LINE_CACHE_GET_CLASS(sub_line_cache)->update(sub_line_cache, update_ctx);
}

void cha_sub_line_cache_set_surface_wrapper(ChaSubLineCache *sub_line_cache, ChaCairoSurfaceWrapper *wrapper, int surface_width, int surface_height) {
	ChaSubLineCachePrivate *priv = cha_sub_line_cache_get_instance_private(sub_line_cache);
	cat_ref_swap(priv->surface, wrapper);
	priv->surface_width = surface_width;
	priv->surface_height = surface_height;
}


static void l_rounded_rect(cairo_t *cr, double x, double y, double width, double height) {
	double aspect = 1.0,     /* aspect ratio */
	       corner_radius = height / 10.0;   /* and corner curvature radius */

	double radius = corner_radius / aspect;
	double degrees = M_PI / 180.0;

	cairo_new_sub_path (cr);
	cairo_arc (cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
	cairo_arc (cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
	cairo_arc (cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
	cairo_arc (cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
	cairo_close_path (cr);

	cairo_fill(cr);
}


static void l_rounded_rect2(cairo_t *cr, double x, double y, double width, double height) {
	double aspect = 0.6;     /* aspect ratio */
	double corner_radius = height / 5.0;   /* and corner curvature radius */

	double radius = corner_radius / aspect;
	double degrees = M_PI / 180.0;

	cairo_new_sub_path (cr);
	cairo_arc (cr, x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
	cairo_arc (cr, x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
	cairo_arc (cr, x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
	cairo_arc (cr, x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
	cairo_close_path (cr);

	cairo_fill(cr);
}


static int l_draw_eol(cairo_t *cairo, ChaSubLineCacheUpdate *update_ctx, int x_left, const char *txt) {
	ChaPrefsColorMapWo *color_map = update_ctx->color_map;
	pango_layout_set_text(update_ctx->scratch, txt, 2);

	PangoRectangle log_rect;
	pango_layout_get_extents(update_ctx->scratch, NULL, &log_rect);

	int xa = x_left;
	int xb = x_left + log_rect.width/PANGO_SCALE + 6;

	cha_prefs_color_map_wo_set_cairo_color(color_map, cairo, 15);
	l_rounded_rect2(cairo, 1+x_left-update_ctx->view_x,1, xb - 1 - xa, update_ctx->sub_line_height-2);
	cha_prefs_color_map_wo_set_cairo_color(color_map, cairo, 0);
	cairo_move_to(cairo, x_left+3-update_ctx->view_x, 0);
	pango_cairo_show_layout(cairo, update_ctx->scratch);
	return xb;

}

static gboolean l_update(ChaSubLineCache *sub_line_cache, ChaSubLineCacheUpdate *update_ctx) {
	ChaSubLineCachePrivate *priv = cha_sub_line_cache_get_instance_private(sub_line_cache);

	gboolean result = FALSE;
	if (priv->pango_layout!=update_ctx->pango_layout) {
		cat_ref_swap(priv->pango_layout, update_ctx->pango_layout);
		result = TRUE;
		update_ctx->cache_dirty = TRUE;
	}

	if (priv->view_x != update_ctx->view_x) {
		priv->view_x = update_ctx->view_x;
		update_ctx->cache_dirty = TRUE;
	}

	if (priv->mark_painted != update_ctx->paint_mark) {
		priv->mark_painted = update_ctx->paint_mark;
		update_ctx->cache_dirty = TRUE;
	}

	if (priv->show_whitespace != update_ctx->show_whitespace) {
		priv->show_whitespace = update_ctx->show_whitespace;
		update_ctx->cache_dirty = TRUE;
	}


	if (priv->attributes_version != update_ctx->attributes_version) {
		priv->attributes_version = update_ctx->attributes_version;
		cat_log_debug("%p new attributes", sub_line_cache);
		update_ctx->cache_dirty = TRUE;
	}

	if (priv->form_hash != update_ctx->form_fields_hash) {
		priv->form_hash = update_ctx->form_fields_hash;
		cat_log_debug("%p form fields", sub_line_cache);
		update_ctx->cache_dirty = TRUE;
	}


	if (priv->surface) {
		if ((priv->surface_width!=update_ctx->view_width)
				|| (priv->surface_height!=update_ctx->sub_line_height)) {
			cat_unref_ptr(priv->surface);
		}
	}

	if (priv->surface==NULL) {
		priv->surface = cha_surface_pool_obtain(priv->surface_pool, update_ctx->window);
		priv->surface_width = update_ctx->view_width;
		priv->surface_height = update_ctx->sub_line_height;


//		priv->cairo_surface = cairo_image_surface_create(CAIRO_FORMAT_RGB24, view_width, sub_line_height);

		cat_log_debug("new surface=%d,%d sub-line-idx=%d", update_ctx->view_width, update_ctx->sub_line_height, priv->sub_line_index);
		update_ctx->cache_dirty = TRUE;
	}

	cat_log_trace("priv->pango_layout=%p, sub_line_index=%d", priv->pango_layout, priv->sub_line_index);



	PangoLayoutLine *pango_line = pango_layout_get_line_readonly(priv->pango_layout, priv->sub_line_index);

	if ((update_ctx->cursor_x_pos!=priv->cursor_x_pos) || (update_ctx->cursor_width!=priv->cursor_width)) {
		priv->cursor_x_pos = update_ctx->cursor_x_pos;
		priv->cursor_width = update_ctx->cursor_width;
//		update_ctx->cache_dirty = TRUE;
		result = TRUE;
	}

	gboolean line_high = update_ctx->highlight_current_line && priv->cursor_x_pos>=0;
	if (priv->current_line_highlighted!=line_high) {
		priv->current_line_highlighted = line_high;
		update_ctx->cache_dirty = TRUE;
		result = TRUE;
	}

	if (priv->line_nr_view_width != update_ctx->line_nr_view_width) {
		update_ctx->cache_dirty = TRUE;
	} else if (update_ctx->line_nr_view_width>0 && priv->line_nr!=update_ctx->line_nr) {
		update_ctx->cache_dirty = TRUE;
	}

	priv->line_nr_view_width = update_ctx->line_nr_view_width;
	priv->line_nr = update_ctx->line_nr;


	if (update_ctx->cache_dirty) {
		ChaPrefsColorMapWo *color_map = update_ctx->color_map;
		cat_log_debug("cache-dirty:%d priv->surface_height=%d", priv->line_nr, priv->surface_height);
		cairo_t *cairo = cha_cairo_surface_wrapper_get_context(priv->surface);
		gboolean is_printer_mode = cha_cairo_surface_wrapper_get(priv->surface)==NULL;

		cat_log_trace("%p cache dirty, pango_layout->width=%d", sub_line_cache, pango_layout_get_width(priv->pango_layout));
		int baseline = (pango_layout_get_baseline(priv->pango_layout))/PANGO_SCALE;
		int text_left = priv->line_nr_view_width;
		if (!is_printer_mode) {
			/* line-markings are hidden in printer mode */
			text_left += update_ctx->sub_line_height;
		}

		cairo_save(cairo);

		/* draw line number */
		cairo_set_source_rgb(cairo, 0.9,0.9,0.9);
		cairo_rectangle(cairo, 0, 0, text_left, update_ctx->sub_line_height);
		cairo_fill(cairo);
		if (priv->line_nr_view_width>0) {
			if (priv->line_nr>=0 && priv->sub_line_index==0) {
				CatStringWo *buf = cat_string_wo_new();
				cat_string_wo_append_decimal(buf, priv->line_nr+1);
				cat_log_debug("text=%o", buf);

				pango_layout_set_text(update_ctx->scratch, cat_string_wo_getchars(buf), cat_string_wo_length(buf));
				PangoRectangle log_rect;
				pango_layout_get_extents(update_ctx->scratch, NULL, &log_rect);
				cairo_move_to(cairo, priv->line_nr_view_width-log_rect.width/PANGO_SCALE, 0);
				cairo_set_source_rgb(cairo, 0.4,0.4,0.4);
				cairo_set_source_rgb(cairo, 0,0,0);
				pango_cairo_show_layout(cairo, update_ctx->scratch);
				cat_unref_ptr(buf);
			}
		}

		/* draw line marking */
		if ((!is_printer_mode) && update_ctx->paint_mark) {
			int font_height = update_ctx->sub_line_height;
			int view_mark_width = 4 + update_ctx->sub_line_height/2;

			cairo_set_source_rgb(cairo, 0.0f, 0.0f, 0.60f);
			cairo_new_path(cairo);
		//	cairo_move_to(painter->back_cairo, 60, 0);
			cairo_move_to(cairo, priv->line_nr_view_width+2, 3);
			cairo_line_to(cairo, priv->line_nr_view_width+view_mark_width-2, font_height/2);
			cairo_line_to(cairo, priv->line_nr_view_width+2, font_height-3);
			cairo_close_path(cairo);
			cairo_fill(cairo);
		}


		/* prepare for text area */
		cairo_translate(cairo, text_left, 0);
		cairo_rectangle(cairo, 0, 0, update_ctx->view_width-text_left, update_ctx->sub_line_height);
		if (update_ctx->highlight_current_line && (priv->cursor_x_pos>=0)) {
			cha_prefs_color_map_wo_set_cairo_color(color_map, cairo, CHA_COLOR_CURRENT_LINE);
		} else {
			cha_prefs_color_map_wo_set_cairo_color(color_map, cairo, CHA_COLOR_BACKGROUND);
		}
		cairo_fill_preserve(cairo);
		cairo_clip(cairo);



		/* draw EOL selection */
		gboolean is_last_sub_line = pango_layout_get_line_count(priv->pango_layout)==priv->sub_line_index+1;
		if (!is_printer_mode && is_last_sub_line && update_ctx->selection_eol) {
			int x_left;
			pango_layout_line_index_to_x(pango_line, pango_line->start_index+pango_line->length-1, TRUE, &x_left);
			x_left = (x_left/PANGO_SCALE) - update_ctx->view_x;
			cha_prefs_color_map_wo_set_cairo_color(color_map, cairo, 1);
			cairo_rectangle(cairo, x_left, 0, update_ctx->sub_line_height/3, update_ctx->sub_line_height);
			cairo_fill(cairo);
		}


		/* draw text*/
		cairo_move_to(cairo, -update_ctx->view_x, baseline);

		pango_cairo_show_layout_line(cairo, pango_line);

		if (update_ctx->show_whitespace) {
			const char *text = pango_layout_get_text(priv->pango_layout);
			int ch_idx = pango_line->start_index;
			int ch_left = pango_line->length;
			while(ch_left-- >0) {
				char ch = text[ch_idx];
				if (ch==' ' || ch=='\t') {
					int x_left, x_right;
					pango_layout_line_index_to_x(pango_line, ch_idx, FALSE, &x_left);
					pango_layout_line_index_to_x(pango_line, ch_idx, TRUE, &x_right);
					if (x_left!=x_right) {
						x_left = x_left/PANGO_SCALE;
						x_right = x_right/PANGO_SCALE;
						int x_ws = x_left;
						if (ch=='\t') {
							pango_layout_set_text(update_ctx->scratch, "\xc2\xbb", 2);
							PangoRectangle log_rect;
							pango_layout_get_extents(update_ctx->scratch, NULL, &log_rect);
							x_ws = (x_left+x_right - log_rect.width/PANGO_SCALE)/2;
						} else {
							pango_layout_set_text(update_ctx->scratch, "\xc2\xb7", 2);
						}

						cairo_save(cairo);
						cairo_rectangle(cairo, x_left,0,x_right-x_left, update_ctx->sub_line_height);
						cairo_clip(cairo);
						cairo_move_to(cairo, x_ws-update_ctx->view_x, 0);
						cha_prefs_color_map_wo_set_cairo_color(color_map, cairo, 15);
						pango_cairo_show_layout(cairo, update_ctx->scratch);
						cairo_restore(cairo);
					}
				}
				ch_idx++;
			}

			/* show EOL */
			if (is_last_sub_line) {
				cairo_save(cairo);

				int x_left;
				pango_layout_line_index_to_x(pango_line, pango_line->start_index+pango_line->length-1, TRUE, &x_left);
				x_left = x_left/PANGO_SCALE;

				ChaLineEnd line_end = cha_line_layout_get_line_end(priv->line_layout);
				if (line_end==CHA_LINE_END_NL) {
					x_left = l_draw_eol(cairo, update_ctx, x_left, "NL");
				}
				if (line_end==CHA_LINE_END_CR || line_end==CHA_LINE_END_CRLF) {
					x_left = l_draw_eol(cairo, update_ctx, x_left, "CR");
				}
				if (line_end==CHA_LINE_END_LF || line_end==CHA_LINE_END_CRLF || line_end==CHA_LINE_END_LFCR) {
					x_left = l_draw_eol(cairo, update_ctx, x_left, "LF");
				}
				if (line_end==CHA_LINE_END_LFCR) {
					x_left = l_draw_eol(cairo, update_ctx, x_left, "CR");
				}

				cairo_restore(cairo);

			}
		}

		cairo_translate(cairo, -priv->view_x,0);
		if (update_ctx->selection_markings) {
			CatIIterator *iter = cat_array_wo_iterator(update_ctx->selection_markings);
			cairo_set_line_width(cairo, 0.5);
			while(cat_iiterator_has_next(iter)) {
				ChaMarking *marking = (ChaMarking *) cat_iiterator_next(iter);

				cha_marking_paint(marking, priv->sub_line_index, cairo, color_map, update_ctx->sub_line_height);
			}
			cat_unref_ptr(iter);
		}


		if (update_ctx->form_fields_on_line) {
			long long pl_row = update_ctx->line_nr;
			int pl_left = pango_line->start_index;
			int pl_right = pl_left + pango_line->length;
			CatIIterator *iter = cat_array_wo_iterator(update_ctx->form_fields_on_line);
			while(cat_iiterator_has_next(iter)) {
				ChaFormFieldWo *field = (ChaFormFieldWo *) cat_iiterator_next(iter);
				ChaCursorMWo *s = cha_form_field_wo_get_start(field);
				int left = cha_cursorm_wo_get_column(s);
				long long top = cha_cursorm_wo_get_row(s);
				s = cha_form_field_wo_get_end(field);
				int right = cha_cursorm_wo_get_column(s);
				long long bottom = cha_cursorm_wo_get_row(s);
				cat_log_detail("field=%o, left=%d, right=%d, pl_left=%d, pl_right=%d, pl_row=%d, top=%d, bottom=%d", field, left, right, pl_left, pl_right, (int) pl_row, (int) top, (int) bottom);
				if ((pl_row<top) || (pl_row>bottom)) {
					continue;
				}

				gboolean is_top = pl_row==top;

				if ((!is_top) || (pl_left>left)) {
					left = pl_left;
				}

				gboolean is_bottom = pl_row==bottom;
				if ((!is_bottom) || (pl_right<right)) {
					right = pl_right;
				}

				int cursor_sub_line_idx;
				int left_vx = 0;
				int right_vx = 0;
				if (left>=pl_left) {
					pango_layout_index_to_line_x(priv->pango_layout, left, FALSE, &cursor_sub_line_idx, &left_vx);
				} else {
					left_vx = 0;
				}

				if (right<pl_right) {
					pango_layout_index_to_line_x(priv->pango_layout, right, FALSE, &cursor_sub_line_idx, &right_vx);
				} else {
					pango_layout_index_to_line_x(priv->pango_layout, pl_right, FALSE, &cursor_sub_line_idx, &right_vx);
				}

				left_vx = left_vx/PANGO_SCALE;
				right_vx = right_vx/PANGO_SCALE;

				int font_height = update_ctx->sub_line_height;

				/* form top */
				double gfx_top = (double) ((is_top ? 2 : 0));
				double gfx_bottom = (double) (font_height - (is_bottom ? 2 : 0));


				cairo_set_source_rgba(cairo, 0.8, 0.4, 0.1, 0.25);
				l_rounded_rect(cairo, left_vx,gfx_top, right_vx-left_vx, gfx_bottom-gfx_top);
			}
		}

		cairo_restore(cairo);

		result = TRUE;
	}

	if (priv->last_drawn_ypos != update_ctx->page_sub_line_y) {
		priv->last_drawn_ypos = update_ctx->page_sub_line_y;
		result = TRUE;
	}

	return result;
}

void cha_sub_line_cache_draw(ChaSubLineCache *sub_line_cache, cairo_t *cr, double clipy, double clipheight) {
	CHA_SUB_LINE_CACHE_GET_CLASS(sub_line_cache)->draw(sub_line_cache, cr, clipy, clipheight);
}

static void l_draw(ChaSubLineCache *sub_line_cache, cairo_t *cr, double clipy, double clipheight) {
	ChaSubLineCachePrivate *priv = cha_sub_line_cache_get_instance_private(sub_line_cache);
//	if (priv->last_drawn_ypos>clipy+clipheight) {
//		return;
//	}
	int height = priv->surface_height;
//	if (priv->last_drawn_ypos+height<clipy) {
//		return;
//	}
	cat_log_debug("drawing at %d, cursor_x_pos=%d, height=%d", (int) (priv->last_drawn_ypos), priv->cursor_x_pos, height);
	cairo_save(cr);
	cairo_surface_t *surface = cha_cairo_surface_wrapper_get(priv->surface);
	if (surface) {
		cairo_set_source_surface(cr, cha_cairo_surface_wrapper_get(priv->surface), 0, (double) priv->last_drawn_ypos);
		cairo_rectangle(cr, 0, (double) priv->last_drawn_ypos, priv->surface_width, height);
		cairo_fill(cr);
	}
//	cairo_paint(cr);


	if (priv->cursor_x_pos>=0) {
		cat_log_debug("drawing cursor at:%d", priv->cursor_x_pos);
		if (priv->cursor_width>0) {
			/* draw cursor overwrite mode */

			cairo_set_operator(cr, CAIRO_OPERATOR_DIFFERENCE);
			cairo_set_source_rgba(cr, 1.0, 1.0, 1.0, 1.0);
			cairo_set_line_width(cr, 0.5);
			int text_left = height;
			if (priv->line_nr_view_width>0) {
				text_left += priv->line_nr_view_width;
			}
			cairo_rectangle(cr, text_left, priv->last_drawn_ypos-2, priv->surface_width-text_left, height+4);
			cairo_clip(cr);

			cairo_rectangle(cr, priv->cursor_x_pos+text_left  - priv->view_x, priv->last_drawn_ypos, priv->cursor_width-1, height-1);
			cairo_fill(cr);

		} else if (priv->cursor_width==0) {
			/* draw cursor insert mode */
			cairo_set_line_width(cr, 2);
			cairo_set_source_rgba(cr, 0.2, 0.2, 0.4, 0.7);
			int text_left = height;
			if (priv->line_nr_view_width>0) {
				text_left += priv->line_nr_view_width;
			}
			cairo_rectangle(cr, text_left, priv->last_drawn_ypos-2, priv->surface_width-text_left, height+4);
			cairo_clip(cr);

			cairo_move_to(cr, priv->cursor_x_pos+1+text_left - priv->view_x, priv->last_drawn_ypos);
			cairo_line_to(cr, priv->cursor_x_pos+1+text_left - priv->view_x, priv->last_drawn_ypos+height);
			cairo_stroke(cr);
		}
	}
	cairo_restore(cr);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *e_append_to) {
	ChaSubLineCache *instance = CHA_SUB_LINE_CACHE(self);
	ChaSubLineCachePrivate *priv = cha_sub_line_cache_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(e_append_to, "%s[%p, sub_line_index=%d, ypos=%d, surf=%p]", iname, self, priv->sub_line_index, (int) (priv->last_drawn_ypos), priv->surface);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

