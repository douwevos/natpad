/*
   File:    drabasicaugmentrequest.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Apr 12, 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

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

#ifndef DOCUMENT_DRABASICAUGMENTREQUEST_H_
#define DOCUMENT_DRABASICAUGMENTREQUEST_H_

#include "draaugmentrequest.h"
#include <caterpillar.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define DRA_TYPE_BASIC_AUGMENT_REQUEST              (dra_basic_augment_request_get_type())
#define DRA_BASIC_AUGMENT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_basic_augment_request_get_type(), DraBasicAugmentRequest))
#define DRA_BASIC_AUGMENT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_BASIC_AUGMENT_REQUEST, DraBasicAugmentRequestClass))
#define DRA_IS_BASIC_AUGMENT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_BASIC_AUGMENT_REQUEST))
#define DRA_IS_BASIC_AUGMENT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_BASIC_AUGMENT_REQUEST))
#define DRA_BASIC_AUGMENT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_BASIC_AUGMENT_REQUEST, DraBasicAugmentRequestClass))

struct _DraBasicAugmentRequest {
	DraAugmentRequest parent;
};

struct _DraBasicAugmentRequestClass {
	DraAugmentRequestClass parent_class;
};

typedef struct _DraBasicAugmentRequest               DraBasicAugmentRequest;
typedef struct _DraBasicAugmentRequestPrivate        DraBasicAugmentRequestPrivate;
typedef struct _DraBasicAugmentRequestClass          DraBasicAugmentRequestClass;


GType dra_basic_augment_request_get_type();

DraBasicAugmentRequest *dra_basic_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *a_slot_key);

G_END_DECLS

#endif /* DOCUMENT_DRABASICAUGMENTREQUEST_H_ */
