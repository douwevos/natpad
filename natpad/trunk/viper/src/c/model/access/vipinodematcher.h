/*
   File:    vipinodematcher.h
   Project: viper
   Author:  Douwe Vos
   Date:    May 9, 2013
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


#ifndef VIPINODEMATCHER_H_
#define VIPINODEMATCHER_H_


#include <caterpillar.h>

#define VIP_TYPE_INODE_MATCHER                 (vip_inode_matcher_get_type())
#define VIP_INODE_MATCHER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), VIP_TYPE_INODE_MATCHER, VipINodeMatcher))
#define VIP_IS_INODE_MATCHER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), VIP_TYPE_INODE_MATCHER))
#define VIP_INODE_MATCHER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), VIP_TYPE_INODE_MATCHER, VipINodeMatcherInterface))


typedef struct _VipINodeMatcher               VipINodeMatcher; /* dummy object */
typedef struct _VipINodeMatcherInterface      VipINodeMatcherInterface;

struct _VipNode;

struct _VipINodeMatcherInterface {
	GTypeInterface parent_iface;

	gboolean (*matches)(VipINodeMatcher *self, struct _VipNode *node);
};

GType vip_inode_matcher_get_type(void);

gboolean vip_inode_matcher_matches(VipINodeMatcher *self, struct _VipNode *vipNode);


#endif /* VIPINODEMATCHER_H_ */
