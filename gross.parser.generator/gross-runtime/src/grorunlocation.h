/*
   File:    grorunlocation.h
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 11, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef GRORUNLOCATION_H_
#define GRORUNLOCATION_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define GRORUN_TYPE_LOCATION              (grorun_location_get_type())
#define GRORUN_LOCATION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), grorun_location_get_type(), GroRunLocation))
#define GRORUN_LOCATION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRORUN_TYPE_LOCATION, GroRunLocationClass))
#define GRORUN_IS_LOCATION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRORUN_TYPE_LOCATION))
#define GRORUN_IS_LOCATION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRORUN_TYPE_LOCATION))
#define GRORUN_LOCATION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRORUN_TYPE_LOCATION, GroRunLocationClass))


typedef struct _GroRunLocation               GroRunLocation;
typedef struct _GroRunLocationPrivate        GroRunLocationPrivate;
typedef struct _GroRunLocationClass          GroRunLocationClass;


struct _GroRunLocation {
	GObject parent;
};

struct _GroRunLocationClass {
	GObjectClass parent_class;
};


GType grorun_location_get_type();

GroRunLocation *grorun_location_new(int begin_column, long begin_row, int end_column, long end_row);

GroRunLocation *grorun_location_set_end(GroRunLocation *location, int end_column, long end_row);

void grorun_location_get_all(GroRunLocation *location, int *begin_column, long *begin_row, int *end_column, long *end_row);

void grorun_location_get_begin(GroRunLocation *location, int *begin_column, long *begin_row);
void grorun_location_get_end(GroRunLocation *location, int *end_column, long *end_row);

int grorun_location_get_width(GroRunLocation *location);


G_END_DECLS

#endif /* GRORUNLOCATION_H_ */
