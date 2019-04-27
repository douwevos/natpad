/*
   File:    catfloat.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Nov 19, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef CATFLOAT_H_
#define CATFLOAT_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_FLOAT              (cat_float_get_type())
#define CAT_FLOAT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_float_get_type(), CatFloat))
#define CAT_FLOAT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_FLOAT, CatFloatClass))
#define CAT_IS_FLOAT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_FLOAT))
#define CAT_IS_FLOAT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_FLOAT))
#define CAT_FLOAT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_FLOAT, CatFloatClass))

typedef struct _CatFloat               CatFloat;
typedef struct _CatFloatPrivate        CatFloatPrivate;
typedef struct _CatFloatClass          CatFloatClass;


struct _CatFloat {
	GObject parent;
};

struct _CatFloatClass {
	GObjectClass parent_class;
};


GType cat_float_get_type();

CatFloat *cat_float_new(float val);

float cat_float_value(CatFloat *fobj);

int cat_float_hash(CatFloat *fobj);

gboolean cat_float_equal(CatFloat *fobj_a, CatFloat *fobj_b);

int cat_float_compare(CatFloat *fobj_a, CatFloat *fobj_b);

G_END_DECLS

#endif /* CATFLOAT_H_ */
