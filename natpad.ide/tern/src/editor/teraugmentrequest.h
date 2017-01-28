/*
   File:    teraugmentrequest.h
   Project: tern
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

#ifndef EDITOR_TERAUGMENTREQUEST_H_
#define EDITOR_TERAUGMENTREQUEST_H_

#include "../grammar/tergrammarmap.h"
#include <caterpillar.h>
#include <chameleon.h>
#include <dragonfly.h>

G_BEGIN_DECLS

#define TER_TYPE_AUGMENT_REQUEST              (ter_augment_request_get_type())
#define TER_AUGMENT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_augment_request_get_type(), TerAugmentRequest))
#define TER_AUGMENT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_AUGMENT_REQUEST, TerAugmentRequestClass))
#define TER_IS_AUGMENT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_AUGMENT_REQUEST))
#define TER_IS_AUGMENT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_AUGMENT_REQUEST))
#define TER_AUGMENT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_AUGMENT_REQUEST, TerAugmentRequestClass))


typedef struct _TerAugmentRequest               TerAugmentRequest;
typedef struct _TerAugmentRequestPrivate        TerAugmentRequestPrivate;
typedef struct _TerAugmentRequestClass          TerAugmentRequestClass;


struct _TerAugmentRequest {
	DraAugmentRequest parent;
};

struct _TerAugmentRequestClass {
	DraAugmentRequestClass parent_class;
};


GType ter_augment_request_get_type();

TerAugmentRequest *ter_augment_request_new(ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *a_slot_key, TerGrammarMap *grammar_map);

G_END_DECLS

#endif /* EDITOR_TERAUGMENTREQUEST_H_ */
