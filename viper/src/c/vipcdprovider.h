/*
   File:    vipcdprovider.h
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

#ifndef VIPCDPROVIDER_H_
#define VIPCDPROVIDER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define VIP_TYPE_CD_PROVIDER              (vip_cd_provider_get_type())
#define VIP_CD_PROVIDER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_cd_provider_get_type(), VipCdProvider))
#define VIP_CD_PROVIDER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_CD_PROVIDER, VipCdProviderClass))
#define VIP_IS_CD_PROVIDER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_CD_PROVIDER))
#define VIP_IS_CD_PROVIDER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_CD_PROVIDER))
#define VIP_CD_PROVIDER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_CD_PROVIDER, VipCdProviderClass))


typedef struct _VipCdProvider               VipCdProvider;
typedef struct _VipCdProviderPrivate        VipCdProviderPrivate;
typedef struct _VipCdProviderClass          VipCdProviderClass;


struct _VipCdProvider {
	GObject parent;
};

struct _VipCdProviderClass {
	GObjectClass parent_class;
};


GType vip_cd_provider_get_type();

VipCdProvider *vip_cd_provider_new();

G_END_DECLS

#endif /* VIPCDPROVIDER_H_ */
