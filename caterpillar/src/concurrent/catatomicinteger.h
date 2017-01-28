/*
   File:    catatomicinteger.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 23, 2014
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef CATATOMICINTEGER_H_
#define CATATOMICINTEGER_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_ATOMIC_INTEGER              (cat_atomic_integer_get_type())
#define CAT_ATOMIC_INTEGER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_atomic_integer_get_type(), CatAtomicInteger))
#define CAT_ATOMIC_INTEGER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_ATOMIC_INTEGER, CatAtomicIntegerClass))
#define CAT_IS_ATOMIC_INTEGER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_ATOMIC_INTEGER))
#define CAT_IS_ATOMIC_INTEGER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_ATOMIC_INTEGER))
#define CAT_ATOMIC_INTEGER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_ATOMIC_INTEGER, CatAtomicIntegerClass))
#define CAT_ATOMIC_INTEGER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CAT_TYPE_ATOMIC_INTEGER, CatAtomicIntegerPrivate))


typedef struct _CatAtomicInteger               CatAtomicInteger;
typedef struct _CatAtomicIntegerPrivate        CatAtomicIntegerPrivate;
typedef struct _CatAtomicIntegerClass          CatAtomicIntegerClass;


struct _CatAtomicInteger {
	GObject parent;
	CatAtomicIntegerPrivate *priv;
};

struct _CatAtomicIntegerClass {
	GObjectClass parent_class;
};


GType cat_atomic_integer_get_type();


CatAtomicInteger *cat_atomic_integer_new();

CatAtomicInteger *cat_atomic_integer_new_val(int initial);

gboolean cat_atomic_integer_compare_set(CatAtomicInteger *int_atomic, int expect, int set);

int cat_atomic_integer_get(CatAtomicInteger *int_atomic);

void cat_atomic_integer_set(CatAtomicInteger *int_atomic, int newVal);

int cat_atomic_integer_add(CatAtomicInteger *int_atomic, int delta);

int cat_atomic_integer_decrement(CatAtomicInteger *int_atomic);

int cat_atomic_integer_increment(CatAtomicInteger *int_atomic);

G_END_DECLS

#endif /* CATATOMICINTEGER_H_ */
