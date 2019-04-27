/*
   File:    jagrefreshcompilerrequest.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 18, 2014
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

#ifndef JAGREFRESHCOMPILERREQUEST_H_
#define JAGREFRESHCOMPILERREQUEST_H_

#include "../jagmooseservice.h"
#include <caterpillar.h>
#include <worm.h>
#include <moose.h>

G_BEGIN_DECLS

#define JAG_TYPE_REFRESH_COMPILER_REQUEST              (jag_refresh_compiler_request_get_type())
#define JAG_REFRESH_COMPILER_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_refresh_compiler_request_get_type(), JagRefreshCompilerRequest))
#define JAG_REFRESH_COMPILER_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_REFRESH_COMPILER_REQUEST, JagRefreshCompilerRequestClass))
#define JAG_IS_REFRESH_COMPILER_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_REFRESH_COMPILER_REQUEST))
#define JAG_IS_REFRESH_COMPILER_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_REFRESH_COMPILER_REQUEST))
#define JAG_REFRESH_COMPILER_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_REFRESH_COMPILER_REQUEST, JagRefreshCompilerRequestClass))

typedef struct _JagRefreshCompilerRequest               JagRefreshCompilerRequest;
typedef struct _JagRefreshCompilerRequestPrivate        JagRefreshCompilerRequestPrivate;
typedef struct _JagRefreshCompilerRequestClass          JagRefreshCompilerRequestClass;


struct _JagRefreshCompilerRequest {
	WorRequest parent;
};

struct _JagRefreshCompilerRequestClass {
	WorRequestClass parent_class;
};


GType jag_refresh_compiler_request_get_type();

JagRefreshCompilerRequest *jag_refresh_compiler_request_new(JagMooseService *jag_moose_service, MooIdPath *module_id_path);

G_END_DECLS

#endif /* JAGREFRESHCOMPILERREQUEST_H_ */
