/*
   File:    leasurfacebox.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 17, 2014
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

#ifndef LEASURFACEBOX_H_
#define LEASURFACEBOX_H_

#include "leairegion.h"
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

typedef enum {
	LEA_SB_HORIZONTAL,
	LEA_SB_VERTICAL
} LeaSurfaceBoxOrientation;


#define LEA_TYPE_SURFACE_BOX              (lea_surface_box_get_type())
#define LEA_SURFACE_BOX(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_surface_box_get_type(), LeaSurfaceBox))
#define LEA_SURFACE_BOX_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_SURFACE_BOX, LeaSurfaceBoxClass))
#define LEA_IS_SURFACE_BOX(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_SURFACE_BOX))
#define LEA_IS_SURFACE_BOX_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_SURFACE_BOX))
#define LEA_SURFACE_BOX_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_SURFACE_BOX, LeaSurfaceBoxClass))

typedef struct _LeaSurfaceBox               LeaSurfaceBox;
typedef struct _LeaSurfaceBoxPrivate        LeaSurfaceBoxPrivate;
typedef struct _LeaSurfaceBoxClass          LeaSurfaceBoxClass;


struct _LeaSurfaceBox {
	GObject parent;
};

struct _LeaSurfaceBoxClass {
	GObjectClass parent_class;
};

#define LEA_SB_PREFERED_ACCUM_WEIGHT	0x7ffff


GType lea_surface_box_get_type();

LeaSurfaceBox *lea_surface_box_new(LeaSurfaceBoxOrientation orientation);

void lea_surface_box_add(LeaSurfaceBox *surface_box, LeaIRegion *region, int index, int weight);

int lea_surface_box_entry_count(LeaSurfaceBox *surface_box);

LeaIRegion *lea_surface_box_get_region_at(LeaSurfaceBox *surface_box, int index);

int lea_surface_box_get_weight_at(LeaSurfaceBox *surface_box, int index);
void lea_surface_box_set_weight_at(LeaSurfaceBox *surface_box, int weight, int index);

LeaSurfaceBoxOrientation lea_surface_box_get_orientation(LeaSurfaceBox *surface_box);

int lea_surface_box_index_of(LeaSurfaceBox *surface_box, LeaIRegion *Region);
void lea_surface_box_remove_at(LeaSurfaceBox *surface_box, int index);
void lea_surface_box_replace_at(LeaSurfaceBox *surface_box, LeaIRegion *replacement, int index);
void lea_surface_box_merge_at(LeaSurfaceBox *surface_box, LeaSurfaceBox *box_to_merge, int index);


G_END_DECLS

#endif /* LEASURFACEBOX_H_ */
