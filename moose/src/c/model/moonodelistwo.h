/*
   File:    moonodelistwo.h
   Project: moose
   Author:  Douwe Vos
   Date:    May 9, 2014
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

#ifndef MOONODELISTWO_H_
#define MOONODELISTWO_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_NODE_LIST_WO              (moo_node_list_wo_get_type())
#define MOO_NODE_LIST_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_node_list_wo_get_type(), MooNodeListWo))
#define MOO_NODE_LIST_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_NODE_LIST_WO, MooNodeListWoClass))
#define MOO_IS_NODE_LIST_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_NODE_LIST_WO))
#define MOO_IS_NODE_LIST_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_NODE_LIST_WO))
#define MOO_NODE_LIST_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_NODE_LIST_WO, MooNodeListWoClass))


typedef struct _MooNodeListWo               MooNodeListWo;
typedef struct _MooNodeListWoPrivate        MooNodeListWoPrivate;
typedef struct _MooNodeListWoClass          MooNodeListWoClass;

struct _MooNodeWo;

struct _MooNodeListWo {
	GObject parent;
};

struct _MooNodeListWoClass {
	GObjectClass parent_class;
};


GType moo_node_list_wo_get_type();

MooNodeListWo *moo_node_list_wo_new();

MooNodeListWo *moo_node_list_wo_ensure_editable(MooNodeListWo *node_list);
gboolean moo_node_list_wo_is_fixed(MooNodeListWo *node_list);
MooNodeListWo *moo_node_list_wo_anchor(MooNodeListWo *node_list, int version);
gboolean moo_node_list_wo_equal(MooNodeListWo *list_a, MooNodeListWo *list_b);

CatArrayWo *moo_node_list_wo_enlist(MooNodeListWo *node_list, CatArrayWo *e_enlist_to);

struct _MooNodeWo *moo_node_list_wo_get_at(MooNodeListWo *node_list, int index);

gboolean moo_node_list_wo_set_at(MooNodeListWo *e_node_list, struct _MooNodeWo *node, int index);

struct _MooNodeWo *moo_node_list_wo_get_first(MooNodeListWo *node_list, int index);

struct _MooNodeWo *moo_node_list_wo_get_last(MooNodeListWo *node_list, int index);

int moo_node_list_wo_count(MooNodeListWo *node_list);

CatIIterator *moo_node_list_wo_iterator(MooNodeListWo *node_list);

void moo_node_list_wo_append(MooNodeListWo *node_list, struct _MooNodeWo *node);

void moo_node_list_wo_insert(MooNodeListWo *node_list, int index, struct _MooNodeWo *node);

void moo_node_list_wo_remove(MooNodeListWo *node_list, int index);

void moo_node_list_wo_clear(MooNodeListWo *node_list);

G_END_DECLS

#endif /* MOONODELISTWO_H_ */
