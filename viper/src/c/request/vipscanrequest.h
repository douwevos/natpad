/*
   File:    vipscanrequest.h
   Project: viper
   Author:  Douwe Vos
   Date:    Feb 8, 2014
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

#ifndef VIPSCANREQUEST_H_
#define VIPSCANREQUEST_H_

#include "../vipservice.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define VIP_TYPE_SCAN_REQUEST              (vip_scan_request_get_type())
#define VIP_SCAN_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_scan_request_get_type(), VipScanRequest))
#define VIP_SCAN_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_SCAN_REQUEST, VipScanRequestClass))
#define VIP_IS_SCAN_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_SCAN_REQUEST))
#define VIP_IS_SCAN_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_SCAN_REQUEST))
#define VIP_SCAN_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_SCAN_REQUEST, VipScanRequestClass))
#define VIP_SCAN_REQUEST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_SCAN_REQUEST, VipScanRequestPrivate))


typedef struct _VipScanRequest               VipScanRequest;
typedef struct _VipScanRequestPrivate        VipScanRequestPrivate;
typedef struct _VipScanRequestClass          VipScanRequestClass;


struct _VipScanRequest {
	WorRequest parent;
	VipScanRequestPrivate *priv;
};

struct _VipScanRequestClass {
	WorRequestClass parent_class;
};


GType vip_scan_request_get_type();

VipScanRequest *vip_scan_request_new(VipService *vip_service);

G_END_DECLS

#endif /* VIPSCANREQUEST_H_ */
