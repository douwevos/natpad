/*
   File:    leairegion.c
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

#include "leairegion.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaIRegion"
#include <logging/catlog.h>

G_DEFINE_INTERFACE(LeaIRegion, lea_iregion, G_TYPE_OBJECT);

static void lea_iregion_default_init (LeaIRegionInterface *iface) {
	iface->sizeAllocate = NULL;
	iface->getAllocation = NULL;
}

void lea_iregion_size_allocate(LeaIRegion *self, GtkAllocation *allocation) {
	g_return_if_fail(LEA_IS_IREGION(self));
	LEA_IREGION_GET_INTERFACE(self)->sizeAllocate(self, allocation);
}

const GtkAllocation lea_iregion_get_allocation(LeaIRegion *self) {
	GtkAllocation allocation;
	g_return_val_if_fail(LEA_IS_IREGION(self), allocation);
	return LEA_IREGION_GET_INTERFACE(self)->getAllocation(self);
}


