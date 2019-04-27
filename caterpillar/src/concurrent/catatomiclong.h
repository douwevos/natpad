/*
   File:    catatomiclong.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Apr 13, 2013
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#ifndef CATATOMICLONG_H_
#define CATATOMICLONG_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_ATOMIC_LONG              (cat_atomic_long_get_type())
#define CAT_ATOMIC_LONG(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_atomic_long_get_type(), CatAtomicLong))
#define CAT_ATOMIC_LONG_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_ATOMIC_LONG, CatAtomicLongClass))
#define CAT_IS_ATOMIC_LONG(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_ATOMIC_LONG))
#define CAT_IS_ATOMIC_LONG_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_ATOMIC_LONG))
#define CAT_ATOMIC_LONG_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_ATOMIC_LONG, CatAtomicLongClass))


typedef struct _CatAtomicLong               CatAtomicLong;
typedef struct _CatAtomicLongPrivate        CatAtomicLongPrivate;
typedef struct _CatAtomicLongClass          CatAtomicLongClass;


struct _CatAtomicLong {
	GObject parent;
};

struct _CatAtomicLongClass {
	GObjectClass parent_class;
};


GType cat_atomic_long_get_type();


CatAtomicLong *cat_atomic_long_new();

CatAtomicLong *cat_atomic_long_new_val(long long initial);

gboolean cat_atomic_long_compare_set(CatAtomicLong *long_atomic, long long expect, long long set);

long long cat_atomic_long_get(CatAtomicLong *long_atomic);

void cat_atomic_long_set(CatAtomicLong *long_atomic, long long newVal);

long long cat_atomic_long_add(CatAtomicLong *long_atomic, long long delta);

long long cat_atomic_long_decrement(CatAtomicLong *long_atomic);

long long cat_atomic_long_increment(CatAtomicLong *long_atomic);

G_END_DECLS

#endif /* CATATOMICLONG_H_ */
