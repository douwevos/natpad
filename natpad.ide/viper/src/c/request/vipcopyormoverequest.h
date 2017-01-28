/*
   File:    vipcopyormoverequest.h
   Project: viper
   Author:  Douwe Vos
   Date:    Feb 26, 2014
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

#ifndef VIPCOPYORMOVEREQUEST_H_
#define VIPCOPYORMOVEREQUEST_H_

#include "../vippath.h"
#include <caterpillar.h>
#include <worm.h>


G_BEGIN_DECLS

#define VIP_TYPE_COPY_OR_MOVE_REQUEST              (vip_copy_or_move_request_get_type())
#define VIP_COPY_OR_MOVE_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), vip_copy_or_move_request_get_type(), VipCopyOrMoveRequest))
#define VIP_COPY_OR_MOVE_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), VIP_TYPE_COPY_OR_MOVE_REQUEST, VipCopyOrMoveRequestClass))
#define VIP_IS_COPY_OR_MOVE_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), VIP_TYPE_COPY_OR_MOVE_REQUEST))
#define VIP_IS_COPY_OR_MOVE_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), VIP_TYPE_COPY_OR_MOVE_REQUEST))
#define VIP_COPY_OR_MOVE_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), VIP_TYPE_COPY_OR_MOVE_REQUEST, VipCopyOrMoveRequestClass))
#define VIP_COPY_OR_MOVE_REQUEST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), VIP_TYPE_COPY_OR_MOVE_REQUEST, VipCopyOrMoveRequestPrivate))


typedef struct _VipCopyOrMoveRequest               VipCopyOrMoveRequest;
typedef struct _VipCopyOrMoveRequestPrivate        VipCopyOrMoveRequestPrivate;
typedef struct _VipCopyOrMoveRequestClass          VipCopyOrMoveRequestClass;


struct _VipCopyOrMoveRequest {
	WorRequest parent;
	VipCopyOrMoveRequestPrivate *priv;
};

struct _VipCopyOrMoveRequestClass {
	WorRequestClass parent_class;
};


struct _VipService;

GType vip_copy_or_move_request_get_type();

VipCopyOrMoveRequest *vip_copy_or_move_request_new(struct _VipService *vip_service, CatArrayWo *source_path_list, CatReadableTreeNode *destination, gboolean is_copy);

G_END_DECLS



#endif /* VIPCOPYORMOVEREQUEST_H_ */
