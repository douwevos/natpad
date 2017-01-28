/*
   File:    cowtreeentry.h
   Project: cow
   Author:  Douwe Vos
   Date:    Aug 30, 2014
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

#ifndef COWTREEENTRY_H_
#define COWTREEENTRY_H_

#include "cowipanelfactory.h"
#include "cowpaneldescription.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define COW_TYPE_TREE_ENTRY              (cow_tree_entry_get_type())
#define COW_TREE_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cow_tree_entry_get_type(), CowTreeEntry))
#define COW_TREE_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), COW_TYPE_TREE_ENTRY, CowTreeEntryClass))
#define COW_IS_TREE_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), COW_TYPE_TREE_ENTRY))
#define COW_IS_TREE_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), COW_TYPE_TREE_ENTRY))
#define COW_TREE_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), COW_TYPE_TREE_ENTRY, CowTreeEntryClass))


typedef struct _CowTreeEntry               CowTreeEntry;
typedef struct _CowTreeEntryPrivate        CowTreeEntryPrivate;
typedef struct _CowTreeEntryClass          CowTreeEntryClass;


struct _CowTreeEntry {
	GObject parent;
};

struct _CowTreeEntryClass {
	GObjectClass parent_class;
};

GType cow_tree_entry_get_type();

CowTreeEntry *cow_tree_entry_new(CatStringWo *name);

CatStringWo *cow_tree_entry_get_name(CowTreeEntry *entry);

void cow_tree_entry_add_description(CowTreeEntry *entry, CowIPanelFactory *factory, CowPanelDescription *description);

int cow_tree_entry_count(CowTreeEntry *entry);
CowIPanelFactory *cow_tree_entry_factory_at(CowTreeEntry *entry, int index);
CowPanelDescription *cow_tree_entry_description_at(CowTreeEntry *entry, int index);


G_END_DECLS

#endif /* COWTREEENTRY_H_ */
