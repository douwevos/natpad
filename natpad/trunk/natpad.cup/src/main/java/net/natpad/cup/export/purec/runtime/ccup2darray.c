#include "[%pre%]2darray.h"
#include <stdlib.h>

void [%pre%]_2d_array_free([%Pre%]2DArray *array) {
	if (array==NULL) {
		return;
	}
	array->data = NULL;
	if (array->row_lengths) {
		free(array->row_lengths);
		array->row_lengths = NULL;
	}
	if (array->row_data) {
		free(array->row_data);
		array->row_data = NULL;
	}
	array->row_count = 0;
	free(array);
}


[%Pre%]2DArray *[%pre%]_2d_array_new(short *data) {
	[%Pre%]2DArray *result = ([%Pre%]2DArray *) malloc(sizeof([%Pre%]2DArray));
	result->data = data;
	int row_idx;
	int row_count = *data;
	result->row_count = row_count;
	data++;
	result->row_lengths = (short *) malloc(sizeof(short)*row_count);
	for(row_idx=0; row_idx<row_count; row_idx++) {
		result->row_lengths[row_idx] = *data;
		data++;
	}
	result->row_data = (short **) malloc(sizeof(short *)*row_count);
	for(row_idx=0; row_idx<row_count; row_idx++) {
		result->row_data[row_idx] = data;
		data+=result->row_lengths[row_idx];
	}
	return result;
}

int [%pre%]_2d_array_get([%Pre%]2DArray *array, int row, int column) {
	if (row<0 || row>=array->row_count) {
		return -1;
	}
	if (column<0 || column>=array->row_lengths[row]) {
		return -1;
	}
	return array->row_data[row][column];
}


short *[%pre%]_2d_array_get_row([%Pre%]2DArray *array, int row, int *row_length) {

	if (row<0 || row>=array->row_count) {
		return NULL;
	}
	if (row_length!=NULL) {
		*row_length = array->row_lengths[row];
	}
	return array->row_data[row];
}



int [%pre%]_2d_array_column_count([%Pre%]2DArray *array, int row) {
	if (row<0 || row>=array->row_count) {
		return -1;
	}
	return array->row_lengths[row];
}

int [%pre%]_2d_array_row_count([%Pre%]2DArray *array) {
	return array->row_count;
}
