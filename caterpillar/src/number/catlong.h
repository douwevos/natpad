/*
   File:    catlong.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Nov 16, 2012
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


#ifndef CATLONG_H_
#define CATLONG_H_

#include <glib-object.h>
#include <sys/types.h>
#include <stdint.h>

G_BEGIN_DECLS

#define CAT_TYPE_LONG              (cat_long_get_type())
#define CAT_LONG(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_long_get_type(), CatLong))
#define CAT_LONG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_LONG, CatLongClass))
#define CAT_IS_LONG(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_LONG))
#define CAT_IS_LONG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_LONG))
#define CAT_LONG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_LONG, CatLongClass))

typedef struct _CatLong               CatLong;
typedef struct _CatLongPrivate        CatLongPrivate;
typedef struct _CatLongClass          CatLongClass;


struct _CatLong {
	GObject parent;
};

struct _CatLongClass {
	GObjectClass parent_class;
};


GType cat_long_get_type();

CatLong *cat_long_new(int64_t value);

int64_t cat_long_value(CatLong *lobj);

int cat_long_hash(CatLong *lobj);

gboolean cat_long_equal(CatLong *lobj_a, CatLong *lobj_b);

int cat_long_compare(CatLong *lobj_a, CatLong *lobj_b);


G_END_DECLS


#endif /* CATLONG_H_ */
