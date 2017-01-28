/*
   File:    chamarking.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Mar 18, 2015
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

#ifndef CHAMARKING_H_
#define CHAMARKING_H_

#include "../preferences/chaprefscolormapwo.h"
#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define CHA_TYPE_MARKING              (cha_marking_get_type())
#define CHA_MARKING(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_marking_get_type(), ChaMarking))
#define CHA_MARKING_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_MARKING, ChaMarkingClass))
#define CHA_IS_MARKING(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_MARKING))
#define CHA_IS_MARKING_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_MARKING))
#define CHA_MARKING_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_MARKING, ChaMarkingClass))


typedef struct _ChaMarking               ChaMarking;
typedef struct _ChaMarkingPrivate        ChaMarkingPrivate;
typedef struct _ChaMarkingClass          ChaMarkingClass;


struct _ChaMarking {
	GObject parent;
};

struct _ChaMarkingClass {
	GObjectClass parent_class;
};


GType cha_marking_get_type();

ChaMarking *cha_marking_new(int sub_line_idx, int x_left, int x_right, double alpha, int color_index);

void cha_marking_set_mark_as_top(ChaMarking *marking);
void cha_marking_set_mark_as_bottom(ChaMarking *marking);

gboolean cha_marking_equal(ChaMarking *marking_a, ChaMarking *marking_b);

void cha_marking_paint(ChaMarking *marking, int sub_line_index, cairo_t *cairo, ChaPrefsColorMapWo *color_map, int height);

G_END_DECLS

#endif /* CHAMARKING_H_ */
