/*
   File:    xanaugmentrequest.h
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 10, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef EDITOR_XANAUGMENTREQUEST_H_
#define EDITOR_XANAUGMENTREQUEST_H_

#include "../parser/xanscannerfactory.h"
#include <caterpillar.h>
#include <dragonfly.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define XAN_TYPE_AUGMENT_REQUEST              (xan_augment_request_get_type())
#define XAN_AUGMENT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), xan_augment_request_get_type(), XanAugmentRequest))
#define XAN_AUGMENT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), XAN_TYPE_AUGMENT_REQUEST, XanAugmentRequestClass))
#define XAN_IS_AUGMENT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAN_TYPE_AUGMENT_REQUEST))
#define XAN_IS_AUGMENT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), XAN_TYPE_AUGMENT_REQUEST))
#define XAN_AUGMENT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), XAN_TYPE_AUGMENT_REQUEST, XanAugmentRequestClass))


typedef struct _XanAugmentRequest               XanAugmentRequest;
typedef struct _XanAugmentRequestPrivate        XanAugmentRequestPrivate;
typedef struct _XanAugmentRequestClass          XanAugmentRequestClass;


struct _XanAugmentRequest {
	DraAugmentRequest parent;
};

struct _XanAugmentRequestClass {
	DraAugmentRequestClass parent_class;
};


GType xan_augment_request_get_type();

XanAugmentRequest *xan_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key, XanScannerFactory *scanner_factory);

G_END_DECLS

#endif /* EDITOR_XANAUGMENTREQUEST_H_ */
