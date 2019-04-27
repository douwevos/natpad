/*
   File:    jagindexrefreshrequest.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    May 7, 2014
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

#ifndef JAGINDEXREFRESHREQUEST_H_
#define JAGINDEXREFRESHREQUEST_H_

#include <worm.h>
#include <caterpillar.h>
#include <moose.h>

G_BEGIN_DECLS

#define JAG_TYPE_INDEX_REFRESH_REQUEST              (jag_index_refresh_request_get_type())
#define JAG_INDEX_REFRESH_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_index_refresh_request_get_type(), JagIndexRefreshRequest))
#define JAG_INDEX_REFRESH_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_INDEX_REFRESH_REQUEST, JagIndexRefreshRequestClass))
#define JAG_IS_INDEX_REFRESH_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_INDEX_REFRESH_REQUEST))
#define JAG_IS_INDEX_REFRESH_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_INDEX_REFRESH_REQUEST))
#define JAG_INDEX_REFRESH_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_INDEX_REFRESH_REQUEST, JagIndexRefreshRequestClass))

typedef struct _JagIndexRefreshRequest               JagIndexRefreshRequest;
typedef struct _JagIndexRefreshRequestPrivate        JagIndexRefreshRequestPrivate;
typedef struct _JagIndexRefreshRequestClass          JagIndexRefreshRequestClass;

struct _JagIndexRefreshRequest {
	WorRequest parent;
};

struct _JagIndexRefreshRequestClass {
	WorRequestClass parent_class;
};

GType jag_index_refresh_request_get_type();

JagIndexRefreshRequest *jag_index_refresh_request_new(MooService *moo_service, WorQueue *post_queue);

G_END_DECLS

#endif /* JAGINDEXREFRESHREQUEST_H_ */
