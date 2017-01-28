/*
   File:    leairegion.h
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

#ifndef LEAIREGION_H_
#define LEAIREGION_H_

#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_IREGION                 (lea_iregion_get_type())
#define LEA_IREGION(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), LEA_TYPE_IREGION, LeaIRegion))
#define LEA_IS_IREGION(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), LEA_TYPE_IREGION))
#define LEA_IREGION_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), LEA_TYPE_IREGION, LeaIRegionInterface))

typedef struct _LeaIRegion               LeaIRegion; /* dummy object */
typedef struct _LeaIRegionInterface      LeaIRegionInterface;

struct _MooNodeWo;

struct _LeaIRegionInterface {
	GTypeInterface parent_iface;

	void (*sizeAllocate)(LeaIRegion *self, GtkAllocation *allocation);
	const GtkAllocation (*getAllocation)(LeaIRegion *self);
};

GType lea_iregion_get_type(void);

void lea_iregion_size_allocate(LeaIRegion *self, GtkAllocation *allocation);
const GtkAllocation lea_iregion_get_allocation(LeaIRegion *self);

G_END_DECLS

#endif /* LEAIREGION_H_ */
