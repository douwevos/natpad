/*
   File:    viprenamenoderequest.h
   Project: viper
   Author:  Douwe Vos
   Date:    Apr 5, 2014
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

#ifndef VIPRENAMENODEREQUEST_H_
#define VIPRENAMENODEREQUEST_H_

#include <caterpillar.h>
#include <worm.h>

G_BEGIN_DECLS

#define VIP_TYPE_RENAME_NODE_REQUEST              (vip_rename_node_request_get_type())
#define VIP_RENAME_NODE_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_rename_node_request_get_type(), VipRenameNodeRequest))
#define VIP_RENAME_NODE_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_RENAME_NODE_REQUEST, VipRenameNodeRequestClass))
#define VIP_IS_RENAME_NODE_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_RENAME_NODE_REQUEST))
#define VIP_IS_RENAME_NODE_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_RENAME_NODE_REQUEST))
#define VIP_RENAME_NODE_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_RENAME_NODE_REQUEST, VipRenameNodeRequestClass))

typedef struct _VipRenameNodeRequest               VipRenameNodeRequest;
typedef struct _VipRenameNodeRequestPrivate        VipRenameNodeRequestPrivate;
typedef struct _VipRenameNodeRequestClass          VipRenameNodeRequestClass;

struct _VipService;

struct _VipRenameNodeRequest {
	WorRequest parent;
};

struct _VipRenameNodeRequestClass {
	WorRequestClass parent_class;
};


GType vip_rename_node_request_get_type();

VipRenameNodeRequest *vip_rename_node_request_new(struct _VipService *vip_service, CatReadableTreeNode *node_to_rename, CatStringWo *a_new_name);

gboolean vip_rename_node_request_wait_for_result(VipRenameNodeRequest *request);

G_END_DECLS

#endif /* VIPRENAMENODEREQUEST_H_ */
