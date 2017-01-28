/*
   File:    turviewer.h
   Project: turtle
   Author:  Douwe Vos
   Date:    Feb 20, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef TURVIEWER_H_
#define TURVIEWER_H_

#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define TUR_TYPE_VIEWER              (tur_viewer_get_type())
#define TUR_VIEWER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), tur_viewer_get_type(), TurViewer))
#define TUR_VIEWER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TUR_TYPE_VIEWER, TurViewerClass))
#define TUR_IS_VIEWER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TUR_TYPE_VIEWER))
#define TUR_IS_VIEWER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TUR_TYPE_VIEWER))
#define TUR_VIEWER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TUR_TYPE_VIEWER, TurViewerClass))


typedef struct _TurViewer               TurViewer;
typedef struct _TurViewerPrivate        TurViewerPrivate;
typedef struct _TurViewerClass          TurViewerClass;


struct _TurViewer {
	GtkWidget parent;
};

struct _TurViewerClass {
	GtkWidgetClass parent_class;
};

GType tur_viewer_get_type();

TurViewer *tur_viewer_new();

G_END_DECLS

#endif /* TURVIEWER_H_ */
