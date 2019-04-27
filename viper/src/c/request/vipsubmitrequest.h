/*
   File:    vipsubmitrequest.h
   Project: viper
   Author:  Douwe Vos
   Date:    Mar 15, 2014
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

#ifndef VIPSUBMITREQUEST_H_
#define VIPSUBMITREQUEST_H_

#include <caterpillar.h>
#include <worm.h>

G_BEGIN_DECLS

#define VIP_TYPE_SUBMIT_REQUEST              (vip_submit_request_get_type())
#define VIP_SUBMIT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_submit_request_get_type(), VipSubmitRequest))
#define VIP_SUBMIT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_SUBMIT_REQUEST, VipSubmitRequestClass))
#define VIP_IS_SUBMIT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_SUBMIT_REQUEST))
#define VIP_IS_SUBMIT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_SUBMIT_REQUEST))
#define VIP_SUBMIT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_SUBMIT_REQUEST, VipSubmitRequestClass))

typedef struct _VipSubmitRequest               VipSubmitRequest;
typedef struct _VipSubmitRequestPrivate        VipSubmitRequestPrivate;
typedef struct _VipSubmitRequestClass          VipSubmitRequestClass;

struct _VipService;

struct _VipSubmitRequest {
	WorRequest parent;
};

struct _VipSubmitRequestClass {
	WorRequestClass parent_class;
};


GType vip_submit_request_get_type();

VipSubmitRequest *vip_submit_request_new(struct _VipService *vip_service);

G_END_DECLS

#endif /* VIPSUBMITREQUEST_H_ */
