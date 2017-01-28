/*
   File:    vipimapper.h
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


#ifndef VIPIMAPPER_H_
#define VIPIMAPPER_H_

#include "vipiscanwork.h"
#include "../vipnode.h"
#include <caterpillar.h>

#define VIP_TYPE_IMAPPER                 (vip_imapper_get_type())
#define VIP_IMAPPER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), VIP_TYPE_IMAPPER, VipIMapper))
#define VIP_IS_IMAPPER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), VIP_TYPE_IMAPPER))
#define VIP_IMAPPER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), VIP_TYPE_IMAPPER, VipIMapperInterface))


typedef struct _VipIMapper               VipIMapper; /* dummy object */
typedef struct _VipIMapperInterface      VipIMapperInterface;



struct _VipIMapperInterface {
	GTypeInterface parent_iface;
	VipIScanWork *(*createWorkForNode)(VipIMapper *self, CatWritableTreeNode *node, gboolean recursive_from_parent, gboolean validated_by_parent);
};

GType vip_imapper_get_type(void);

VipIScanWork *vip_imapper_create_work_for_node(VipIMapper *self, CatWritableTreeNode *node, gboolean recursive_from_parent, gboolean validated_by_parent);


#endif /* VIPIMAPPER_H_ */
