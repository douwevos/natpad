#ifndef [%PRE%]2DARRAY_H_
#define [%PRE%]2DARRAY_H_

typedef struct s_[%Pre%]2DArray       [%Pre%]2DArray;

struct s_[%Pre%]2DArray {
	short *data;
	int row_count;
	short *row_lengths;
	short **row_data;
};

[%Pre%]2DArray *[%pre%]_2d_array_new(short *data);

int [%pre%]_2d_array_get([%Pre%]2DArray *array, int row, int column);
short *[%pre%]_2d_array_get_row([%Pre%]2DArray *array, int row, int *row_length);

int [%pre%]_2d_array_column_count([%Pre%]2DArray *array, int row);
int [%pre%]_2d_array_row_count([%Pre%]2DArray *array);


#endif /* [%PRE%]2DARRAY_H_ */
