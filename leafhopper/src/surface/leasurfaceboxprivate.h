/*
   File:    leasurfaceboxprivate.h
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

#ifndef LEASURFACEBOXPRIVATE_H_
#define LEASURFACEBOXPRIVATE_H_

#include "leasurfacebox.h"
#include "drag/leaigrip.h"

LeaIGrip *lea_surf_box_calculate_locator(LeaSurfaceBox *surface_box, int x, int y, int drag_magnet, LeaIGrip *top_grip);

#endif /* LEASURFACEBOXPRIVATE_H_ */
