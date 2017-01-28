/*
   File:    vipimap.c
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



#include "vipimap.h"
#include "vipiresource.h"

G_DEFINE_INTERFACE(VipIMap, vip_imap, VIP_TYPE_IRESOURCE);

static void vip_imap_default_init (VipIMapInterface *iface) {
	iface->createFile = NULL;
}

CatArrayWo *vip_imap_enlist(VipIMap *self) {
	g_return_val_if_fail(VIP_IS_IMAP(self), NULL);
	return VIP_IMAP_GET_INTERFACE(self)->enlist(self);
}

VipIFile *vip_imap_create_file(VipIMap *self, CatStringWo *new_file_name) {
	g_return_val_if_fail(VIP_IS_IMAP(self), NULL);
	VipIMapInterface *iface = VIP_IMAP_GET_INTERFACE(self);
	if (iface->createFile) {
		return iface->createFile(self, new_file_name);
	}
	return NULL;
}
