/*
   File:    vipicdprovider.h
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

#ifndef VIPICDPROVIDER_H_
#define VIPICDPROVIDER_H_

#include "vippath.h"
#include <caterpillar.h>

#define VIP_TYPE_ICD_PROVIDER                 (vip_icd_provider_get_type())
#define VIP_ICD_PROVIDER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), VIP_TYPE_ICD_PROVIDER, VipICdProvider))
#define VIP_IS_ICD_PROVIDER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), VIP_TYPE_ICD_PROVIDER))
#define VIP_ICD_PROVIDER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), VIP_TYPE_ICD_PROVIDER, VipICdProviderInterface))


typedef struct _VipICdProvider               VipICdProvider; /* dummy object */
typedef struct _VipICdProviderInterface      VipICdProviderInterface;

struct _VipSnapshot;

struct _VipICdProviderInterface {
	GTypeInterface parent_iface;
	VipPath *(*getCd)(VipICdProvider *self, CatStringWo *a_drive);
	VipPath *(*getRoot)(VipICdProvider *self, CatStringWo *a_drive);
};

GType vip_icd_provider_get_type(void);

VipPath *vip_icd_provider_get_cd(VipICdProvider *self, CatStringWo *a_drive);
VipPath *vip_icd_provider_get_root(VipICdProvider *self, CatStringWo *a_drive);


#endif /* VIPICDPROVIDER_H_ */
