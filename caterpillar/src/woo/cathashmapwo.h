/*
   File:    cathashmapwo.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Oct 8, 2014
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

#ifndef CATHASHMAPWO_H_
#define CATHASHMAPWO_H_

#include "catwo.h"
#include "catarraywo.h"

G_BEGIN_DECLS

#define CAT_TYPE_HASH_MAP_WO              (cat_hash_map_wo_get_type())
#define CAT_HASH_MAP_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_hash_map_wo_get_type(), CatHashMapWo))
#define CAT_HASH_MAP_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_HASH_MAP_WO, CatHashMapWoClass))
#define CAT_IS_HASH_MAP_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_HASH_MAP_WO))
#define CAT_IS_HASH_MAP_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_HASH_MAP_WO))
#define CAT_HASH_MAP_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_HASH_MAP_WO, CatHashMapWoClass))


typedef struct _CatHashMapWo               CatHashMapWo;
typedef struct _CatHashMapWoPrivate        CatHashMapWoPrivate;
typedef struct _CatHashMapWoClass          CatHashMapWoClass;


struct _CatHashMapWo {
	CatWo parent;
};

struct _CatHashMapWoClass {
	CatWoClass parent_class;
};


GType cat_hash_map_wo_get_type();

CatHashMapWo *cat_hash_map_wo_new(GHashFunc hash_func, GEqualFunc equal_func);

GObject *cat_hash_map_wo_get(const CatHashMapWo *map, void *key);
int cat_hash_map_wo_size(const CatHashMapWo *map);
int cat_hash_map_wo_modification_count(const CatHashMapWo *map);
gboolean cat_hash_map_wo_equal(const CatHashMapWo *map, const CatHashMapWo *other, GEqualFunc equal_func_for_value);


void cat_hash_map_wo_clear(CatHashMapWo *e_map);
CatHashMapWo *cat_hash_map_wo_clone(const CatHashMapWo *source, CatCloneDepth clone_depth) __attribute_warn_unused_result__;

/* Store the given value for the given key. If a value was already assigned to the key it will be overwritten with the
 * new value. If the original value was the same as the new value then false is returned informing the caller that
 * nothing changed. Two values are the same if they have the same pointer. In other words equality of the values is
 * based on pointers.
 *
 * @returns true if the value was stored and changed the map or false when the map was not changed.
 */
gboolean cat_hash_map_wo_put(CatHashMapWo *e_map, GObject *key, GObject *value);
void cat_hash_map_wo_put_all(CatHashMapWo *e_map, CatHashMapWo *to_add);
GObject *cat_hash_map_wo_remove(CatHashMapWo *e_map, GObject *key) __attribute_warn_unused_result__;

CatArrayWo *cat_hash_map_wo_enlist_keys(CatHashMapWo *map, CatArrayWo *e_into);

/* Append all values of the map into the given array e_into. If e_into is NULL
 *
 */
CatArrayWo *cat_hash_map_wo_enlist_values(CatHashMapWo *map, CatArrayWo *e_into);


CatHashMapWo *cat_hash_map_wo_create_editable(CatHashMapWo *instance) __attribute_warn_unused_result__;
CatHashMapWo *cat_hash_map_wo_anchor(CatHashMapWo *instance, int version) __attribute_warn_unused_result__;
CatHashMapWo *cat_hash_map_wo_get_original(CatHashMapWo *instance);
gboolean cat_hash_map_wo_is_anchored(const CatHashMapWo *instance);

G_END_DECLS

#endif /* CATHASHMAPWO_H_ */
