/*
   File:    cheaugmentrequest.h
   Project: cheetah
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

#ifndef EDITOR_CHEAUGMENTREQUEST_H_
#define EDITOR_CHEAUGMENTREQUEST_H_

#include <caterpillar.h>
#include <worm.h>
#include <chameleon.h>
#include <dragonfly.h>

G_BEGIN_DECLS

#define CHE_TYPE_AUGMENT_REQUEST              (che_augment_request_get_type())
#define CHE_AUGMENT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), che_augment_request_get_type(), CheAugmentRequest))
#define CHE_AUGMENT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHE_TYPE_AUGMENT_REQUEST, CheAugmentRequestClass))
#define CHE_IS_AUGMENT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHE_TYPE_AUGMENT_REQUEST))
#define CHE_IS_AUGMENT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHE_TYPE_AUGMENT_REQUEST))
#define CHE_AUGMENT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHE_TYPE_AUGMENT_REQUEST, CheAugmentRequestClass))


typedef struct _CheAugmentRequest               CheAugmentRequest;
typedef struct _CheAugmentRequestPrivate        CheAugmentRequestPrivate;
typedef struct _CheAugmentRequestClass          CheAugmentRequestClass;


struct _CheAugmentRequest {
	DraAugmentRequest parent;
};

struct _CheAugmentRequestClass {
	DraAugmentRequestClass parent_class;
};


GType che_augment_request_get_type();

CheAugmentRequest *che_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key, CatLock *lock);

G_END_DECLS

#endif /* EDITOR_CHEAUGMENTREQUEST_H_ */
