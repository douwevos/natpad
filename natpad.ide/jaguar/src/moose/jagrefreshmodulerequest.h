/*
   File:    jagrefreshmodulerequest.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 6, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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


#ifndef JAGREFRESHMODULEREQUEST_H_
#define JAGREFRESHMODULEREQUEST_H_

#include "jagmooseservice.h"
#include <caterpillar.h>
#include <worm.h>
#include <moose.h>

G_BEGIN_DECLS

#define JAG_TYPE_REFRESH_MODULE_REQUEST              (jag_refresh_module_request_get_type())
#define JAG_REFRESH_MODULE_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_refresh_module_request_get_type(), JagRefreshModuleRequest))
#define JAG_REFRESH_MODULE_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_REFRESH_MODULE_REQUEST, JagRefreshModuleRequestClass))
#define JAG_IS_REFRESH_MODULE_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_REFRESH_MODULE_REQUEST))
#define JAG_IS_REFRESH_MODULE_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_REFRESH_MODULE_REQUEST))
#define JAG_REFRESH_MODULE_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_REFRESH_MODULE_REQUEST, JagRefreshModuleRequestClass))
#define JAG_REFRESH_MODULE_REQUEST_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_REFRESH_MODULE_REQUEST, JagRefreshModuleRequestPrivate))


typedef struct _JagRefreshModuleRequest               JagRefreshModuleRequest;
typedef struct _JagRefreshModuleRequestPrivate        JagRefreshModuleRequestPrivate;
typedef struct _JagRefreshModuleRequestClass          JagRefreshModuleRequestClass;


struct _JagRefreshModuleRequest {
	WorRequest parent;
	JagRefreshModuleRequestPrivate *priv;
};

struct _JagRefreshModuleRequestClass {
	WorRequestClass parent_class;
};


GType jag_refresh_module_request_get_type();

JagRefreshModuleRequest *jag_refresh_module_request_new(JagMooseService *jag_mooose_service, long long unique_id, int last_known_node_idx);

G_END_DECLS

#endif /* JAGREFRESHMODULEREQUEST_H_ */
