/*
   File:    leabigdrawingpanel.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Dec 23, 2013
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

#ifndef LEABIGDRAWINGPANEL_H_
#define LEABIGDRAWINGPANEL_H_

#include "../../src/leafhopper.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_BIG_DRAWING_PANEL              (lea_big_drawing_panel_get_type())
#define LEA_BIG_DRAWING_PANEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_big_drawing_panel_get_type(), LeaBigDrawingPanel))
#define LEA_BIG_DRAWING_PANEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_BIG_DRAWING_PANEL, LeaBigDrawingPanelClass))
#define LEA_IS_BIG_DRAWING_PANEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_BIG_DRAWING_PANEL))
#define LEA_IS_BIG_DRAWING_PANEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_BIG_DRAWING_PANEL))
#define LEA_BIG_DRAWING_PANEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_BIG_DRAWING_PANEL, LeaBigDrawingPanelClass))
#define LEA_BIG_DRAWING_PANEL_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), LEA_TYPE_BIG_DRAWING_PANEL, LeaBigDrawingPanelPrivate))


typedef struct _LeaBigDrawingPanel               LeaBigDrawingPanel;
typedef struct _LeaBigDrawingPanelPrivate        LeaBigDrawingPanelPrivate;
typedef struct _LeaBigDrawingPanelClass          LeaBigDrawingPanelClass;


struct _LeaBigDrawingPanel {
	LeaPanel parent;
	LeaBigDrawingPanelPrivate *priv;
};

struct _LeaBigDrawingPanelClass {
	LeaPanelClass parent_class;
};


GType lea_big_drawing_panel_get_type();

LeaBigDrawingPanel *lea_big_drawing_panel_new();

G_END_DECLS

#endif /* LEABIGDRAWINGPANEL_H_ */
