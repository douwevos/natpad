/*
   File:    mulpytaugmentrequest.h
   Project: mule
   Author:  Douwe Vos
   Date:    Dec 28, 2016
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

#ifndef PYTHON_MULPYTAUGMENTREQUEST_H_
#define PYTHON_MULPYTAUGMENTREQUEST_H_

#include "mulpytscannerfactory.h"
#include <caterpillar.h>
#include <dragonfly.h>
#include <chameleon.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define MUL_TYPE_PYT_AUGMENT_REQUEST              (mul_pyt_augment_request_get_type())
#define MUL_PYT_AUGMENT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_pyt_augment_request_get_type(), MulPytAugmentRequest))
#define MUL_PYT_AUGMENT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_PYT_AUGMENT_REQUEST, MulPytAugmentRequestClass))
#define MUL_IS_PYT_AUGMENT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_PYT_AUGMENT_REQUEST))
#define MUL_IS_PYT_AUGMENT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_PYT_AUGMENT_REQUEST))
#define MUL_PYT_AUGMENT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_PYT_AUGMENT_REQUEST, MulPytAugmentRequestClass))


typedef struct _MulPytAugmentRequest               MulPytAugmentRequest;
typedef struct _MulPytAugmentRequestPrivate        MulPytAugmentRequestPrivate;
typedef struct _MulPytAugmentRequestClass          MulPytAugmentRequestClass;


struct _MulPytAugmentRequest {
	DraAugmentRequest parent;
};

struct _MulPytAugmentRequestClass {
	DraAugmentRequestClass parent_class;
};


GType mul_pyt_augment_request_get_type();

MulPytAugmentRequest *mul_pyt_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key,
		GroRunModel *model, GroRunITokenFactory *token_factory, MulPytScannerFactory *scanner_factory);

G_END_DECLS

#endif /* PYTHON_MULPYTAUGMENTREQUEST_H_ */
