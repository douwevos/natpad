/*
   File:    catatomicreference.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    May 9, 2013
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


#ifndef CATATOMICREFERENCE_H_
#define CATATOMICREFERENCE_H_


#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_ATOMIC_REFERENCE              (cat_atomic_reference_get_type())
#define CAT_ATOMIC_REFERENCE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_atomic_reference_get_type(), CatAtomicReference))
#define CAT_ATOMIC_REFERENCE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_ATOMIC_REFERENCE, CatAtomicReferenceClass))
#define CAT_IS_ATOMIC_REFERENCE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_ATOMIC_REFERENCE))
#define CAT_IS_ATOMIC_REFERENCE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_ATOMIC_REFERENCE))
#define CAT_ATOMIC_REFERENCE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_ATOMIC_REFERENCE, CatAtomicReferenceClass))
#define CAT_ATOMIC_REFERENCE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CAT_TYPE_ATOMIC_REFERENCE, CatAtomicReferencePrivate))


typedef struct _CatAtomicReference               CatAtomicReference;
typedef struct _CatAtomicReferencePrivate        CatAtomicReferencePrivate;
typedef struct _CatAtomicReferenceClass          CatAtomicReferenceClass;


struct _CatAtomicReference {
	GObject parent;
	CatAtomicReferencePrivate *priv;
};

struct _CatAtomicReferenceClass {
	GObjectClass parent_class;
};


GType cat_atomic_reference_get_type();


CatAtomicReference *cat_atomic_reference_new();

CatAtomicReference *cat_atomic_reference_new_val(GObject *initial);

gboolean cat_atomic_reference_compare_set(CatAtomicReference *atomic_ref, GObject *expect, GObject *set);

/*
 * cat_atomic_reference_get:
 * @atomic_ref: a #CatAtomicReference
 *
 * Returns a reference to the current set #GObject. Please note that when the function returns the content of the
 * atomic reference might already have been changed. This is also the reason why the reference count of the returned
 * object is increased and needs to be dereferenced by the caller.
 *
 * Return value: A reference to the object stored in this atomic-reference.
 */
GObject *cat_atomic_reference_get(CatAtomicReference *atomic_ref);

/**
 * cat_atomic_reference_get_unsafe:
 * @atomic_ref: a #CatAtomicReference
 *
 * Returns the current set #GObject. Please note that the function returns the content of the atomic reference without
 * obeying any locks. This is useful when the caller is sure that this function is called from the same thread as where
 * the value is set and that the value is only set by this thread only (in other words there is a single writer thread
 * and multiple reader threads).
 *
 * Return value: The object stored in this atomic-reference.
 */
GObject *cat_atomic_reference_get_unsafe(CatAtomicReference *atomic_ref);


gboolean cat_atomic_reference_set(CatAtomicReference *atomic_ref, GObject *newVal);

G_END_DECLS

#endif /* CATATOMICREFERENCE_H_ */
