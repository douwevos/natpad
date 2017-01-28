/*
   File:    vipiresource.c
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


#include "vipiresource.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "VipIResource"
#include <logging/catlog.h>

G_DEFINE_INTERFACE(VipIResource, vip_iresource, G_TYPE_OBJECT);

static void vip_iresource_default_init (VipIResourceInterface *iface) {
	iface->rename = NULL;
	iface->canRename = NULL;
	iface->canDelete = NULL;
	iface->path = NULL;
}

CatStringWo *vip_iresource_get_name(VipIResource *self) {
	g_return_val_if_fail(VIP_IS_IRESOURCE(self), NULL);
	return VIP_IRESOURCE_GET_INTERFACE(self)->getName(self);
}

long long vip_iresource_last_modified(VipIResource *self) {
	g_return_val_if_fail(VIP_IS_IRESOURCE(self), 0);
	return VIP_IRESOURCE_GET_INTERFACE(self)->lastModified(self);
}

VipIResource *vip_iresource_rename(VipIResource *self, CatStringWo *new_name) {
	g_return_val_if_fail(VIP_IS_IRESOURCE(self), NULL);
	VipIResourceInterface *iface = VIP_IRESOURCE_GET_INTERFACE(self);
	if (iface->rename) {
		return iface->rename(self, new_name);
	}
	return NULL;
}

gboolean vip_iresource_can_rename(VipIResource *self) {
	g_return_val_if_fail(VIP_IS_IRESOURCE(self), FALSE);
	VipIResourceInterface *iface = VIP_IRESOURCE_GET_INTERFACE(self);
	if (iface->canRename) {
		return iface->canRename(self);
	}
	return FALSE;
}


gboolean vip_iresource_delete(VipIResource *self) {
	g_return_val_if_fail(VIP_IS_IRESOURCE(self), FALSE);
	VipIResourceInterface *iface = VIP_IRESOURCE_GET_INTERFACE(self);
	if (iface->delete) {
		return iface->delete(self);
	}
	return FALSE;
}

gboolean vip_iresource_can_delete(VipIResource *self) {
	g_return_val_if_fail(VIP_IS_IRESOURCE(self), FALSE);
	VipIResourceInterface *iface = VIP_IRESOURCE_GET_INTERFACE(self);
	if (iface->canDelete) {
		return iface->canDelete(self);
	}
	return FALSE;
}

VipPath *vip_iresource_path(VipIResource *self) {
	g_return_val_if_fail(VIP_IS_IRESOURCE(self), NULL);
	VipIResourceInterface *iface = VIP_IRESOURCE_GET_INTERFACE(self);
	if (iface->path) {
		return iface->path(self);
	}
	return NULL;
}

gboolean vip_iresource_equal(VipIResource *res_a, VipIResource *res_b) {
	if (res_a==res_b) {
		return TRUE;
	}
	if (res_a==NULL || res_b==NULL) {
		return FALSE;
	}
	VipIResourceInterface *iface = VIP_IRESOURCE_GET_INTERFACE(res_a);
	return iface->equal(res_a, res_b);
}

