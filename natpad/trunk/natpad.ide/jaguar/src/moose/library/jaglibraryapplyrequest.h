/*
   File:    jaglibraryapplyrequest.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jul 22, 2014
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

#ifndef JAGLIBRARYAPPLYREQUEST_H_
#define JAGLIBRARYAPPLYREQUEST_H_

#include <worm.h>
#include <caterpillar.h>
#include <moose.h>
#include "../jagmooseservice.h"

G_BEGIN_DECLS

#define JAG_TYPE_LIBRARY_APPLY_REQUEST              (jag_library_apply_request_get_type())
#define JAG_LIBRARY_APPLY_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_library_apply_request_get_type(), JagLibraryApplyRequest))
#define JAG_LIBRARY_APPLY_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_LIBRARY_APPLY_REQUEST, JagLibraryApplyRequestClass))
#define JAG_IS_LIBRARY_APPLY_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_LIBRARY_APPLY_REQUEST))
#define JAG_IS_LIBRARY_APPLY_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_LIBRARY_APPLY_REQUEST))
#define JAG_LIBRARY_APPLY_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_LIBRARY_APPLY_REQUEST, JagLibraryApplyRequestClass))


typedef struct _JagLibraryApplyRequest               JagLibraryApplyRequest;
typedef struct _JagLibraryApplyRequestPrivate        JagLibraryApplyRequestPrivate;
typedef struct _JagLibraryApplyRequestClass          JagLibraryApplyRequestClass;


struct _JagLibraryApplyRequest {
	WorRequest parent;
};

struct _JagLibraryApplyRequestClass {
	WorRequestClass parent_class;
};


GType jag_library_apply_request_get_type();

JagLibraryApplyRequest *jag_library_apply_request_new(JagMooseService *moose_service, MooIdPath *id_path, MooNodeWo *node_wo_jar);

G_END_DECLS

#endif /* JAGLIBRARYAPPLYREQUEST_H_ */
