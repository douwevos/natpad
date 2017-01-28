/*
   File:    viprefreshnoderequest.h
   Project: viper
   Author:  Douwe Vos
   Date:    Mar 14, 2014
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

#ifndef VIPREFRESHNODEREQUEST_H_
#define VIPREFRESHNODEREQUEST_H_

#include <caterpillar.h>
#include <worm.h>

G_BEGIN_DECLS

#define VIP_TYPE_REFRESH_NODE_REQUEST              (vip_refresh_node_request_get_type())
#define VIP_REFRESH_NODE_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_refresh_node_request_get_type(), VipRefreshNodeRequest))
#define VIP_REFRESH_NODE_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_REFRESH_NODE_REQUEST, VipRefreshNodeRequestClass))
#define VIP_IS_REFRESH_NODE_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_REFRESH_NODE_REQUEST))
#define VIP_IS_REFRESH_NODE_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_REFRESH_NODE_REQUEST))
#define VIP_REFRESH_NODE_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_REFRESH_NODE_REQUEST, VipRefreshNodeRequestClass))
#define VIP_REFRESH_NODE_REQUEST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_REFRESH_NODE_REQUEST, VipRefreshNodeRequestPrivate))


typedef struct _VipRefreshNodeRequest               VipRefreshNodeRequest;
typedef struct _VipRefreshNodeRequestPrivate        VipRefreshNodeRequestPrivate;
typedef struct _VipRefreshNodeRequestClass          VipRefreshNodeRequestClass;

struct _VipService;

struct _VipRefreshNodeRequest {
	WorRequest parent;
	VipRefreshNodeRequestPrivate *priv;
};

struct _VipRefreshNodeRequestClass {
	WorRequestClass parent_class;
};


GType vip_refresh_node_request_get_type();

VipRefreshNodeRequest *vip_refresh_node_request_new(struct _VipService *vip_service, CatReadableTreeNode *node_to_refresh);

G_END_DECLS

#endif /* VIPREFRESHNODEREQUEST_H_ */
