/*
   File:    chasearchpageoccurrences.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Sep 17, 2015
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

#ifndef SEARCH_CHASEARCHPAGEOCCURRENCES_H_
#define SEARCH_CHASEARCHPAGEOCCURRENCES_H_

#include "chasearchoccurrence.h"
#include "../document/chapagewo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_SEARCH_PAGE_OCCURRENCES              (cha_search_page_occurrences_get_type())
#define CHA_SEARCH_PAGE_OCCURRENCES(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_search_page_occurrences_get_type(), ChaSearchPageOccurrences))
#define CHA_SEARCH_PAGE_OCCURRENCES_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_SEARCH_PAGE_OCCURRENCES, ChaSearchPageOccurrencesClass))
#define CHA_IS_SEARCH_PAGE_OCCURRENCES(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_SEARCH_PAGE_OCCURRENCES))
#define CHA_IS_SEARCH_PAGE_OCCURRENCES_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_SEARCH_PAGE_OCCURRENCES))
#define CHA_SEARCH_PAGE_OCCURRENCES_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_SEARCH_PAGE_OCCURRENCES, ChaSearchPageOccurrencesClass))


typedef struct _ChaSearchPageOccurrences               ChaSearchPageOccurrences;
typedef struct _ChaSearchPageOccurrencesPrivate        ChaSearchPageOccurrencesPrivate;
typedef struct _ChaSearchPageOccurrencesClass          ChaSearchPageOccurrencesClass;


struct _ChaSearchPageOccurrences {
	GObject parent;
};

struct _ChaSearchPageOccurrencesClass {
	GObjectClass parent_class;
};


GType cha_search_page_occurrences_get_type();

ChaSearchPageOccurrences *cha_search_page_occurrences_new(ChaPageWo *a_page, int page_index);

void cha_search_page_occurrences_add(ChaSearchPageOccurrences *page_occurrences, ChaSearchOccurrence *occurrence);

int cha_search_page_occurrences_count(const ChaSearchPageOccurrences *page_occurrences);
int cha_search_page_occurrences_get_page_index(const ChaSearchPageOccurrences *page_occurrences);
ChaPageWo *cha_search_page_occurrences_get_page(const ChaSearchPageOccurrences *page_occurrences);
CatArrayWo *cha_search_page_occurrences_get_at(const ChaSearchPageOccurrences *page_occurrences, int index);

CatArrayWo *cha_search_page_occurrences_enlist_for_line(const ChaSearchPageOccurrences *page_occurrences, int line_index);

G_END_DECLS

#endif /* SEARCH_CHASEARCHPAGEOCCURRENCES_H_ */
