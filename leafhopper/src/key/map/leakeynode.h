/*
   File:    leakeynode.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 30, 2013
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

#ifndef LEAKEYNODE_H_
#define LEAKEYNODE_H_

#include "../leaikeybinding.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_KEY_NODE              (lea_key_node_get_type())
#define LEA_KEY_NODE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_key_node_get_type(), LeaKeyNode))
#define LEA_KEY_NODE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_KEY_NODE, LeaKeyNodeClass))
#define LEA_IS_KEY_NODE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_KEY_NODE))
#define LEA_IS_KEY_NODE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_KEY_NODE))
#define LEA_KEY_NODE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_KEY_NODE, LeaKeyNodeClass))

typedef struct _LeaKeyNode               LeaKeyNode;
typedef struct _LeaKeyNodePrivate        LeaKeyNodePrivate;
typedef struct _LeaKeyNodeClass          LeaKeyNodeClass;


struct _LeaKeyNode {
	GObject parent;
};

struct _LeaKeyNodeClass {
	GObjectClass parent_class;
	int (*hash)(LeaKeyNode *self);
	gboolean (*equal)(LeaKeyNode *node_a, LeaKeyNode *node_b);
};

GType lea_key_node_get_type();

void lea_key_node_construct(LeaKeyNode *key_node);
LeaKeyNode *lea_key_node_new();

LeaKeyNode *lea_key_node_add(LeaKeyNode *node, LeaKeyNode *child_node);
LeaKeyNode *lea_key_node_get(LeaKeyNode *node, LeaKeyNode *child_node);
void lea_key_node_remove(LeaKeyNode *node, LeaKeyNode *child_node);

gboolean lea_key_node_is_shallow(LeaKeyNode *node);

gboolean lea_key_node_has_children(LeaKeyNode *node);

void lea_key_node_enlist_children(LeaKeyNode *node, CatArrayWo *e_enlistto);

void lea_key_node_set_key_binding(LeaKeyNode *node, LeaIKeyBinding *key_binding);
LeaIKeyBinding *lea_key_node_get_key_binding(LeaKeyNode *node);

int lea_key_node_hash(LeaKeyNode *node);

gboolean lea_key_node_equal(LeaKeyNode *node_a, LeaKeyNode *node_b);

G_END_DECLS

#endif /* LEAKEYNODE_H_ */
