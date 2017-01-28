
/*
   File:    gro2dintarray.c
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

#include "gro2dintarray.h"

struct _Gro2DIntArrayPrivate {
	int const *data;
	int row_count;
	int default_value;
	int *row_lengths;
	int const **row_data;
};

G_DEFINE_TYPE_WITH_CODE(Gro2DIntArray, gro_2d_int_array, G_TYPE_OBJECT,
		G_ADD_PRIVATE(Gro2DIntArray)
);

static void l_dispose(GObject *object);

static void gro_2d_int_array_class_init(Gro2DIntArrayClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
}

static void gro_2d_int_array_init(Gro2DIntArray *instance) {
}

static void l_dispose(GObject *object) {
	Gro2DIntArray *instance = GRO_2D_INT_ARRAY(object);
	Gro2DIntArrayPrivate *priv = gro_2d_int_array_get_instance_private(instance);
	gro_free_ptr(priv->row_lengths);
	gro_free_ptr(priv->row_data);
	G_OBJECT_CLASS(gro_2d_int_array_parent_class)->dispose(object);
}


Gro2DIntArray *gro_2d_int_array_new(int const *data) {
	Gro2DIntArray *result = g_object_new(GRO_TYPE_2D_INT_ARRAY, NULL);
	Gro2DIntArrayPrivate *priv = gro_2d_int_array_get_instance_private(result);
	priv->data = data;
	int row_idx;
	int row_count = *data;
	priv->row_count = row_count;
	data++;
	priv->default_value = *data;
	data++;
	priv->row_lengths = (int *) g_malloc(sizeof(int)*row_count);
	for(row_idx=0; row_idx<row_count; row_idx++) {
		priv->row_lengths[row_idx] = *data;
		data++;
	}
	priv->row_data = (int const **) g_malloc(sizeof(int *)*row_count);

	for(row_idx=0; row_idx<row_count; row_idx++) {
		priv->row_data[row_idx] = data;

#ifdef GROSS_DEBUG
		printf("[%d]:", row_idx);
		int col_idx;
		for(col_idx=0; col_idx<priv->row_lengths[row_idx]; col_idx++) {
			printf("%d ", data[col_idx]);
		}
		printf("\n");
#endif
		data+=priv->row_lengths[row_idx];
	}
	return result;	return result;
}



int gro_2d_int_array_get(Gro2DIntArray *array, int row, int column) {
	Gro2DIntArrayPrivate *priv = gro_2d_int_array_get_instance_private(array);
	if (row<0 || row>=priv->row_count) {
		return priv->default_value;
	}
	if (column<0 || column>=priv->row_lengths[row]) {
		return priv->default_value;
	}
	return priv->row_data[row][column];
}


int const *gro_2d_int_array_get_row(Gro2DIntArray *array, int row, int *row_length) {
	Gro2DIntArrayPrivate *priv = gro_2d_int_array_get_instance_private(array);
	if (row<0 || row>=priv->row_count) {
		return NULL;
	}
	if (row_length!=NULL) {
		*row_length = priv->row_lengths[row];
	}
	return priv->row_data[row];
}



int gro_2d_int_array_column_count(Gro2DIntArray *array, int row) {
	Gro2DIntArrayPrivate *priv = gro_2d_int_array_get_instance_private(array);
	if (row<0 || row>=priv->row_count) {
		return priv->default_value;
	}
	return priv->row_lengths[row];
}

int gro_2d_int_array_row_count(Gro2DIntArray *array) {
	Gro2DIntArrayPrivate *priv = gro_2d_int_array_get_instance_private(array);
	return priv->row_count;
}
