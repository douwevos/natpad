/*
   File:    leagripshift.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 18, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef LEAGRIPSHIFT_H_
#define LEAGRIPSHIFT_H_

#include "../leasurfacebox.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_GRIP_SHIFT              (lea_grip_shift_get_type())
#define LEA_GRIP_SHIFT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_grip_shift_get_type(), LeaGripShift))
#define LEA_GRIP_SHIFT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_GRIP_SHIFT, LeaGripShiftClass))
#define LEA_IS_GRIP_SHIFT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_GRIP_SHIFT))
#define LEA_IS_GRIP_SHIFT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_GRIP_SHIFT))
#define LEA_GRIP_SHIFT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_GRIP_SHIFT, LeaGripShiftClass))


typedef struct _LeaGripShift               LeaGripShift;
typedef struct _LeaGripShiftPrivate        LeaGripShiftPrivate;
typedef struct _LeaGripShiftClass          LeaGripShiftClass;


struct _LeaGripShift {
	GObject parent;
};

struct _LeaGripShiftClass {
	GObjectClass parent_class;
};


GType lea_grip_shift_get_type();

LeaGripShift *lea_grip_shift_new(LeaSurfaceBox *surface_box, int entry_index, int delta_left, int delta_top);

G_END_DECLS

#endif /* LEAGRIPSHIFT_H_ */
