/*
   File:    catboolean.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Nov 12, 2012
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


#ifndef CATBOOLEAN_H_
#define CATBOOLEAN_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_BOOLEAN              (cat_boolean_get_type())
#define CAT_BOOLEAN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_boolean_get_type(), CatBoolean))
#define CAT_BOOLEAN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_BOOLEAN, CatBooleanClass))
#define CAT_IS_BOOLEAN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_BOOLEAN))
#define CAT_IS_BOOLEAN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_BOOLEAN))
#define CAT_BOOLEAN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_BOOLEAN, CatBooleanClass))
#define CAT_BOOLEAN_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CAT_TYPE_BOOLEAN, CatBooleanPrivate))

typedef struct _CatBoolean               CatBoolean;
typedef struct _CatBooleanPrivate        CatBooleanPrivate;
typedef struct _CatBooleanClass          CatBooleanClass;


struct _CatBoolean {
	GObject parent;
	CatBooleanPrivate *priv;
};

struct _CatBooleanClass {
	GObjectClass parent_class;
};


GType cat_boolean_get_type();

CatBoolean *cat_boolean_new(gboolean init_value);

gboolean cat_boolean_value(CatBoolean *bobj);

int cat_boolean_hash(CatBoolean *bobj);

gboolean cat_boolean_equal(CatBoolean *bobj_a, CatBoolean *bobj_b);

int cat_boolean_compare(CatBoolean *bobj_a, CatBoolean *bobj_b);


G_END_DECLS


#endif /* CATBOOLEAN_H_ */
