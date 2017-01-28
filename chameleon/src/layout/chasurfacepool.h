/*
   File:    chasurfacepool.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Mar 13, 2015
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

#ifndef CHASURFACEPOOL_H_
#define CHASURFACEPOOL_H_

#include "chacairosurfacewrapper.h"
#include <caterpillar.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define CHA_TYPE_SURFACE_POOL              (cha_surface_pool_get_type())
#define CHA_SURFACE_POOL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_surface_pool_get_type(), ChaSurfacePool))
#define CHA_SURFACE_POOL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_SURFACE_POOL, ChaSurfacePoolClass))
#define CHA_IS_SURFACE_POOL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_SURFACE_POOL))
#define CHA_IS_SURFACE_POOL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_SURFACE_POOL))
#define CHA_SURFACE_POOL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_SURFACE_POOL, ChaSurfacePoolClass))


typedef struct _ChaSurfacePool               ChaSurfacePool;
typedef struct _ChaSurfacePoolPrivate        ChaSurfacePoolPrivate;
typedef struct _ChaSurfacePoolClass          ChaSurfacePoolClass;


struct _ChaSurfacePool {
	GObject parent;
};

struct _ChaSurfacePoolClass {
	GObjectClass parent_class;
};


GType cha_surface_pool_get_type();

ChaSurfacePool *cha_surface_pool_new();


ChaCairoSurfaceWrapper *cha_surface_pool_obtain(ChaSurfacePool *pool, GdkWindow *window);

void cha_surface_pool_release(ChaSurfacePool *pool, ChaCairoSurfaceWrapper *surface);

void cha_surface_pool_set_image_dimension(ChaSurfacePool *pool, int width, int height);

void cha_surface_pool_set_image_height(ChaSurfacePool *pool, int height);
void cha_surface_pool_set_image_width(ChaSurfacePool *pool, int width);

G_END_DECLS

#endif /* CHASURFACEPOOL_H_ */
