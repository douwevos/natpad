/*
   File:    armaugmentrequest.h
   Project: armadillo
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

#ifndef SRC_EDITOR_ARMAUGMENTREQUEST_H_
#define SRC_EDITOR_ARMAUGMENTREQUEST_H_

#include <caterpillar.h>
#include <worm.h>
#include <chameleon.h>
#include <dragonfly.h>
#include "../parser/armpparser.h"
#include "../parser/armpscannerfactory.h"

G_BEGIN_DECLS

#define ARM_TYPE_AUGMENT_REQUEST              (arm_augment_request_get_type())
#define ARM_AUGMENT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), arm_augment_request_get_type(), ArmAugmentRequest))
#define ARM_AUGMENT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ARM_TYPE_AUGMENT_REQUEST, ArmAugmentRequestClass))
#define ARM_IS_AUGMENT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ARM_TYPE_AUGMENT_REQUEST))
#define ARM_IS_AUGMENT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ARM_TYPE_AUGMENT_REQUEST))
#define ARM_AUGMENT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ARM_TYPE_AUGMENT_REQUEST, ArmAugmentRequestClass))


typedef struct _ArmAugmentRequest               ArmAugmentRequest;
typedef struct _ArmAugmentRequestPrivate        ArmAugmentRequestPrivate;
typedef struct _ArmAugmentRequestClass          ArmAugmentRequestClass;


struct _ArmAugmentRequest {
	DraAugmentRequest parent;
};

struct _ArmAugmentRequestClass {
	DraAugmentRequestClass parent_class;
};


GType arm_augment_request_get_type();

ArmAugmentRequest *arm_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key,
		GroRunModel *model, GroRunITokenFactory *token_factory, ArmPScannerFactory *scanner_factory);

G_END_DECLS

#endif /* SRC_EDITOR_ARMAUGMENTREQUEST_H_ */
