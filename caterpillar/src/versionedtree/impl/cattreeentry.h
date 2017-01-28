/*
   File:    cattreeentry.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 2, 2014
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

#ifndef CATTREEENTRY_H_
#define CATTREEENTRY_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_TREE_ENTRY              (cat_tree_entry_get_type())
#define CAT_TREE_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_tree_entry_get_type(), CatTreeEntry))
#define CAT_TREE_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_TREE_ENTRY, CatTreeEntryClass))
#define CAT_IS_TREE_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_TREE_ENTRY))
#define CAT_IS_TREE_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_TREE_ENTRY))
#define CAT_TREE_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_TREE_ENTRY, CatTreeEntryClass))
#define CAT_TREE_ENTRY_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), CAT_TYPE_TREE_ENTRY, CatTreeEntryPrivate))


typedef struct _CatTreeEntry               CatTreeEntry;
typedef struct _CatTreeEntryPrivate        CatTreeEntryPrivate;
typedef struct _CatTreeEntryClass          CatTreeEntryClass;


struct _CatTreeEntry {
	GObject parent;
	CatTreeEntryPrivate *priv;
};

struct _CatTreeEntryClass {
	GObjectClass parent_class;
};


GType cat_tree_entry_get_type();

CatTreeEntry *cat_tree_entry_new();

void cat_tree_entry_mark(CatTreeEntry *entry);

gboolean cat_tree_entry_is_marked_fixed(CatTreeEntry *entry);

CatTreeEntry *cat_tree_entry_ensure_writable(CatTreeEntry *entry);

void cat_tree_entry_set_parent_location(CatTreeEntry *entry, int location);

int cat_tree_entry_get_parent_location(CatTreeEntry *entry);

int cat_tree_entry_find_child_index(CatTreeEntry *entry, int location);


void cat_tree_entry_add_child(CatTreeEntry *entry, int location);

void cat_tree_entry_set_child_at(CatTreeEntry *entry, int index, int location);

int cat_tree_entry_removed_child_at(CatTreeEntry *entry, int index);


int cat_tree_entry_get_child(CatTreeEntry *entry, int index);

int cat_tree_entry_count(CatTreeEntry *entry);


void cat_tree_entry_set_payload(CatTreeEntry *entry, GObject *payload);

GObject *cat_tree_entry_get_payload(CatTreeEntry *entry);

gboolean cat_tree_entry_references_location(CatTreeEntry *entry, int location);

void cat_tree_entry_writable_move_location(CatTreeEntry *entry, int src_location, int dest_location);

G_END_DECLS

#endif /* CATTREEENTRY_H_ */
