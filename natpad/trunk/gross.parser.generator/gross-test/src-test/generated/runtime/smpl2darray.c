#include "smpl2darray.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "Smpl2DArray"
#include <logging/catlog.h>

G_DEFINE_TYPE(Smpl2DArray, smpl_2d_array, G_TYPE_OBJECT)

static void _dispose(GObject *object);

static void smpl_2d_array_class_init(Smpl2DArrayClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void smpl_2d_array_init(Smpl2DArray *obj) {
}

static void _dispose(GObject *object) {
	Smpl2DArray *instance = SMPL_2D_ARRAY(object);
	cat_free_ptr(instance->data);
	cat_free_ptr(instance->row_lengths);
	cat_free_ptr(instance->row_data);
	instance->row_count = 0;
}


Smpl2DArray *smpl_2d_array_new(short *data) {
	Smpl2DArray *result = g_object_new(SMPL_TYPE_2D_ARRAY, NULL);
	cat_ref_anounce(result);
	result->data = data;
	int row_idx;
	int row_count = *data;
	result->row_count = row_count;
	data++;
	result->row_lengths = (short *) g_malloc(sizeof(short)*row_count);
	for(row_idx=0; row_idx<row_count; row_idx++) {
		result->row_lengths[row_idx] = *data;
		data++;
	}
	result->row_data = (short **) g_malloc(sizeof(short *)*row_count);
	for(row_idx=0; row_idx<row_count; row_idx++) {
		result->row_data[row_idx] = data;
		data+=result->row_lengths[row_idx];
	}
	return result;
}

int smpl_2d_array_get(Smpl2DArray *array, int row, int column) {
	if (row<0 || row>=array->row_count) {
		return -1;
	}
	if (column<0 || column>=array->row_lengths[row]) {
		return -1;
	}
	return array->row_data[row][column];
}


short *smpl_2d_array_get_row(Smpl2DArray *array, int row, int *row_length) {

	if (row<0 || row>=array->row_count) {
		return NULL;
	}
	if (row_length!=NULL) {
		*row_length = array->row_lengths[row];
	}
	return array->row_data[row];
}



int smpl_2d_array_column_count(Smpl2DArray *array, int row) {
	if (row<0 || row>=array->row_count) {
		return -1;
	}
	return array->row_lengths[row];
}

int smpl_2d_array_row_count(Smpl2DArray *array) {
	return array->row_count;
}
