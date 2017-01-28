
/*
   File:    grolocation.h
   Project: gross
   Author:  Douwe Vos
   Date:    May 26, 2016
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

#ifndef PARSER_GROLOCATION_H_
#define PARSER_GROLOCATION_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_LOCATION              (gro_location_get_type())
#define GRO_LOCATION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_location_get_type(), GroLocation))
#define GRO_LOCATION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_LOCATION, GroLocationClass))
#define GRO_IS_LOCATION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_LOCATION))
#define GRO_IS_LOCATION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_LOCATION))
#define GRO_LOCATION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_LOCATION, GroLocationClass))


typedef struct _GroLocation               GroLocation;
typedef struct _GroLocationPrivate        GroLocationPrivate;
typedef struct _GroLocationClass          GroLocationClass;


struct _GroLocation {
	GObject parent;
};

struct _GroLocationClass {
	GObjectClass parent_class;
};


GType gro_location_get_type();

GroLocation *gro_location_new(long long row, int column);

long long gro_location_get_row(const GroLocation *location);
int gro_location_get_column(const GroLocation *location);

G_END_DECLS

#endif /* PARSER_GROLOCATION_H_ */
