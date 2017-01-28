#ifndef SMPL2DARRAY_H_
#define SMPL2DARRAY_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define SMPL_TYPE_2D_ARRAY            (smpl_2d_array_get_type())
#define SMPL_2D_ARRAY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), smpl_2d_array_get_type(), Smpl2DArray))
#define SMPL_2D_ARRAY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), SMPL_TYPE_2D_ARRAY, Smpl2DArrayClass))
#define SMPL_IS_2D_ARRAY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SMPL_TYPE_2D_ARRAY))
#define SMPL_IS_2D_ARRAY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), SMPL_TYPE_2D_ARRAY))
#define SMPL_2D_ARRAY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), SMPL_TYPE_2D_ARRAY, Smpl2DArrayClass))

typedef struct _Smpl2DArray       Smpl2DArray;
typedef struct _Smpl2DArrayClass  Smpl2DArrayClass;


struct _Smpl2DArray {
	GObject parent;
	short *data;
	int row_count;
	short *row_lengths;
	short **row_data;
};


struct _Smpl2DArrayClass {
	GObjectClass parent_class;
};


GType smpl_2d_array_get_type(void);

Smpl2DArray *smpl_2d_array_new(short *data);

int smpl_2d_array_get(Smpl2DArray *array, int row, int column);
short *smpl_2d_array_get_row(Smpl2DArray *array, int row, int *row_length);

int smpl_2d_array_column_count(Smpl2DArray *array, int row);
int smpl_2d_array_row_count(Smpl2DArray *array);


G_END_DECLS


#endif /* SMPL2DARRAY_H_ */
