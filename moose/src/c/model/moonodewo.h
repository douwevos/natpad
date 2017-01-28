/*
   File:    moonode.h
   Project: moose
   Author:  Douwe Vos
   Date:    May 11, 2013
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


#ifndef MOONODEWO_H_
#define MOONODEWO_H_

#include "moocontentmapwo.h"
#include "mooicontent.h"
#include "mooinodeexpander.h"
#include "moonodelistwo.h"
#include "access/mooinodematcher.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_NODE_WO              (moo_node_wo_get_type())
#define MOO_NODE_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_node_wo_get_type(), MooNodeWo))
#define MOO_NODE_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_NODE_WO, MooNodeWoClass))
#define MOO_IS_NODE_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_NODE_WO))
#define MOO_IS_NODE_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_NODE_WO))
#define MOO_NODE_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_NODE_WO, MooNodeWoClass))
#define MOO_NODE_WO_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_NODE_WO, MooNodeWoPrivate))


typedef struct _MooNodeWo               MooNodeWo;
typedef struct _MooNodeWoPrivate        MooNodeWoPrivate;
typedef struct _MooNodeWoClass          MooNodeWoClass;

struct _MooIdPath;

struct _MooNodeWo {
	GObject parent;
	MooNodeWoPrivate *priv;
};

struct _MooNodeWoClass {
	GObjectClass parent_class;
	MooNodeWo *(*clone)(MooNodeWo *to_clone);
};


GType moo_node_wo_get_type();

MooNodeWo *moo_node_wo_new(long long uniqueId, CatStringWo *c_name, MooNodeListWo *children, MooContentMapWo *content_map, int zorder);

void moo_node_wo_construct(MooNodeWo *node, long long uniqueId, CatStringWo *c_name, MooNodeListWo *children, MooContentMapWo *content_map, int zorder);

MooNodeWo *moo_node_wo_clone(MooNodeWo *to_clone);

MooNodeWo *moo_node_wo_ensure_editable(MooNodeWo *node, MooNodeWo *editing_parent);
MooNodeWo *moo_node_wo_get_original(MooNodeWo *node);
gboolean moo_node_wo_is_fixed(MooNodeWo *node);
MooNodeWo *moo_node_wo_anchor(MooNodeWo *node, int version);
gboolean moo_node_wo_equal(MooNodeWo *node_a, MooNodeWo *node_b);



MooINodeExpander *moo_node_wo_get_expander(MooNodeWo *node);
void moo_node_wo_set_expander(MooNodeWo *node, MooINodeExpander *expander);


MooNodeWo *moo_node_wo_get_parent_node_ref(MooNodeWo *e_node);

MooContentMapWo *moo_node_wo_get_content_map(MooNodeWo *node);
MooIContent *moo_node_wo_get_content(MooNodeWo *node, CatStringWo *key);
gboolean moo_node_wo_set_content(MooNodeWo *e_node, MooIContent *new_content);

long long moo_node_wo_get_unique_id(MooNodeWo *node);
int moo_node_wo_get_z_order(MooNodeWo *node);
void moo_node_wo_set_z_order(MooNodeWo *e_node, int new_z);
int moo_node_wo_get_version(MooNodeWo *node);
CatStringWo *moo_node_wo_get_name(MooNodeWo *node);
gboolean moo_node_wo_set_name(MooNodeWo *e_node, CatStringWo *new_name);

MooNodeListWo *moo_node_wo_get_children(MooNodeWo *node);
CatArrayWo *moo_node_wo_enlist_children(MooNodeWo *node, CatArrayWo *e_children);
int moo_node_wo_child_count(MooNodeWo *node);
MooNodeWo *moo_node_wo_child_at(MooNodeWo *node, int index);

void moo_node_wo_insert_child_at(MooNodeWo *e_node, int index, MooNodeWo *child);
void moo_node_wo_append_child(MooNodeWo *e_node, MooNodeWo *child);
MooNodeWo *moo_node_wo_get_editable_child_at(MooNodeWo *node, int index);
MooNodeListWo *moo_node_wo_get_editable_children(MooNodeWo *node);


int moo_node_wo_find_index(MooNodeWo *node, MooINodeMatcher *nodeMatcher, gpointer userdata, int cache_index);
struct _MooIdPath *moo_node_wo_find_path(MooNodeWo *node, long long unique_id);

void moo_node_wo_print(MooNodeWo *node, CatStringWo *e_buf);

gboolean moo_node_wo_equals(MooNodeWo *node_a, MooNodeWo *node_b);

int moo_node_wo_hash_code(MooNodeWo *node);

G_END_DECLS

#endif /* MOONODEWO_H_ */
