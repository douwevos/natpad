/*
   File:    cusaugmentrequest.h
   Project: cuscus
   Author:  Douwe Vos
   Date:    Dec 27, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef EDITOR_CUSAUGMENTREQUEST_H_
#define EDITOR_CUSAUGMENTREQUEST_H_

#include <caterpillar.h>
#include <chameleon.h>
#include <dragonfly.h>

G_BEGIN_DECLS

#define CUS_TYPE_AUGMENT_REQUEST              (cus_augment_request_get_type())
#define CUS_AUGMENT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cus_augment_request_get_type(), CusAugmentRequest))
#define CUS_AUGMENT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CUS_TYPE_AUGMENT_REQUEST, CusAugmentRequestClass))
#define CUS_IS_AUGMENT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CUS_TYPE_AUGMENT_REQUEST))
#define CUS_IS_AUGMENT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CUS_TYPE_AUGMENT_REQUEST))
#define CUS_AUGMENT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CUS_TYPE_AUGMENT_REQUEST, CusAugmentRequestClass))


typedef struct _CusAugmentRequest               CusAugmentRequest;
typedef struct _CusAugmentRequestPrivate        CusAugmentRequestPrivate;
typedef struct _CusAugmentRequestClass          CusAugmentRequestClass;


struct _CusAugmentRequest {
	DraAugmentRequest parent;
};

struct _CusAugmentRequestClass {
	DraAugmentRequestClass parent_class;
};


GType cus_augment_request_get_type();

CusAugmentRequest *cus_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key);

G_END_DECLS

#endif /* EDITOR_CUSAUGMENTREQUEST_H_ */
