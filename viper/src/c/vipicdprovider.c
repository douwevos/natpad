/*
   File:    vipicdprovider.c
   Project: viper
   Author:  Douwe Vos
   Date:    Aug 9, 2014
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

#include "vipicdprovider.h"

G_DEFINE_INTERFACE(VipICdProvider, vip_icd_provider, G_TYPE_OBJECT);

static void vip_icd_provider_default_init (VipICdProviderInterface *klass) {
}

VipPath *vip_icd_provider_get_cd(VipICdProvider *self, CatStringWo *a_drive) {
	g_return_val_if_fail(VIP_IS_ICD_PROVIDER(self), NULL);
	return VIP_ICD_PROVIDER_GET_INTERFACE(self)->getCd(self, a_drive);
}

VipPath *vip_icd_provider_get_root(VipICdProvider *self, CatStringWo *a_drive) {
	g_return_val_if_fail(VIP_IS_ICD_PROVIDER(self), NULL);
	return VIP_ICD_PROVIDER_GET_INTERFACE(self)->getRoot(self, a_drive);
}
