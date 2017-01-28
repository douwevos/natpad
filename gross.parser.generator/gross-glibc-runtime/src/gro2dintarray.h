
/*
   File:    gro2dintarray.h
   Project: gross-glibc-runtime
   Author:  Douwe Vos
   Date:    Jun 25, 2016
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

#ifndef GRO2DINTARRAY_H_
#define GRO2DINTARRAY_H_

#include "groglibctypes.h"

G_BEGIN_DECLS

#define GRO_TYPE_2D_INT_ARRAY              (gro_2d_int_array_get_type())
#define GRO_2D_INT_ARRAY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_2d_int_array_get_type(), Gro2DIntArray))
#define GRO_2D_INT_ARRAY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_2D_INT_ARRAY, Gro2DIntArrayClass))
#define GRO_IS_2D_INT_ARRAY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_2D_INT_ARRAY))
#define GRO_IS_2D_INT_ARRAY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_2D_INT_ARRAY))
#define GRO_2D_INT_ARRAY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_2D_INT_ARRAY, Gro2DIntArrayClass))


typedef struct _Gro2DIntArray               Gro2DIntArray;
typedef struct _Gro2DIntArrayPrivate        Gro2DIntArrayPrivate;
typedef struct _Gro2DIntArrayClass          Gro2DIntArrayClass;


struct _Gro2DIntArray {
	GObject parent;
};

struct _Gro2DIntArrayClass {
	GObjectClass parent_class;
};


GType gro_2d_int_array_get_type();

Gro2DIntArray *gro_2d_int_array_new(int const *data);

int gro_2d_int_array_get(Gro2DIntArray *array, int row, int column);
int const *gro_2d_int_array_get_row(Gro2DIntArray *array, int row, int *row_length);

int gro_2d_int_array_column_count(Gro2DIntArray *array, int row);
int gro_2d_int_array_row_count(Gro2DIntArray *array);


G_END_DECLS

#endif /* GRO2DINTARRAY_H_ */
