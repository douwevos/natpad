/*
   File:    xandomnode.h
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 13, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef OUTLINE_DOM_XANDOMNODE_H_
#define OUTLINE_DOM_XANDOMNODE_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define XAN_TYPE_DOM_NODE              (xan_dom_node_get_type())
#define XAN_DOM_NODE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), xan_dom_node_get_type(), XanDomNode))
#define XAN_DOM_NODE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), XAN_TYPE_DOM_NODE, XanDomNodeClass))
#define XAN_IS_DOM_NODE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAN_TYPE_DOM_NODE))
#define XAN_IS_DOM_NODE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), XAN_TYPE_DOM_NODE))
#define XAN_DOM_NODE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), XAN_TYPE_DOM_NODE, XanDomNodeClass))


typedef struct _XanDomNode               XanDomNode;
typedef struct _XanDomNodePrivate        XanDomNodePrivate;
typedef struct _XanDomNodeClass          XanDomNodeClass;


struct _XanDomNode {
	GObject parent;
};

struct _XanDomNodeClass {
	GObjectClass parent_class;
};


GType xan_dom_node_get_type();

XanDomNode *xan_dom_node_new();

G_END_DECLS

#endif /* OUTLINE_DOM_XANDOMNODE_H_ */
