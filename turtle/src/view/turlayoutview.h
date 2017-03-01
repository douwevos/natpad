/*
   File:    turlayoutview.h
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

#ifndef VIEW_TURLAYOUTVIEW_H_
#define VIEW_TURLAYOUTVIEW_H_

#include "../model/turlayout.h"
#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define TUR_TYPE_LAYOUT_VIEW              (tur_layout_view_get_type())
#define TUR_LAYOUT_VIEW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), tur_layout_view_get_type(), TurLayoutView))
#define TUR_LAYOUT_VIEW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TUR_TYPE_LAYOUT_VIEW, TurLayoutViewClass))
#define TUR_IS_LAYOUT_VIEW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TUR_TYPE_LAYOUT_VIEW))
#define TUR_IS_LAYOUT_VIEW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TUR_TYPE_LAYOUT_VIEW))
#define TUR_LAYOUT_VIEW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TUR_TYPE_LAYOUT_VIEW, TurLayoutViewClass))


typedef struct _TurLayoutView               TurLayoutView;
typedef struct _TurLayoutViewPrivate        TurLayoutViewPrivate;
typedef struct _TurLayoutViewClass          TurLayoutViewClass;


struct _TurLayoutView {
	GObject parent;
};

struct _TurLayoutViewClass {
	GObjectClass parent_class;
};


GType tur_layout_view_get_type();

TurLayoutView *tur_layout_view_new(GtkWidget *widget, PangoContext *pango_context, TurLayout *layout);

void tur_layout_view_set_view_size(TurLayoutView *view, int view_width, int view_height);

void tur_layout_view_draw(TurLayoutView *view, cairo_t *cr);

void tur_layout_view_invalidate(TurLayoutView *view);

G_END_DECLS

#endif /* VIEW_TURLAYOUTVIEW_H_ */
