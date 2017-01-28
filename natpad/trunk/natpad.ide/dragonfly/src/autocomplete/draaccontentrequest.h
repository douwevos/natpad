/*
   File:    draaccontentrequest.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 19, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef AUTOCOMPLETE_DRAACCONTENTREQUEST_H_
#define AUTOCOMPLETE_DRAACCONTENTREQUEST_H_

#include "draiaccontentprovider.h"
#include <worm.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_AC_CONTENT_REQUEST              (dra_ac_content_request_get_type())
#define DRA_AC_CONTENT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_ac_content_request_get_type(), DraAcContentRequest))
#define DRA_AC_CONTENT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_AC_CONTENT_REQUEST, DraAcContentRequestClass))
#define DRA_IS_AC_CONTENT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_AC_CONTENT_REQUEST))
#define DRA_IS_AC_CONTENT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_AC_CONTENT_REQUEST))
#define DRA_AC_CONTENT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_AC_CONTENT_REQUEST, DraAcContentRequestClass))


typedef struct _DraAcContentRequest               DraAcContentRequest;
typedef struct _DraAcContentRequestPrivate        DraAcContentRequestPrivate;
typedef struct _DraAcContentRequestClass          DraAcContentRequestClass;


struct _DraAcContentRequest {
	WorRequest parent;
};

struct _DraAcContentRequestClass {
	WorRequestClass parent_class;
};


GType dra_ac_content_request_get_type();

DraAcContentRequest *dra_ac_content_request_new();

G_END_DECLS

#endif /* AUTOCOMPLETE_DRAACCONTENTREQUEST_H_ */
