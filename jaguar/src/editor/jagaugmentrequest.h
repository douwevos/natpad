/*
   File:    jagaugmentrequest.h
   Project: jaguar
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

#ifndef SRC_EDITOR_JAGAUGMENTREQUEST_H_
#define SRC_EDITOR_JAGAUGMENTREQUEST_H_

#include <caterpillar.h>
#include <worm.h>
#include <chameleon.h>
#include <dragonfly.h>

G_BEGIN_DECLS

#define JAG_TYPE_AUGMENT_REQUEST              (jag_augment_request_get_type())
#define JAG_AUGMENT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_augment_request_get_type(), JagAugmentRequest))
#define JAG_AUGMENT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_AUGMENT_REQUEST, JagAugmentRequestClass))
#define JAG_IS_AUGMENT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_AUGMENT_REQUEST))
#define JAG_IS_AUGMENT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_AUGMENT_REQUEST))
#define JAG_AUGMENT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_AUGMENT_REQUEST, JagAugmentRequestClass))


typedef struct _JagAugmentRequest               JagAugmentRequest;
typedef struct _JagAugmentRequestPrivate        JagAugmentRequestPrivate;
typedef struct _JagAugmentRequestClass          JagAugmentRequestClass;


struct _JagAugmentRequest {
	DraAugmentRequest parent;
};

struct _JagAugmentRequestClass {
	DraAugmentRequestClass parent_class;
};


GType jag_augment_request_get_type();

JagAugmentRequest *jag_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key);

G_END_DECLS

#endif /* SRC_EDITOR_JAGAUGMENTREQUEST_H_ */
