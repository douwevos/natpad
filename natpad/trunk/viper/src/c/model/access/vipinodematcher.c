/*
   File:    vipinodematcher.c
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


#include "vipinodematcher.h"

G_DEFINE_INTERFACE(VipINodeMatcher, vip_inode_matcher, G_TYPE_OBJECT);

static void vip_inode_matcher_default_init (VipINodeMatcherInterface *klass) {
}


gboolean vip_inode_matcher_matches(VipINodeMatcher *self, struct _VipNode *vipNode) {
	g_return_val_if_fail(VIP_IS_INODE_MATCHER(self), FALSE);
	return VIP_INODE_MATCHER_GET_INTERFACE(self)->matches(self, vipNode);
}
