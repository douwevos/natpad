/*
   File:    cowipanelfactory.c
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

#include "cowipanelfactory.h"

G_DEFINE_INTERFACE(CowIPanelFactory, cow_ipanel_factory, G_TYPE_OBJECT);

static void cow_ipanel_factory_default_init (CowIPanelFactoryInterface *iface) {
	iface->create = NULL;
}

CowPanel *cow_ipanel_factory_create(CowIPanelFactory *self, struct _CowDialog *cow_dialog, CowPanelDescription *panel_descr) {
	g_return_val_if_fail(COW_IS_IPANEL_FACTORY(self), NULL);
	CowIPanelFactoryInterface *iface = COW_IPANEL_FACTORY_GET_INTERFACE(self);
	if (iface->create) {
		return iface->create(self, cow_dialog, panel_descr);
	}
	return NULL;
}
