/*
   File:    jagrefreshlibrariesrequest.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jul 20, 2014
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

#ifndef JAGREFRESHLIBRARIESREQUEST_H_
#define JAGREFRESHLIBRARIESREQUEST_H_

#include <caterpillar.h>
#include <worm.h>
#include <moose.h>

G_BEGIN_DECLS

#define JAG_TYPE_REFRESH_LIBRARIES_REQUEST              (jag_refresh_libraries_request_get_type())
#define JAG_REFRESH_LIBRARIES_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_refresh_libraries_request_get_type(), JagRefreshLibrariesRequest))
#define JAG_REFRESH_LIBRARIES_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_REFRESH_LIBRARIES_REQUEST, JagRefreshLibrariesRequestClass))
#define JAG_IS_REFRESH_LIBRARIES_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_REFRESH_LIBRARIES_REQUEST))
#define JAG_IS_REFRESH_LIBRARIES_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_REFRESH_LIBRARIES_REQUEST))
#define JAG_REFRESH_LIBRARIES_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_REFRESH_LIBRARIES_REQUEST, JagRefreshLibrariesRequestClass))


typedef struct _JagRefreshLibrariesRequest               JagRefreshLibrariesRequest;
typedef struct _JagRefreshLibrariesRequestPrivate        JagRefreshLibrariesRequestPrivate;
typedef struct _JagRefreshLibrariesRequestClass          JagRefreshLibrariesRequestClass;


struct _JagRefreshLibrariesRequest {
	WorRequest parent;
};

struct _JagRefreshLibrariesRequestClass {
	WorRequestClass parent_class;
};


GType jag_refresh_libraries_request_get_type();

JagRefreshLibrariesRequest *jag_refresh_libraries_request_new(MooService *moo_service, long long module_node_id, int last_known_node_idx);

G_END_DECLS

#endif /* JAGREFRESHLIBRARIESREQUEST_H_ */
