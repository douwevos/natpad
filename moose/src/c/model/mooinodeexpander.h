/*
   File:    mooinodeexpander.h
   Project: moose
   Author:  Douwe Vos
   Date:    Jun 28, 2014
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

#ifndef MOOINODEEXPANDER_H_
#define MOOINODEEXPANDER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_INODE_EXPANDER                 (moo_inode_expander_get_type())
#define MOO_INODE_EXPANDER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), MOO_TYPE_INODE_EXPANDER, MooINodeExpander))
#define MOO_IS_INODE_EXPANDER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), MOO_TYPE_INODE_EXPANDER))
#define MOO_INODE_EXPANDER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), MOO_TYPE_INODE_EXPANDER, MooINodeExpanderInterface))

typedef struct _MooINodeExpander               MooINodeExpander; /* dummy object */
typedef struct _MooINodeExpanderInterface      MooINodeExpanderInterface;

struct _MooNodeWo;

struct _MooINodeExpanderInterface {
	GTypeInterface parent_iface;

	gboolean (*expand)(MooINodeExpander *self, struct _MooNodeWo *moo_node);
	void (*collapse)(MooINodeExpander *self, struct _MooNodeWo *moo_node);
};

GType moo_inode_expander_get_type(void);

gboolean moo_inode_expander_expand(MooINodeExpander *self, struct _MooNodeWo *node);
void moo_inode_expander_collapse(MooINodeExpander *self, struct _MooNodeWo *node);

G_END_DECLS

#endif /* MOOINODEEXPANDER_H_ */
