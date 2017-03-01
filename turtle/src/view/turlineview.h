/*
   File:    turlineview.h
   Project: turtle
   Author:  Douwe Vos
   Date:    Feb 14, 2017
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

#ifndef VIEW_TURLINEVIEW_H_
#define VIEW_TURLINEVIEW_H_

#include "tursurfacepool.h"
#include "../model/turline.h"
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS


typedef struct _TurLineViewUpdate TurLineViewUpdate;

struct _TurLineViewUpdate {
	TurSurfacePool *surface_pool;
	int view_width, view_height;
	int line_height;
	GdkWindow *window;
	PangoLayout *pango_layout;
	long long int y_pos;
};



#define TUR_TYPE_LINE_VIEW              (tur_line_view_get_type())
#define TUR_LINE_VIEW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), tur_line_view_get_type(), TurLineView))
#define TUR_LINE_VIEW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TUR_TYPE_LINE_VIEW, TurLineViewClass))
#define TUR_IS_LINE_VIEW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TUR_TYPE_LINE_VIEW))
#define TUR_IS_LINE_VIEW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TUR_TYPE_LINE_VIEW))
#define TUR_LINE_VIEW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TUR_TYPE_LINE_VIEW, TurLineViewClass))


typedef struct _TurLineView               TurLineView;
typedef struct _TurLineViewPrivate        TurLineViewPrivate;
typedef struct _TurLineViewClass          TurLineViewClass;


struct _TurLineView {
	GObject parent;
};

struct _TurLineViewClass {
	GObjectClass parent_class;
};


GType tur_line_view_get_type();

TurLineView *tur_line_view_new(TurLine *line);

gboolean tur_line_view_defined_by(TurLineView *line_view, TurLine *line);

gboolean tur_line_view_update(TurLineView *line_view, const TurLineViewUpdate update);

void tur_line_view_draw(TurLineView *line_view, cairo_t *cr);

G_END_DECLS

#endif /* VIEW_TURLINEVIEW_H_ */
