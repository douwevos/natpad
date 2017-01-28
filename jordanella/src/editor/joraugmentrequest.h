/*
   File:    joraugmentrequest.h
   Project: jordanella
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

#ifndef EDITOR_JORAUGMENTREQUEST_H_
#define EDITOR_JORAUGMENTREQUEST_H_

#include <caterpillar.h>
#include <dragonfly.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define JOR_TYPE_AUGMENT_REQUEST              (jor_augment_request_get_type())
#define JOR_AUGMENT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jor_augment_request_get_type(), JorAugmentRequest))
#define JOR_AUGMENT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JOR_TYPE_AUGMENT_REQUEST, JorAugmentRequestClass))
#define JOR_IS_AUGMENT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JOR_TYPE_AUGMENT_REQUEST))
#define JOR_IS_AUGMENT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JOR_TYPE_AUGMENT_REQUEST))
#define JOR_AUGMENT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JOR_TYPE_AUGMENT_REQUEST, JorAugmentRequestClass))


typedef struct _JorAugmentRequest               JorAugmentRequest;
typedef struct _JorAugmentRequestPrivate        JorAugmentRequestPrivate;
typedef struct _JorAugmentRequestClass          JorAugmentRequestClass;


struct _JorAugmentRequest {
	DraAugmentRequest parent;
};

struct _JorAugmentRequestClass {
	DraAugmentRequestClass parent_class;
};


GType jor_augment_request_get_type();

JorAugmentRequest *jor_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key);

G_END_DECLS

#endif /* EDITOR_JORAUGMENTREQUEST_H_ */
