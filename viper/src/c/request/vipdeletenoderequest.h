/*
   File:    vipdeletenoderequest.h
   Project: viper
   Author:  Douwe Vos
   Date:    Apr 19, 2014
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

#ifndef VIPDELETENODEREQUEST_H_
#define VIPDELETENODEREQUEST_H_

#include <caterpillar.h>
#include <worm.h>

G_BEGIN_DECLS

#define VIP_TYPE_DELETE_NODE_REQUEST              (vip_delete_node_request_get_type())
#define VIP_DELETE_NODE_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_delete_node_request_get_type(), VipDeleteNodeRequest))
#define VIP_DELETE_NODE_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_DELETE_NODE_REQUEST, VipDeleteNodeRequestClass))
#define VIP_IS_DELETE_NODE_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_DELETE_NODE_REQUEST))
#define VIP_IS_DELETE_NODE_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_DELETE_NODE_REQUEST))
#define VIP_DELETE_NODE_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_DELETE_NODE_REQUEST, VipDeleteNodeRequestClass))
#define VIP_DELETE_NODE_REQUEST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_DELETE_NODE_REQUEST, VipDeleteNodeRequestPrivate))


typedef struct _VipDeleteNodeRequest               VipDeleteNodeRequest;
typedef struct _VipDeleteNodeRequestPrivate        VipDeleteNodeRequestPrivate;
typedef struct _VipDeleteNodeRequestClass          VipDeleteNodeRequestClass;

struct _VipService;

struct _VipDeleteNodeRequest {
	WorRequest parent;
	VipDeleteNodeRequestPrivate *priv;
};

struct _VipDeleteNodeRequestClass {
	WorRequestClass parent_class;
};


GType vip_delete_node_request_get_type();

VipDeleteNodeRequest *vip_delete_node_request_new(struct _VipService *vip_service, CatReadableTreeNode *node_to_rename);

gboolean vip_delete_node_request_wait_for_result(VipDeleteNodeRequest *request);

G_END_DECLS

#endif /* VIPDELETENODEREQUEST_H_ */
