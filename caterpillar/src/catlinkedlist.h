/*
   File:    catlinkedlist.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Jun 22, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef SRC_CATLINKEDLIST_H_
#define SRC_CATLINKEDLIST_H_

#include "catiiterator.h"
#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_LINKED_LIST              (cat_linked_list_get_type())
#define CAT_LINKED_LIST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_linked_list_get_type(), CatLinkedList))
#define CAT_LINKED_LIST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_LINKED_LIST, CatLinkedListClass))
#define CAT_IS_LINKED_LIST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_LINKED_LIST))
#define CAT_IS_LINKED_LIST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_LINKED_LIST))
#define CAT_LINKED_LIST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_LINKED_LIST, CatLinkedListClass))


typedef struct _CatLinkedList               CatLinkedList;
typedef struct _CatLinkedListPrivate        CatLinkedListPrivate;
typedef struct _CatLinkedListClass          CatLinkedListClass;


struct _CatLinkedList {
	GObject parent;
};

struct _CatLinkedListClass {
	GObjectClass parent_class;
};


GType cat_linked_list_get_type();

CatLinkedList *cat_linked_list_new();

int cat_linked_list_size(CatLinkedList *linked_list);

void cat_linked_list_append(CatLinkedList *linked_list, GObject *data);
void cat_linked_list_prepend(CatLinkedList *linked_list, GObject *data);

gboolean cat_linked_list_remove(CatLinkedList *linked_list, GObject *data);
gboolean cat_linked_list_remove_last(CatLinkedList *linked_list, GObject **removed);
gboolean cat_linked_list_remove_first(CatLinkedList *linked_list, GObject **removed);
void cat_linked_listed_remove_tail(CatLinkedList *linked_list, int from);

CatIIterator *cat_linked_list_iterator(CatLinkedList *linked_list);

GObject *cat_linked_list_get(CatLinkedList *linked_list, int index);

G_END_DECLS

#endif /* SRC_CATLINKEDLIST_H_ */
