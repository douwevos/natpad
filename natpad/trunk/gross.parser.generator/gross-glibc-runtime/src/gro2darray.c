#include "gro2darray.h"

struct _Gro2DArrayPrivate {
	short *data;
	int row_count;
	int default_value;
	short *row_lengths;
	short **row_data;
};

G_DEFINE_TYPE_WITH_CODE(Gro2DArray, gro_2d_array, G_TYPE_OBJECT,
		G_ADD_PRIVATE(Gro2DArray)
);

static void l_dispose(GObject *object);

static void gro_2d_array_class_init(Gro2DArrayClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
}

static void gro_2d_array_init(Gro2DArray *obj) {
}

static void l_dispose(GObject *object) {
	Gro2DArray *instance = GRO_2D_ARRAY(object);
	Gro2DArrayPrivate *priv = gro_2d_array_get_instance_private(instance);
	gro_free_ptr(priv->data);
	gro_free_ptr(priv->row_lengths);
	gro_free_ptr(priv->row_data);
	priv->row_count = 0;
}


Gro2DArray *gro_2d_array_new(short *data) {
	Gro2DArray *result = g_object_new(GRO_TYPE_2D_ARRAY, NULL);
	Gro2DArrayPrivate *priv = gro_2d_array_get_instance_private(result);
	priv->data = data;
	int row_idx;
	int row_count = *data;
	priv->row_count = row_count;
	data++;
	priv->default_value = *data;
	data++;
	priv->row_lengths = (short *) g_malloc(sizeof(short)*row_count);
	for(row_idx=0; row_idx<row_count; row_idx++) {
		priv->row_lengths[row_idx] = *data;
		data++;
	}
	priv->row_data = (short **) g_malloc(sizeof(short *)*row_count);

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
	return result;
}

int gro_2d_array_get(Gro2DArray *array, int row, int column) {
	Gro2DArrayPrivate *priv = gro_2d_array_get_instance_private(array);
	if (row<0 || row>=priv->row_count) {
		return priv->default_value;
	}
	if (column<0 || column>=priv->row_lengths[row]) {
		return priv->default_value;
	}
	return priv->row_data[row][column];
}


short *gro_2d_array_get_row(Gro2DArray *array, int row, int *row_length) {
	Gro2DArrayPrivate *priv = gro_2d_array_get_instance_private(array);
	if (row<0 || row>=priv->row_count) {
		return NULL;
	}
	if (row_length!=NULL) {
		*row_length = priv->row_lengths[row];
	}
	return priv->row_data[row];
}



int gro_2d_array_column_count(Gro2DArray *array, int row) {
	Gro2DArrayPrivate *priv = gro_2d_array_get_instance_private(array);
	if (row<0 || row>=priv->row_count) {
		return priv->default_value;
	}
	return priv->row_lengths[row];
}

int gro_2d_array_row_count(Gro2DArray *array) {
	Gro2DArrayPrivate *priv = gro_2d_array_get_instance_private(array);
	return priv->row_count;
}
