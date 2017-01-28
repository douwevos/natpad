/*
   File:    vipiscanwork.h
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


#ifndef VIPISCANWORK_H_
#define VIPISCANWORK_H_


#include "../vipnode.h"
#include <caterpillar.h>

#define VIP_TYPE_ISCAN_WORK                 (vip_iscan_work_get_type())
#define VIP_ISCAN_WORK(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), VIP_TYPE_ISCAN_WORK, VipIScanWork))
#define VIP_IS_ISCAN_WORK(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), VIP_TYPE_ISCAN_WORK))
#define VIP_ISCAN_WORK_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), VIP_TYPE_ISCAN_WORK, VipIScanWorkInterface))


typedef struct _VipIScanWork               VipIScanWork; /* dummy object */
typedef struct _VipIScanWorkInterface      VipIScanWorkInterface;

struct _VipMapperRegistry;


struct _VipIScanWorkInterface {
	GTypeInterface parent_iface;

	CatArrayWo *(*initChildWork)(VipIScanWork *self, struct _VipMapperRegistry *mapperRegistry);
	void (*runScan)(VipIScanWork *self);
};

GType vip_iscan_work_get_type(void);

#endif /* VIPISCANWORK_H_ */
