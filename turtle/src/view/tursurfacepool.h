/*
   File:    tursurfacepool.h
   Project: turtle
   Author:  Douwe Vos
   Date:    Feb 14, 2017
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

#ifndef VIEW_TURSURFACEPOOL_H_
#define VIEW_TURSURFACEPOOL_H_

#include "tursurfacewrapper.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define TUR_TYPE_SURFACE_POOL              (tur_surface_pool_get_type())
#define TUR_SURFACE_POOL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), tur_surface_pool_get_type(), TurSurfacePool))
#define TUR_SURFACE_POOL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TUR_TYPE_SURFACE_POOL, TurSurfacePoolClass))
#define TUR_IS_SURFACE_POOL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TUR_TYPE_SURFACE_POOL))
#define TUR_IS_SURFACE_POOL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TUR_TYPE_SURFACE_POOL))
#define TUR_SURFACE_POOL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TUR_TYPE_SURFACE_POOL, TurSurfacePoolClass))


typedef struct _TurSurfacePool               TurSurfacePool;
typedef struct _TurSurfacePoolPrivate        TurSurfacePoolPrivate;
typedef struct _TurSurfacePoolClass          TurSurfacePoolClass;


struct _TurSurfacePool {
	GObject parent;
};

struct _TurSurfacePoolClass {
	GObjectClass parent_class;
};


GType tur_surface_pool_get_type();

TurSurfacePool *tur_surface_pool_new();

void tur_surface_pool_set_image_dimension(TurSurfacePool *pool, int width, int height);
void tur_surface_pool_set_image_height(TurSurfacePool *pool, int height);
void tur_surface_pool_set_image_width(TurSurfacePool *pool, int width);

TurSurfaceWrapper *tur_surface_pool_obtain(TurSurfacePool *pool, GdkWindow *window);
void tur_surface_pool_release(TurSurfacePool *pool, TurSurfaceWrapper *surface);

G_END_DECLS

#endif /* VIEW_TURSURFACEPOOL_H_ */
