/*
   File:    catweakreference.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Dec 25, 2013
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

#ifndef CATWEAKREFERENCE_H_
#define CATWEAKREFERENCE_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_WEAK_REFERENCE              (cat_weak_reference_get_type())
#define CAT_WEAK_REFERENCE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_weak_reference_get_type(), CatWeakReference))
#define CAT_WEAK_REFERENCE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_WEAK_REFERENCE, CatWeakReferenceClass))
#define CAT_IS_WEAK_REFERENCE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_WEAK_REFERENCE))
#define CAT_IS_WEAK_REFERENCE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_WEAK_REFERENCE))
#define CAT_WEAK_REFERENCE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_WEAK_REFERENCE, CatWeakReferenceClass))


typedef struct _CatWeakReference               CatWeakReference;
typedef struct _CatWeakReferencePrivate        CatWeakReferencePrivate;
typedef struct _CatWeakReferenceClass          CatWeakReferenceClass;


struct _CatWeakReference {
	GObject parent;
};

struct _CatWeakReferenceClass {
	GObjectClass parent_class;
};


GType cat_weak_reference_get_type();

/**
 * Creates a new weak-reference to the given value.
 */
CatWeakReference *cat_weak_reference_new(GObject *value);

/**
 * Returns a strong reference to the internally stored value or NULL if the value was already unreferenced.
 */
GObject *cat_weak_reference_get(CatWeakReference *reference);


void cat_weak_reference_set(CatWeakReference *reference, GObject *new_value);


G_END_DECLS



#endif /* CATWEAKREFERENCE_H_ */
