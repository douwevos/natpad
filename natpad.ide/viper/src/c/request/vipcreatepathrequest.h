/*
   File:    vipcreatepathrequest.h
   Project: viper
   Author:  Douwe Vos
   Date:    Feb 13, 2014
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

#ifndef VIPCREATEPATHREQUEST_H_
#define VIPCREATEPATHREQUEST_H_

#include "../vippath.h"
#include "../model/access/vipnodepath.h"
#include <caterpillar.h>
#include <worm.h>

G_BEGIN_DECLS

#define VIP_TYPE_CREATE_PATH_REQUEST              (vip_create_path_request_get_type())
#define VIP_CREATE_PATH_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_create_path_request_get_type(), VipCreatePathRequest))
#define VIP_CREATE_PATH_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_CREATE_PATH_REQUEST, VipCreatePathRequestClass))
#define VIP_IS_CREATE_PATH_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_CREATE_PATH_REQUEST))
#define VIP_IS_CREATE_PATH_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_CREATE_PATH_REQUEST))
#define VIP_CREATE_PATH_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_CREATE_PATH_REQUEST, VipCreatePathRequestClass))
#define VIP_CREATE_PATH_REQUEST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_CREATE_PATH_REQUEST, VipCreatePathRequestPrivate))


typedef struct _VipCreatePathRequest               VipCreatePathRequest;
typedef struct _VipCreatePathRequestPrivate        VipCreatePathRequestPrivate;
typedef struct _VipCreatePathRequestClass          VipCreatePathRequestClass;

struct _VipService;

struct _VipCreatePathRequest {
	WorRequest parent;
	VipCreatePathRequestPrivate *priv;
};

struct _VipCreatePathRequestClass {
	WorRequestClass parent_class;
};


GType vip_create_path_request_get_type();

VipCreatePathRequest *vip_create_path_request_new(struct _VipService *vip_service, VipPath *full_path);

VipNodePath *vip_create_path_request_wait_for_path(VipCreatePathRequest *request);

G_END_DECLS

#endif /* VIPCREATEPATHREQUEST_H_ */
