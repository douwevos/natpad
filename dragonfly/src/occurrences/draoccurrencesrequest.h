/*
   File:    draoccurrencesrequest.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Nov 18, 2015
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

#ifndef OCCURRENCES_DRAOCCURRENCESREQUEST_H_
#define OCCURRENCES_DRAOCCURRENCESREQUEST_H_

#include "../draeditor.h"
#include <worm.h>
#include <chameleon.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_OCCURRENCES_REQUEST              (dra_occurrences_request_get_type())
#define DRA_OCCURRENCES_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_occurrences_request_get_type(), DraOccurrencesRequest))
#define DRA_OCCURRENCES_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_OCCURRENCES_REQUEST, DraOccurrencesRequestClass))
#define DRA_IS_OCCURRENCES_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_OCCURRENCES_REQUEST))
#define DRA_IS_OCCURRENCES_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_OCCURRENCES_REQUEST))
#define DRA_OCCURRENCES_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_OCCURRENCES_REQUEST, DraOccurrencesRequestClass))


typedef struct _DraOccurrencesRequest               DraOccurrencesRequest;
typedef struct _DraOccurrencesRequestPrivate        DraOccurrencesRequestPrivate;
typedef struct _DraOccurrencesRequestClass          DraOccurrencesRequestClass;


struct _DraOccurrencesRequest {
	WorRequest parent;
};

struct _DraOccurrencesRequestClass {
	WorRequestClass parent_class;
};


GType dra_occurrences_request_get_type();

DraOccurrencesRequest *dra_occurrences_request_new(DraEditor *editor, ChaRevisionWo *a_revision);

G_END_DECLS

#endif /* OCCURRENCES_DRAOCCURRENCESREQUEST_H_ */
