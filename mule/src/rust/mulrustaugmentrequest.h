/*
   File:    mulrustaugmentrequest.h
   Project: mule
   Author:  Douwe Vos
   Date:    Jan 9, 2017
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

#ifndef RUST_MULRUSTAUGMENTREQUEST_H_
#define RUST_MULRUSTAUGMENTREQUEST_H_

#include "mulrustscannerfactory.h"
#include <caterpillar.h>
#include <dragonfly.h>
#include <chameleon.h>
#include <grossruntime.h>

G_BEGIN_DECLS

#define MUL_TYPE_RUST_AUGMENT_REQUEST              (mul_rust_augment_request_get_type())
#define MUL_RUST_AUGMENT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_rust_augment_request_get_type(), MulRustAugmentRequest))
#define MUL_RUST_AUGMENT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_RUST_AUGMENT_REQUEST, MulRustAugmentRequestClass))
#define MUL_IS_RUST_AUGMENT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_RUST_AUGMENT_REQUEST))
#define MUL_IS_RUST_AUGMENT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_RUST_AUGMENT_REQUEST))
#define MUL_RUST_AUGMENT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_RUST_AUGMENT_REQUEST, MulRustAugmentRequestClass))


typedef struct _MulRustAugmentRequest               MulRustAugmentRequest;
typedef struct _MulRustAugmentRequestPrivate        MulRustAugmentRequestPrivate;
typedef struct _MulRustAugmentRequestClass          MulRustAugmentRequestClass;


struct _MulRustAugmentRequest {
	DraAugmentRequest parent;
};

struct _MulRustAugmentRequestClass {
	DraAugmentRequestClass parent_class;
};


GType mul_rust_augment_request_get_type();

MulRustAugmentRequest *mul_rust_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key,
		GroRunModel *model, GroRunITokenFactory *token_factory, MulRustScannerFactory *scanner_factory);

G_END_DECLS

#endif /* RUST_MULRUSTAUGMENTREQUEST_H_ */
