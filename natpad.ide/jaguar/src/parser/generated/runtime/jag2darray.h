#ifndef JAG2DARRAY_H_
#define JAG2DARRAY_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_2D_ARRAY            (jag_2d_array_get_type())
#define JAG_2D_ARRAY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_2d_array_get_type(), Jag2DArray))
#define JAG_2D_ARRAY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_2D_ARRAY, Jag2DArrayClass))
#define JAG_IS_2D_ARRAY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_2D_ARRAY))
#define JAG_IS_2D_ARRAY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_2D_ARRAY))
#define JAG_2D_ARRAY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_2D_ARRAY, Jag2DArrayClass))

typedef struct _Jag2DArray       Jag2DArray;
typedef struct _Jag2DArrayClass  Jag2DArrayClass;


struct _Jag2DArray {
	GObject parent;
	short *data;
	int row_count;
	short *row_lengths;
	short **row_data;
};


struct _Jag2DArrayClass {
	GObjectClass parent_class;
};


GType jag_2d_array_get_type(void);

Jag2DArray *jag_2d_array_new(short *data);

int jag_2d_array_get(Jag2DArray *array, int row, int column);
short *jag_2d_array_get_row(Jag2DArray *array, int row, int *row_length);

int jag_2d_array_column_count(Jag2DArray *array, int row);
int jag_2d_array_row_count(Jag2DArray *array);


G_END_DECLS


#endif /* JAG2DARRAY_H_ */
