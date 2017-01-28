/*
   File:    vipservice.h
   Project: viper
   Author:  Douwe Vos
   Date:    Jun 6, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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


#ifndef VIPSERVICE_H_
#define VIPSERVICE_H_

#include "vippath.h"
#include "vipilistener.h"
#include "vipicdprovider.h"
#include "vipcdprovider.h"
#include "request/vipcreatepathrequest.h"
#include "request/vipcopyormoverequest.h"
#include "request/viprenamenoderequest.h"
#include "request/vipdeletenoderequest.h"
#include "model/vipsnapshot.h"
#include "model/access/vipnodepath.h"
#include "model/scanner/vipmapperregistry.h"
#include <caterpillar.h>
#include <worm.h>

G_BEGIN_DECLS

#define VIP_TYPE_SERVICE              (vip_service_get_type())
#define VIP_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_service_get_type(), VipService))
#define VIP_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_SERVICE, VipServiceClass))
#define VIP_IS_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_SERVICE))
#define VIP_IS_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_SERVICE))
#define VIP_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_SERVICE, VipServiceClass))
#define VIP_SERVICE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_SERVICE, VipServicePrivate))

typedef struct _VipService               VipService;
typedef struct _VipServicePrivate        VipServicePrivate;
typedef struct _VipServiceClass          VipServiceClass;


struct _VipService {
	GObject parent;
	VipServicePrivate *priv;
};

struct _VipServiceClass {
	GObjectClass parent_class;
};


GType vip_service_get_type();

VipService *vip_service_new(WorService *wor_service, VipCdProvider *cd_provider);

//VipResourceMap *vip_service_get_resource_map(VipService *vip_service);

//gboolean vip_service_delete_resource(VipService *vip_service, VipResource *resource);

//gboolean vip_service_move_resource(VipService *vip_service, VipResource *source, VipResource *destination);

//gboolean vip_service_copy_resource(VipService *vip_service, VipResource *source, VipResource *destination);

//gboolean vip_service_rename_resource(VipService *vip_service, VipResource *source, CatStringWo *a_new_name);

//gboolean vip_service_new_folder(VipService *vip_service, VipResource *source, CatStringWo *a_new_folder_name);

//gboolean vip_service_create_directory(VipService *vip_service, VipLockList *lock_list, VipResource *new_directory);



 /** new **/

VipPath *vip_service_get_current_work_directory(VipService *service);

VipICdProvider *vip_service_get_cd_provider(VipService *service);

void vip_service_start(VipService *service);

void vip_service_post(VipService *service, WorRequest *request);

void vip_service_scan(VipService *service);

void vip_service_refresh_node(VipService *service, CatReadableTreeNode *node_to_refresh);
VipRenameNodeRequest *vip_service_rename_node(VipService *service, CatReadableTreeNode *node_to_rename, CatStringWo *new_name);

VipDeleteNodeRequest *vip_service_delete_node(VipService *service, CatReadableTreeNode *node_to_delete);

void vip_service_submit(VipService *service, gboolean now);

VipCreatePathRequest *vip_service_create_path(VipService *service, VipPath *full_path);

VipCopyOrMoveRequest *vip_service_copy_or_move_to(VipService *service, CatArrayWo *paths_to_copy, gboolean move, CatReadableTreeNode *destination_vip);

CatTree *vip_service_get_tree(VipService *service);

gboolean vip_service_set_snapshot(VipService *service, VipSnapshot *new_snapshot);

VipSnapshot *vip_service_get_snapshot(VipService *service);

VipMapperRegistry *vip_service_get_mapper_registry(VipService *service);


void vip_service_add_listener(VipService *service, VipIListener *listener);


void vip_service_dump_tree(VipService *service);

G_END_DECLS

#endif /* VIPSERVICE_H_ */
