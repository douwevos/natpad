/*
   File:    chapagelayoutrequest.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 23, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef CHAPAGELAYOUTREQUEST_H_
#define CHAPAGELAYOUTREQUEST_H_

#include "chadocumentview.h"
#include "chapagelayout.h"
#include "../document/chapagewo.h"
#include <caterpillar.h>
#include <worm.h>

G_BEGIN_DECLS

#define CHA_TYPE_PAGE_LAYOUT_REQUEST              (cha_page_layout_request_get_type())
#define CHA_PAGE_LAYOUT_REQUEST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_page_layout_request_get_type(), ChaPageLayoutRequest))
#define CHA_PAGE_LAYOUT_REQUEST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_PAGE_LAYOUT_REQUEST, ChaPageLayoutRequestClass))
#define CHA_IS_PAGE_LAYOUT_REQUEST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_PAGE_LAYOUT_REQUEST))
#define CHA_IS_PAGE_LAYOUT_REQUEST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_PAGE_LAYOUT_REQUEST))
#define CHA_PAGE_LAYOUT_REQUEST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_PAGE_LAYOUT_REQUEST, ChaPageLayoutRequestClass))


typedef struct _ChaPageLayoutRequest               ChaPageLayoutRequest;
typedef struct _ChaPageLayoutRequestPrivate        ChaPageLayoutRequestPrivate;
typedef struct _ChaPageLayoutRequestClass          ChaPageLayoutRequestClass;


struct _ChaPageLayoutRequest {
	WorRequest parent;
};

struct _ChaPageLayoutRequestClass {
	WorRequestClass parent_class;
	CatAtomicInteger *enlisted_counter;
	int64_t last_update;
};


GType cha_page_layout_request_get_type();

ChaPageLayoutRequest *cha_page_layout_request_new(ChaDocumentView *document_view, ChaPageWo *a_page_wo, ChaPageLayout *page_layout, int text_layout_width, short font_version, int page_index);

G_END_DECLS

#endif /* CHAPAGELAYOUTREQUEST_H_ */
