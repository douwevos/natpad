/*
   File:    mooinodematcher.c
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



#include "mooinodematcher.h"

G_DEFINE_INTERFACE(MooINodeMatcher, moo_inode_matcher, G_TYPE_OBJECT);

static void moo_inode_matcher_default_init (MooINodeMatcherInterface *klass) {
}


//long long moo_inode_matcher_next(MooINodeMatcher *self) {
//	g_return_val_if_fail(MOO_IS_INODE_MATCHER(self), 0);
//	return MOO_INODE_MATCHER_GET_INTERFACE(self)->next(self);
//}

