/*
   File:    leaidrawable.h
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

#ifndef LEAIDRAWABLE_H_
#define LEAIDRAWABLE_H_

#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_IDRAWABLE                 (lea_idrawable_get_type())
#define LEA_IDRAWABLE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), LEA_TYPE_IDRAWABLE, LeaIDrawable))
#define LEA_IS_IDRAWABLE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), LEA_TYPE_IDRAWABLE))
#define LEA_IDRAWABLE_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), LEA_TYPE_IDRAWABLE, LeaIDrawableInterface))

typedef struct _LeaIDrawable               LeaIDrawable; /* dummy object */
typedef struct _LeaIDrawableInterface      LeaIDrawableInterface;

struct _MooNodeWo;

struct _LeaIDrawableInterface {
	GTypeInterface parent_iface;

	void (*draw)(LeaIDrawable *self, cairo_t *cairo);
	cairo_region_t *(*createRegion)(LeaIDrawable *self);
};

GType lea_idrawable_get_type(void);

void lea_idrawable_draw(LeaIDrawable *self, cairo_t *cairo);
cairo_region_t *lea_idrawable_create_region(LeaIDrawable *self);

G_END_DECLS

#endif /* LEAIDRAWABLE_H_ */
