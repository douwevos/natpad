/*
   File:    cathashset.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Sep 13, 2011
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2011 Douwe Vos.

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


#ifndef CATHASHSET_H_
#define CATHASHSET_H_

//#include <caterpillar.h>

#include "woo/catarraywo.h"
#include "catiiterator.h"
#include <glib-object.h>

G_BEGIN_DECLS

#ifndef __attribute_warn_unused_result__
#define __attribute_warn_unused_result__
#endif

#define CAT_TYPE_HASH_SET              (cat_hash_set_get_type())
#define CAT_HASH_SET(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_hash_set_get_type(), CatHashSet))
#define CAT_HASH_SET_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_HASH_SET, CatHashSetClass))
#define CAT_IS_HASH_SET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_HASH_SET))
#define CAT_IS_HASH_SET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_HASH_SET))
#define CAT_HASH_SET_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_HASH_SET, CatHashSetClass))
#define CAT_HASH_SET_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CAT_TYPE_HASH_SET, CatHashSetPrivate))


typedef struct _CatHashSet               CatHashSet;
typedef struct _CatHashSetPrivate        CatHashSetPrivate;
typedef struct _CatHashSetClass          CatHashSetClass;


struct _CatHashSet {
	GObject parent;
	CatHashSetPrivate *priv;
};

struct _CatHashSetClass {
	GObjectClass parent_class;
};


GType cat_hash_set_get_type();

CatHashSet *cat_hash_set_new(GHashFunc hash_func, GEqualFunc equal_func);


void cat_hash_set_clear(CatHashSet *hash_set);

int cat_hash_set_size(CatHashSet *hash_set);


/**
 * Adds the given object to_add to the HashSet. If the object was added the method returns NULL. If an equal object
 * already existed in the HashSet then it is returned by the method.
 */
GObject *cat_hash_set_add(CatHashSet *hash_set, GObject *to_add);

gboolean cat_hash_set_add_all(CatHashSet *hash_set, const CatHashSet *set_to_add);


/**
 * Gets an object from the hash-set. If the given key doesn't exist then NULL is returned. If the key does exist then
 * the object stored in the hash-set is returned (this might be the key itself).
 */
GObject *cat_hash_set_get(CatHashSet *hash_set, GObject *key);

/**
 * Removes the given key from the hash set. Either the object being removed is returned or NULL is returned when an
 * object matching the key did not exist. The returned object should be dereferenced by the caller.
 */
GObject *cat_hash_set_remove(CatHashSet *hash_set, GObject *key) __attribute_warn_unused_result__;

/**
 * Creates a flat list of all objects stored in this set.
 */
CatArrayWo *cat_hash_set_enlist_all(CatHashSet *hash_set);

int cat_hash_set_add_all_from_list(CatHashSet *hash_set, CatArrayWo *list_to_add);

CatIIterator *cat_hash_set_iterator(CatHashSet *hash_set) __attribute_warn_unused_result__;

gboolean cat_hash_set_intersection(CatHashSet *hash_set, CatHashSet *to_intersect_with);

gboolean cat_hash_set_equal(const CatHashSet *hash_set_a, const CatHashSet *hash_set_b);

G_END_DECLS


#endif /* CATHASHSET_H_ */
