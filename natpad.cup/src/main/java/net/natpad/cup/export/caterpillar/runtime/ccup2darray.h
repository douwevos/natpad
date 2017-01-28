#ifndef [%PRE%]2DARRAY_H_
#define [%PRE%]2DARRAY_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define [%PRE%]_TYPE_2D_ARRAY            ([%pre%]_2d_array_get_type())
#define [%PRE%]_2D_ARRAY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), [%pre%]_2d_array_get_type(), [%Pre%]2DArray))
#define [%PRE%]_2D_ARRAY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), [%PRE%]_TYPE_2D_ARRAY, [%Pre%]2DArrayClass))
#define [%PRE%]_IS_2D_ARRAY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), [%PRE%]_TYPE_2D_ARRAY))
#define [%PRE%]_IS_2D_ARRAY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), [%PRE%]_TYPE_2D_ARRAY))
#define [%PRE%]_2D_ARRAY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), [%PRE%]_TYPE_2D_ARRAY, [%Pre%]2DArrayClass))

typedef struct _[%Pre%]2DArray       [%Pre%]2DArray;
typedef struct _[%Pre%]2DArrayClass  [%Pre%]2DArrayClass;


struct _[%Pre%]2DArray {
	GObject parent;
	short *data;
	int row_count;
	short *row_lengths;
	short **row_data;
};


struct _[%Pre%]2DArrayClass {
	GObjectClass parent_class;
};


GType [%pre%]_2d_array_get_type(void);

[%Pre%]2DArray *[%pre%]_2d_array_new(short *data);

int [%pre%]_2d_array_get([%Pre%]2DArray *array, int row, int column);
short *[%pre%]_2d_array_get_row([%Pre%]2DArray *array, int row, int *row_length);

int [%pre%]_2d_array_column_count([%Pre%]2DArray *array, int row);
int [%pre%]_2d_array_row_count([%Pre%]2DArray *array);


G_END_DECLS


#endif /* [%PRE%]2DARRAY_H_ */
