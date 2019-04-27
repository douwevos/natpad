/*
   File:    cattreeentrylist.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 4, 2014
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

#ifndef CATTREEENTRYLIST_H_
#define CATTREEENTRYLIST_H_

#include "cattreeentry.h"
#include "../../catimatcher.h"

G_BEGIN_DECLS

#define CAT_TYPE_TREE_ENTRY_LIST              (cat_tree_entry_list_get_type())
#define CAT_TREE_ENTRY_LIST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_tree_entry_list_get_type(), CatTreeEntryList))
#define CAT_TREE_ENTRY_LIST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_TREE_ENTRY_LIST, CatTreeEntryListClass))
#define CAT_IS_TREE_ENTRY_LIST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_TREE_ENTRY_LIST))
#define CAT_IS_TREE_ENTRY_LIST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_TREE_ENTRY_LIST))
#define CAT_TREE_ENTRY_LIST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_TREE_ENTRY_LIST, CatTreeEntryListClass))

typedef struct _CatTreeEntryList               CatTreeEntryList;
typedef struct _CatTreeEntryListPrivate        CatTreeEntryListPrivate;
typedef struct _CatTreeEntryListClass          CatTreeEntryListClass;

struct _CatTreeEntryList {
	GObject parent;
};

struct _CatTreeEntryListClass {
	GObjectClass parent_class;
};

GType cat_tree_entry_list_get_type();

CatTreeEntryList *cat_tree_entry_list_new();

void cat_tree_entry_list_mark(CatTreeEntryList *entry_list);

gboolean cat_tree_entry_list_is_marked_fixed(CatTreeEntryList *entry_list);

CatTreeEntryList *cat_tree_entry_list_ensure_writable(CatTreeEntryList *entry_list);

int cat_tree_entry_list_create_node(CatTreeEntryList *entry_list);


void cat_tree_entry_list_remove_entry(CatTreeEntryList *entry_list, int entry_location, gboolean recursive);

CatTreeEntry *cat_tree_entry_list_get_entry(CatTreeEntryList *entry_list, int entry_location);

CatTreeEntry *cat_tree_entry_list_get_writable_entry(CatTreeEntryList *entry_list, int entry_location);

int cat_tree_entry_list_find_location(CatTreeEntryList *entry_list, CatIMatcher *matcher, int guess_location);

G_END_DECLS

#endif /* CATTREEENTRYLIST_H_ */
