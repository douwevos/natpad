/*
   File:    dragroupsearch.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Sep 2, 2015
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

#ifndef ACTION_DRAGROUPSEARCH_H_
#define ACTION_DRAGROUPSEARCH_H_

#include "../draeditorpanel.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define DRA_TYPE_GROUP_SEARCH              (dra_group_search_get_type())
#define DRA_GROUP_SEARCH(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_group_search_get_type(), DraGroupSearch))
#define DRA_GROUP_SEARCH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_GROUP_SEARCH, DraGroupSearchClass))
#define DRA_IS_GROUP_SEARCH(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_GROUP_SEARCH))
#define DRA_IS_GROUP_SEARCH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_GROUP_SEARCH))
#define DRA_GROUP_SEARCH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_GROUP_SEARCH, DraGroupSearchClass))


typedef struct _DraGroupSearch               DraGroupSearch;
typedef struct _DraGroupSearchPrivate        DraGroupSearchPrivate;
typedef struct _DraGroupSearchClass          DraGroupSearchClass;


struct _DraGroupSearch {
	LeaActionGroup parent;
};

struct _DraGroupSearchClass {
	LeaActionGroupClass parent_class;
};


GType dra_group_search_get_type();

DraGroupSearch *dra_group_search_new(LeaKeyContext *key_context, gpointer clipboard_handler);

void dra_group_search_set_editor_panel(DraGroupSearch *group_search, DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* ACTION_DRAGROUPSEARCH_H_ */
