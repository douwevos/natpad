#ifndef GRO2DARRAY_H_
#define GRO2DARRAY_H_

#include "groglibctypes.h"
#include <glib-object.h>

G_BEGIN_DECLS

#define GRO_TYPE_2D_ARRAY            (gro_2d_array_get_type())
#define GRO_2D_ARRAY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_2d_array_get_type(), Gro2DArray))
#define GRO_2D_ARRAY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_2D_ARRAY, Gro2DArrayClass))
#define GRO_IS_2D_ARRAY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_2D_ARRAY))
#define GRO_IS_2D_ARRAY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_2D_ARRAY))
#define GRO_2D_ARRAY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_2D_ARRAY, Gro2DArrayClass))

typedef struct _Gro2DArray              Gro2DArray;
typedef struct _Gro2DArrayPrivate       Gro2DArrayPrivate;
typedef struct _Gro2DArrayClass         Gro2DArrayClass;


struct _Gro2DArray {
	GObject parent;
};

struct _Gro2DArrayClass {
	GObjectClass parent_class;
};

GType gro_2d_array_get_type(void);

Gro2DArray *gro_2d_array_new(short *data);

int gro_2d_array_get(Gro2DArray *array, int row, int column);
short *gro_2d_array_get_row(Gro2DArray *array, int row, int *row_length);

int gro_2d_array_column_count(Gro2DArray *array, int row);
int gro_2d_array_row_count(Gro2DArray *array);

G_END_DECLS

#endif /* GRO2DARRAY_H_ */
