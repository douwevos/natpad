/*
   File:    mooinodematcher.h
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


#ifndef MOOINODEMATCHER_H_
#define MOOINODEMATCHER_H_

#include <caterpillar.h>

#define MOO_TYPE_INODE_MATCHER                 (moo_inode_matcher_get_type())
#define MOO_INODE_MATCHER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), MOO_TYPE_INODE_MATCHER, MooINodeMatcher))
#define MOO_IS_INODE_MATCHER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), MOO_TYPE_INODE_MATCHER))
#define MOO_INODE_MATCHER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), MOO_TYPE_INODE_MATCHER, MooINodeMatcherInterface))


typedef struct _MooINodeMatcher               MooINodeMatcher; /* dummy object */
typedef struct _MooINodeMatcherInterface      MooINodeMatcherInterface;


struct _MooNodeWo;

struct _MooINodeMatcherInterface {
	GTypeInterface parent_iface;
	gboolean (*matches)(MooINodeMatcher *self, struct _MooNodeWo *node, gpointer userdata);
};

GType moo_inode_matcher_get_type(void);


#endif /* MOOINODEMATCHER_H_ */
