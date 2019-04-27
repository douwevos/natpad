/*
   File:    terunicharnode.h
   Project: tern
   Author:  Douwe Vos
   Date:    Jun 2, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#ifndef TERUNICHARNODE_H_
#define TERUNICHARNODE_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define TER_TYPE_UNICHAR_NODE              (ter_unichar_node_get_type())
#define TER_UNICHAR_NODE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_unichar_node_get_type(), TerUnicharNode))
#define TER_UNICHAR_NODE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_UNICHAR_NODE, TerUnicharNodeClass))
#define TER_IS_UNICHAR_NODE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_UNICHAR_NODE))
#define TER_IS_UNICHAR_NODE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_UNICHAR_NODE))
#define TER_UNICHAR_NODE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_UNICHAR_NODE, TerUnicharNodeClass))

typedef struct _TerUnicharNode               TerUnicharNode;
typedef struct _TerUnicharNodePrivate        TerUnicharNodePrivate;
typedef struct _TerUnicharNodeClass          TerUnicharNodeClass;

struct _TerUnicharNode {
	GObject parent;
};

struct _TerUnicharNodeClass {
	GObjectClass parent_class;
};

GType ter_unichar_node_get_type();

TerUnicharNode *ter_unichar_node_new(gunichar unichar, gushort level);

TerUnicharNode *ter_unichar_node_get(TerUnicharNode *node, gunichar unichar);

TerUnicharNode *ter_unichar_node_get_or_create(TerUnicharNode *node, gunichar unichar);


void ter_unichar_node_set_keyword_ns1(TerUnicharNode *node, gushort code);

gushort ter_unichar_node_get_keyword_ns1(TerUnicharNode *node);

gushort ter_unichar_node_get_level(TerUnicharNode *node);

void ter_unichar_node_dump(TerUnicharNode *node, CatStringWo *a_indent);

G_END_DECLS

#endif /* TERUNICHARNODE_H_ */
