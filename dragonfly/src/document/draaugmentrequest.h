/*
   File:    draaugmentrequest.h
   Project: dragonfly
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

#ifndef DOCUMENT_DRAAUGMENTREQUEST_H_
#define DOCUMENT_DRAAUGMENTREQUEST_H_

#include "../drakeywordprinter.h"
#include "../spell/draspellhelper.h"
#include <caterpillar.h>
#include <worm.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define DRA_TYPE_AUGMENT_REQUEST              (dra_augment_request_get_type())
#define DRA_AUGMENT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_augment_request_get_type(), DraAugmentRequest))
#define DRA_AUGMENT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_AUGMENT_REQUEST, DraAugmentRequestClass))
#define DRA_IS_AUGMENT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_AUGMENT_REQUEST))
#define DRA_IS_AUGMENT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_AUGMENT_REQUEST))
#define DRA_AUGMENT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_AUGMENT_REQUEST, DraAugmentRequestClass))


typedef struct _DraAugmentRequest               DraAugmentRequest;
typedef struct _DraAugmentRequestPrivate        DraAugmentRequestPrivate;
typedef struct _DraAugmentRequestClass          DraAugmentRequestClass;


struct _DraAugmentRequest {
	WorRequest parent;
	int slot_index;
};

struct _DraAugmentRequestClass {
	WorRequestClass parent_class;
	gboolean (*runAugment)(DraAugmentRequest *request, ChaRevisionWo *a_revision, DraKeywordPrinter *keyword_printer, DraKeywordPrinter *line_tag_printer);
};


GType dra_augment_request_get_type();

void dra_augment_request_construct(DraAugmentRequest *request, ChaDocument *document, ChaRevisionWo *a_revision, CatStringWo *slot_key);

CatStringWo *dra_augment_request_get_slot_key(DraAugmentRequest *request);

void dra_augment_request_set_spell_helper(DraAugmentRequest *request, DraSpellHelper *spell_helper);
DraSpellHelper *dra_augment_request_get_spell_helper(DraAugmentRequest *request);

G_END_DECLS

#endif /* DOCUMENT_DRAAUGMENTREQUEST_H_ */
