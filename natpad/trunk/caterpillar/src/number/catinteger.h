/*
   File:    catinteger.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Nov 5, 2012
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

#ifndef CATINTEGER_H_
#define CATINTEGER_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_INTEGER              (cat_integer_get_type())
#define CAT_INTEGER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_integer_get_type(), CatInteger))
#define CAT_INTEGER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_INTEGER, CatIntegerClass))
#define CAT_IS_INTEGER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_INTEGER))
#define CAT_IS_INTEGER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_INTEGER))
#define CAT_INTEGER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_INTEGER, CatIntegerClass))
#define CAT_INTEGER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CAT_TYPE_INTEGER, CatIntegerPrivate))

typedef struct _CatInteger               CatInteger;
typedef struct _CatIntegerPrivate        CatIntegerPrivate;
typedef struct _CatIntegerClass          CatIntegerClass;


struct _CatInteger {
	GObject parent;
	CatIntegerPrivate *priv;
};

struct _CatIntegerClass {
	GObjectClass parent_class;
};


GType cat_integer_get_type();

CatInteger *cat_integer_new(int value);

int cat_integer_value(CatInteger *iobj);

int cat_integer_hash(CatInteger *iobj);

gboolean cat_integer_equal(const CatInteger *iobj_a, const CatInteger *iobj_b);

int cat_integer_compare(CatInteger *iobj_a, CatInteger *iobj_b);

G_END_DECLS

#endif /* CATINTEGER_H_ */
