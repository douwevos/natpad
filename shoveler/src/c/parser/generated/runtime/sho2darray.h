#ifndef SHO2DARRAY_H_
#define SHO2DARRAY_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define SHO_TYPE_2D_ARRAY            (sho_2d_array_get_type())
#define SHO_2D_ARRAY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_2d_array_get_type(), Sho2DArray))
#define SHO_2D_ARRAY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_2D_ARRAY, Sho2DArrayClass))
#define SHO_IS_2D_ARRAY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_2D_ARRAY))
#define SHO_IS_2D_ARRAY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_2D_ARRAY))
#define SHO_2D_ARRAY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_2D_ARRAY, Sho2DArrayClass))

typedef struct _Sho2DArray       Sho2DArray;
typedef struct _Sho2DArrayClass  Sho2DArrayClass;


struct _Sho2DArray {
	GObject parent;
	short *data;
	int row_count;
	short *row_lengths;
	short **row_data;
};


struct _Sho2DArrayClass {
	GObjectClass parent_class;
};


GType sho_2d_array_get_type(void);

Sho2DArray *sho_2d_array_new(short *data);

int sho_2d_array_get(Sho2DArray *array, int row, int column);
short *sho_2d_array_get_row(Sho2DArray *array, int row, int *row_length);

int sho_2d_array_column_count(Sho2DArray *array, int row);
int sho_2d_array_row_count(Sho2DArray *array);


G_END_DECLS


#endif /* SHO2DARRAY_H_ */
