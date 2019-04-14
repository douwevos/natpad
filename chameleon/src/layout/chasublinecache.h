/*
   File:    chasublinecache.h
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

#ifndef LAYOUT_CHASUBLINECACHE_H_
#define LAYOUT_CHASUBLINECACHE_H_

#include "chalinelayout.h"
#include "chasurfacepool.h"
#include "../preferences/chapreferenceswo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_SUB_LINE_CACHE              (cha_sub_line_cache_get_type())
#define CHA_SUB_LINE_CACHE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_sub_line_cache_get_type(), ChaSubLineCache))
#define CHA_SUB_LINE_CACHE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_SUB_LINE_CACHE, ChaSubLineCacheClass))
#define CHA_IS_SUB_LINE_CACHE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_SUB_LINE_CACHE))
#define CHA_IS_SUB_LINE_CACHE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_SUB_LINE_CACHE))
#define CHA_SUB_LINE_CACHE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_SUB_LINE_CACHE, ChaSubLineCacheClass))

typedef struct _ChaSubLineCacheUpdate ChaSubLineCacheUpdate;

struct _ChaSubLineCacheUpdate {
	GdkWindow *window;
	PangoLayout *pango_layout;
	ChaPreferencesWo *a_preferences;
	CatArrayWo *form_fields_on_line;
	int form_fields_hash;
	CatArrayWo *selection_markings;
	gboolean selection_eol;
	long long int page_sub_line_y;
	int view_x;
	int view_width;
	int sub_line_height;
	int attributes_version;
	int cursor_x_pos;
	int cursor_width;
	long long int line_nr;
	int line_nr_view_width;
	PangoLayout *scratch;
	gboolean paint_mark;
	gboolean show_whitespace;
	gboolean cache_dirty;
	gboolean highlight_current_line;
	ChaPrefsColorMapWo *color_map;
	gboolean line_ends_are_mixed;
	ChaLineEnd line_ends;
};


typedef struct _ChaSubLineCache               ChaSubLineCache;
typedef struct _ChaSubLineCachePrivate        ChaSubLineCachePrivate;
typedef struct _ChaSubLineCacheClass          ChaSubLineCacheClass;


struct _ChaSubLineCache {
	GObject parent;
};

struct _ChaSubLineCacheClass {
	GObjectClass parent_class;
	gboolean (*update)(ChaSubLineCache *sub_line_cache, ChaSubLineCacheUpdate *update_ctx);
	void (*draw)(ChaSubLineCache *sub_line_cache, cairo_t *cr, double clipy, double clipheight);
};

GType cha_sub_line_cache_get_type();

void cha_sub_line_cache_construct(ChaSubLineCache *sub_cache, ChaSurfacePool *surface_pool, ChaLineLayout *line_layout, int sub_line_index);

ChaSubLineCache *cha_sub_line_cache_new(ChaSurfacePool *surface_pool, ChaLineLayout *line_layout, int sub_line_index);

int cha_sub_line_cache_get_sub_line_index(ChaSubLineCache *sub_line_cache);

ChaLineLayout *cha_sub_line_cache_get_line_layout(ChaSubLineCache *sub_line_cache);

int cha_sub_line_cache_get_bottom(ChaSubLineCache *sub_line_cache);

gboolean cha_sub_line_cache_defined_by(ChaSubLineCache *sub_line_cache, ChaLineLayout *line_layout, int sub_line_index);

gboolean cha_sub_line_cache_update(ChaSubLineCache *sub_line_cache, ChaSubLineCacheUpdate *update_ctx);

void cha_sub_line_cache_draw(ChaSubLineCache *line_cache, cairo_t *cr, double clipy, double clipheight);

void cha_sub_line_cache_set_surface_wrapper(ChaSubLineCache *sub_line_cache, ChaCairoSurfaceWrapper *wrapper, int surface_width, int surface_height);

G_END_DECLS

#endif /* LAYOUT_CHASUBLINECACHE_H_ */
