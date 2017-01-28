#include "[%pre%]vector.h"
#include <stdlib.h>



void [%pre%]_vector_free([%Pre%]Vector *vector) {
	if (vector) {
		if (vector->data) {
			free(vector->data);
			vector->data = NULL;
		}
		free(vector);
	}
}

[%Pre%]Vector *[%pre%]_vector_new(VectorReleaseCB vector_release_cb) {
	[%Pre%]Vector *result = ([%Pre%]Vector *) malloc(sizeof([%Pre%]Vector));
	result->data_buf_size=10;
	result->data_size=0;
	result->data = (void**) malloc(sizeof(void*)*result->data_buf_size);
	result->vector_release_cb = vector_release_cb;
	return result;
}

int [%pre%]_vector_count([%Pre%]Vector *vector) {
	return vector->data_size;
}


void [%pre%]_vector_clear([%Pre%]Vector *vector) {
	if (vector->vector_release_cb) {
		int idx;
		for(idx=0; idx<vector->data_size; idx++) {
			if (vector->data[idx]) {
				vector->vector_release_cb(vector, vector->data[idx]);
			}
		}
	}
	vector->data_size = 0;
}


static void ensure_capacity([%Pre%]Vector *vector, int min_cap) {
	min_cap = min_cap+16;
	min_cap = min_cap-min_cap%16;
	if (min_cap!=vector->data_size) {
		if (min_cap>vector->data_size) {
			void **new_data = malloc(sizeof(void *)*min_cap);
			int idx;
			for(idx=0; idx<vector->data_size; idx++) {
				new_data[idx] = vector->data[idx];
			}
			free(vector->data);
			vector->data = new_data;
			vector->data_buf_size = min_cap;
		}
	}
}

void [%pre%]_vector_add([%Pre%]Vector *vector, void *data) {
	ensure_capacity(vector, vector->data_size+1);
	vector->data[vector->data_size++] = data;
}

void *[%pre%]_vector_pop([%Pre%]Vector *vector) {
	if (vector->data_size<=0) {
		return NULL;
	}
	vector->data_size--;
	if (vector->vector_release_cb) {
		vector->vector_release_cb(vector, vector->data[vector->data_size]);
	}
	return vector->data[vector->data_size];
}


void *[%pre%]_vector_get_at([%Pre%]Vector *vector, int index) {
	if (index<0 || index>=vector->data_size) {
		return NULL;
	}
	return vector->data[index];
}

void [%pre%]_vector_set_at([%Pre%]Vector *vector, void *val, int index) {
	if (index<0) {
		return;
	}
	if (index>=vector->data_size) {
		ensure_capacity(vector, index+1);
		while(vector->data_size<=index) {
			vector->data[vector->data_size++] = NULL;
		}
	}
	vector->data[index] = NULL;
	// TODO consider releasing old value
	vector->data[index];
}


