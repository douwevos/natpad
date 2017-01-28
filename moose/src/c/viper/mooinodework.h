/*
   File:    mooinodework.h
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


#ifndef MOOINODEWORK_H_
#define MOOINODEWORK_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_INODE_WORK                 (moo_inode_work_get_type())
#define MOO_INODE_WORK(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), MOO_TYPE_INODE_WORK, MooINodeWork))
#define MOO_IS_INODE_WORK(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), MOO_TYPE_INODE_WORK))
#define MOO_INODE_WORK_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), MOO_TYPE_INODE_WORK, MooINodeWorkInterface))


typedef struct _MooINodeWork               MooINodeWork; /* dummy object */
typedef struct _MooINodeWorkInterface      MooINodeWorkInterface;


struct _MooTransaction;

struct _MooINodeWorkInterface {
	GTypeInterface parent_iface;
	void (*doWork)(MooINodeWork *self, CatArrayWo *e_work_list);
};

GType moo_inode_work_get_type(void);

void moo_inode_work_do_work(MooINodeWork *self, CatArrayWo *e_work_list);

G_END_DECLS

#endif /* MOOINODEWORK_H_ */
