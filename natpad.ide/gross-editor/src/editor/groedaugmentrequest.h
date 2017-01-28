/*
   File:    groedaugmentrequest.h
   Project: gross-editor
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

#ifndef EDITOR_GROEDAUGMENTREQUEST_H_
#define EDITOR_GROEDAUGMENTREQUEST_H_

#include <caterpillar.h>
#include <dragonfly.h>
#include <grossruntime.h>
#include <grossparser.h>


G_BEGIN_DECLS

#define GROED_TYPE_AUGMENT_REQUEST              (groed_augment_request_get_type())
#define GROED_AUGMENT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), groed_augment_request_get_type(), GroEdAugmentRequest))
#define GROED_AUGMENT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GROED_TYPE_AUGMENT_REQUEST, GroEdAugmentRequestClass))
#define GROED_IS_AUGMENT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GROED_TYPE_AUGMENT_REQUEST))
#define GROED_IS_AUGMENT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GROED_TYPE_AUGMENT_REQUEST))
#define GROED_AUGMENT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GROED_TYPE_AUGMENT_REQUEST, GroEdAugmentRequestClass))


typedef struct _GroEdAugmentRequest               GroEdAugmentRequest;
typedef struct _GroEdAugmentRequestPrivate        GroEdAugmentRequestPrivate;
typedef struct _GroEdAugmentRequestClass          GroEdAugmentRequestClass;


struct _GroEdAugmentRequest {
	DraAugmentRequest parent;
};

struct _GroEdAugmentRequestClass {
	DraAugmentRequestClass parent_class;
};


GType groed_augment_request_get_type();

GroEdAugmentRequest *groed_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key,
		GroRunModel *model, GroRunITokenFactory *token_factory, GroScannerFactory *scanner_factory);

G_END_DECLS

#endif /* EDITOR_GROEDAUGMENTREQUEST_H_ */
