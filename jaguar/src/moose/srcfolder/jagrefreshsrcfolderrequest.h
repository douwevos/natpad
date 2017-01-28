/*
   File:    jagrefreshsrcfolderrequest.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    May 26, 2014
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

#ifndef JAGREFRESHSRCFOLDERREQUEST_H_
#define JAGREFRESHSRCFOLDERREQUEST_H_

#include <worm.h>
#include <caterpillar.h>
#include <moose.h>

G_BEGIN_DECLS

#define JAG_TYPE_REFRESH_SRC_FOLDER_REQUEST              (jag_refresh_src_folder_request_get_type())
#define JAG_REFRESH_SRC_FOLDER_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_refresh_src_folder_request_get_type(), JagRefreshSrcFolderRequest))
#define JAG_REFRESH_SRC_FOLDER_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_REFRESH_SRC_FOLDER_REQUEST, JagRefreshSrcFolderRequestClass))
#define JAG_IS_REFRESH_SRC_FOLDER_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_REFRESH_SRC_FOLDER_REQUEST))
#define JAG_IS_REFRESH_SRC_FOLDER_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_REFRESH_SRC_FOLDER_REQUEST))
#define JAG_REFRESH_SRC_FOLDER_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_REFRESH_SRC_FOLDER_REQUEST, JagRefreshSrcFolderRequestClass))
#define JAG_REFRESH_SRC_FOLDER_REQUEST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_REFRESH_SRC_FOLDER_REQUEST, JagRefreshSrcFolderRequestPrivate))


typedef struct _JagRefreshSrcFolderRequest               JagRefreshSrcFolderRequest;
typedef struct _JagRefreshSrcFolderRequestPrivate        JagRefreshSrcFolderRequestPrivate;
typedef struct _JagRefreshSrcFolderRequestClass          JagRefreshSrcFolderRequestClass;


struct _JagRefreshSrcFolderRequest {
	WorRequest parent;
	JagRefreshSrcFolderRequestPrivate *priv;
};

struct _JagRefreshSrcFolderRequestClass {
	WorRequestClass parent_class;
};


GType jag_refresh_src_folder_request_get_type();

JagRefreshSrcFolderRequest *jag_refresh_src_folder_request_new(MooService *moo_service, long long module_node_id, int last_known_node_idx);

G_END_DECLS

#endif /* JAGREFRESHSRCFOLDERREQUEST_H_ */
