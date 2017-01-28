/*
   File:    mooinodework.c
   Project: moose
   Author:  Douwe Vos
   Date:    May 14, 2013
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

#include "mooinodework.h"

G_DEFINE_INTERFACE(MooINodeWork, moo_inode_work, G_TYPE_OBJECT);

static void moo_inode_work_default_init (MooINodeWorkInterface *klass) {
}


void moo_inode_work_do_work(MooINodeWork *self, CatArrayWo *e_work_list) {
	g_return_if_fail(MOO_IS_INODE_WORK(self));
	MOO_INODE_WORK_GET_INTERFACE(self)->doWork(self, e_work_list);
}
