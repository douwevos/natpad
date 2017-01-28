/*
   File:    catweaklist.h
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

#ifndef CATWEAKLIST_H_
#define CATWEAKLIST_H_

#include "../woo/catarraywo.h"
#include "../catiiterator.h"

G_BEGIN_DECLS

#define CAT_TYPE_WEAK_LIST              (cat_weak_list_get_type())
#define CAT_WEAK_LIST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_weak_list_get_type(), CatWeakList))
#define CAT_WEAK_LIST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_WEAK_LIST, CatWeakListClass))
#define CAT_IS_WEAK_LIST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_WEAK_LIST))
#define CAT_IS_WEAK_LIST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_WEAK_LIST))
#define CAT_WEAK_LIST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_WEAK_LIST, CatWeakListClass))
#define CAT_WEAK_LIST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CAT_TYPE_WEAK_LIST, CatWeakListPrivate))


typedef struct _CatWeakList               CatWeakList;
typedef struct _CatWeakListPrivate        CatWeakListPrivate;
typedef struct _CatWeakListClass          CatWeakListClass;


struct _CatWeakList {
	GObject parent;
	CatWeakListPrivate *priv;
};

struct _CatWeakListClass {
	GObjectClass parent_class;
};

GType cat_weak_list_get_type();

/**
 * Creates a new empty CatWeakList instance. A CatWeakList allows the user to create a reference list of
 * weak-referenced objects.
 */
CatWeakList *cat_weak_list_new();

/**
 * Returns the number of weak-references in the list. The number is the raw number of elements in the internally used
 * map. This internally used map might contain already released references and this method does not traverse through
 * the list to check this.
 */
int cat_weak_list_size(CatWeakList *weak_list);

/**
 * Appends the given ref to this CatWeakList. If the ref is NULL then nothing is added.
 */
void cat_weak_list_append(CatWeakList *weak_list, GObject *ref);

/**
 * Appends the given ref to this CatWeakList but only if it was not already present in the list. If the ref is NULL
 * then nothing is added. The functions returns true if the entry was added and false otherwise.
 */
gboolean cat_weak_list_append_once(CatWeakList *weak_list, GObject *ref);


/**
 * Removes the first weak reference to the given ref searching the list backwards. The method returns TRUE if the ref
 * was present in the list and was removed, otherwise it will return FALSE.
 */
gboolean cat_weak_list_remove(CatWeakList *weak_list, GObject *ref);

/**
 * Enlist the references in the given array enlist_to. If enlist_to is NULL a new array will be created. The enlisted
 * will hold a strong reference through the CatArray.
 */
CatArrayWo *cat_weak_list_enlist(CatWeakList *weak_list, CatArrayWo *e_enlist_to);

/**
 * Creates an iterator of the references in the CatWeakList. Once the iterator is created any changes to the list will
 * not interfere with the iterator.
 */
CatIIterator *cat_weak_list_iterator(CatWeakList *weak_list);


G_END_DECLS

#endif /* CATWEAKLIST_H_ */
