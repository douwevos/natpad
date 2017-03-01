/*
   File:    turinode.h
   Project: turtle
   Author:  Douwe Vos
   Date:    Feb 12, 2017
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

#ifndef MODEL_TURINODE_H_
#define MODEL_TURINODE_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define TUR_TYPE_INODE                 (tur_inode_get_type())
#define TUR_INODE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), TUR_TYPE_INODE, TurINode))
#define TUR_IS_INODE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), TUR_TYPE_INODE))
#define TUR_INODE_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), TUR_TYPE_INODE, TurINodeInterface))


typedef struct _TurINode               TurINode; /* dummy object */
typedef struct _TurINodeInterface      TurINodeInterface;


struct _TurINodeInterface {
	GTypeInterface parent_iface;
	CatArrayWo *(*enlist)(TurINode *node, CatArrayWo *enlist_in);
	gboolean (*hasChildren)(TurINode *node);
	CatStringWo *(*name)(TurINode *node);
	int (*id)(TurINode *node);
	int (*version)(TurINode *node);
	void (*hold)(TurINode *node);
	void (*release)(TurINode *node);
};

GType tur_inode_get_type(void);

G_END_DECLS
#endif /* MODEL_TURINODE_H_ */
