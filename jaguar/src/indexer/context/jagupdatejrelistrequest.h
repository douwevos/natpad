/*
   File:    jagupdatejrelistrequest.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    May 2, 2014
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

#ifndef JAGUPDATEJRELISTREQUEST_H_
#define JAGUPDATEJRELISTREQUEST_H_

#include "jagindexerjremap.h"
#include <worm.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_UPDATE_JRE_LIST_REQUEST              (jag_update_jre_list_request_get_type())
#define JAG_UPDATE_JRE_LIST_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_update_jre_list_request_get_type(), JagUpdateJreListRequest))
#define JAG_UPDATE_JRE_LIST_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_UPDATE_JRE_LIST_REQUEST, JagUpdateJreListRequestClass))
#define JAG_IS_UPDATE_JRE_LIST_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_UPDATE_JRE_LIST_REQUEST))
#define JAG_IS_UPDATE_JRE_LIST_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_UPDATE_JRE_LIST_REQUEST))
#define JAG_UPDATE_JRE_LIST_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_UPDATE_JRE_LIST_REQUEST, JagUpdateJreListRequestClass))

typedef struct _JagUpdateJreListRequest               JagUpdateJreListRequest;
typedef struct _JagUpdateJreListRequestPrivate        JagUpdateJreListRequestPrivate;
typedef struct _JagUpdateJreListRequestClass          JagUpdateJreListRequestClass;


struct _JagUpdateJreListRequest {
	WorRequest parent;
};

struct _JagUpdateJreListRequestClass {
	WorRequestClass parent_class;
};


GType jag_update_jre_list_request_get_type();

JagUpdateJreListRequest *jag_update_jre_list_request_new(JagIndexerJreMap *jre_map, CatArrayWo *jre_list);

G_END_DECLS

#endif /* JAGUPDATEJRELISTREQUEST_H_ */
