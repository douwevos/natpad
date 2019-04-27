/*
   File:    vipwritefilerequest.h
   Project: viper
   Author:  Douwe Vos
   Date:    Apr 13, 2014
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

#ifndef VIPWRITEFILEREQUEST_H_
#define VIPWRITEFILEREQUEST_H_

#include <caterpillar.h>
#include <worm.h>

G_BEGIN_DECLS

#define VIP_TYPE_WRITE_FILE_REQUEST              (vip_write_file_request_get_type())
#define VIP_WRITE_FILE_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_write_file_request_get_type(), VipWriteFileRequest))
#define VIP_WRITE_FILE_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_WRITE_FILE_REQUEST, VipWriteFileRequestClass))
#define VIP_IS_WRITE_FILE_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_WRITE_FILE_REQUEST))
#define VIP_IS_WRITE_FILE_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_WRITE_FILE_REQUEST))
#define VIP_WRITE_FILE_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_WRITE_FILE_REQUEST, VipWriteFileRequestClass))

typedef struct _VipWriteFileRequest               VipWriteFileRequest;
typedef struct _VipWriteFileRequestPrivate        VipWriteFileRequestPrivate;
typedef struct _VipWriteFileRequestClass          VipWriteFileRequestClass;

struct _VipService;

struct _VipWriteFileRequest {
	WorRequest parent;
};

struct _VipWriteFileRequestClass {
	WorRequestClass parent_class;

	void (*write)(VipWriteFileRequest *request, CatIOutputStream *output_stream);
};


GType vip_write_file_request_get_type();

VipWriteFileRequest *vip_write_file_request_new_simple(struct _VipService *vip_service, CatTreeNode *tree_node, CatStringWo *e_data);

void vip_write_file_request_construct(VipWriteFileRequest *request, struct _VipService *vip_service, CatTreeNode *tree_node);

void vip_write_file_request_wait_to_finish(VipWriteFileRequest *request);

G_END_DECLS

#endif /* VIPWRITEFILEREQUEST_H_ */
