/*
   File:    cowipanelfactory.h
   Project: cow
   Author:  Douwe Vos
   Date:    Aug 30, 2014
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

#ifndef COWIPANELFACTORY_H_
#define COWIPANELFACTORY_H_

#include "../cowcontainer.h"
#include "cowpanel.h"
#include "cowpaneldescription.h"
#include <caterpillar.h>

#define COW_TYPE_IPANEL_FACTORY                 (cow_ipanel_factory_get_type())
#define COW_IPANEL_FACTORY(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), COW_TYPE_IPANEL_FACTORY, CowIPanelFactory))
#define COW_IS_IPANEL_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), COW_TYPE_IPANEL_FACTORY))
#define COW_IPANEL_FACTORY_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), COW_TYPE_IPANEL_FACTORY, CowIPanelFactoryInterface))

typedef struct _CowIPanelFactory               CowIPanelFactory; /* dummy object */
typedef struct _CowIPanelFactoryInterface      CowIPanelFactoryInterface;

struct _CowDialog;
struct _CowPanelDescription;

struct _CowIPanelFactoryInterface {
	GTypeInterface parent_iface;
	void (*attach)(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowContainer *e_container);
	void (*detach)(CowIPanelFactory *self, struct _CowDialog *cow_dialog);
	CowPanel *(*create)(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowPanelDescription *panel_descr);
};

GType cow_ipanel_factory_get_type(void);


CowPanel *cow_ipanel_factory_create(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowPanelDescription *panel_descr);

#endif /* COWIPANELFACTORY_H_ */
