/*
   File:    turlayout.h
   Project: turtle
   Author:  Douwe Vos
   Date:    Feb 12, 2017
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

#ifndef LAYOUT_TURLAYOUT_H_
#define LAYOUT_TURLAYOUT_H_

#include "turline.h"
#include "../model/turinode.h"
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define TUR_TYPE_LAYOUT              (tur_layout_get_type())
#define TUR_LAYOUT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), tur_layout_get_type(), TurLayout))
#define TUR_LAYOUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TUR_TYPE_LAYOUT, TurLayoutClass))
#define TUR_IS_LAYOUT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TUR_TYPE_LAYOUT))
#define TUR_IS_LAYOUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TUR_TYPE_LAYOUT))
#define TUR_LAYOUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TUR_TYPE_LAYOUT, TurLayoutClass))


typedef struct _TurLayout               TurLayout;
typedef struct _TurLayoutPrivate        TurLayoutPrivate;
typedef struct _TurLayoutClass          TurLayoutClass;


struct _TurLayout {
	GObject parent;
};

struct _TurLayoutClass {
	GObjectClass parent_class;
};


GType tur_layout_get_type();

TurLayout *tur_layout_new(TurINode *root_node);

TurLine * tur_layout_get_root_line(TurLayout *layout);

G_END_DECLS

#endif /* LAYOUT_TURLAYOUT_H_ */
