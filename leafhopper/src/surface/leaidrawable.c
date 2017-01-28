/*
   File:    leaidrawable.c
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

#include "leaidrawable.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaIDrawable"
#include <logging/catlog.h>

G_DEFINE_INTERFACE(LeaIDrawable, lea_idrawable, G_TYPE_OBJECT);

static void lea_idrawable_default_init (LeaIDrawableInterface *iface) {
	iface->draw = NULL;
}

void lea_idrawable_draw(LeaIDrawable *self, cairo_t *cairo) {
	g_return_if_fail(LEA_IS_IDRAWABLE(self));
	LEA_IDRAWABLE_GET_INTERFACE(self)->draw(self, cairo);
}

cairo_region_t *lea_idrawable_create_region(LeaIDrawable *self) {
	g_return_val_if_fail(LEA_IS_IDRAWABLE(self), NULL);
	return LEA_IDRAWABLE_GET_INTERFACE(self)->createRegion(self);
}
