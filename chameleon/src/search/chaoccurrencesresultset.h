/*
   File:    chaoccurrencesresultset.h
   Project: chameleon
   Author:  Douwe Vos
   Date:    Nov 5, 2015
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

#ifndef SEARCH_CHAOCCURRENCESRESULTSET_H_
#define SEARCH_CHAOCCURRENCESRESULTSET_H_

#include "chasearchpageoccurrences.h"
#include "chasearchoccurrence.h"
#include "chasearchquerywo.h"
#include "../document/chacursor.h"
#include "../document/chapagewo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_OCCURRENCES_RESULT_SET              (cha_occurrences_result_set_get_type())
#define CHA_OCCURRENCES_RESULT_SET(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_occurrences_result_set_get_type(), ChaOccurrencesResultSet))
#define CHA_OCCURRENCES_RESULT_SET_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_OCCURRENCES_RESULT_SET, ChaOccurrencesResultSetClass))
#define CHA_IS_OCCURRENCES_RESULT_SET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_OCCURRENCES_RESULT_SET))
#define CHA_IS_OCCURRENCES_RESULT_SET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_OCCURRENCES_RESULT_SET))
#define CHA_OCCURRENCES_RESULT_SET_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_OCCURRENCES_RESULT_SET, ChaOccurrencesResultSetClass))


typedef struct _ChaOccurrencesResultSet               ChaOccurrencesResultSet;
typedef struct _ChaOccurrencesResultSetPrivate        ChaOccurrencesResultSetPrivate;
typedef struct _ChaOccurrencesResultSetClass          ChaOccurrencesResultSetClass;


struct _ChaOccurrencesResultSet {
	GObject parent;
};

struct _ChaOccurrencesResultSetClass {
	GObjectClass parent_class;
};


GType cha_occurrences_result_set_get_type();

ChaOccurrencesResultSet *cha_occurrences_result_set_new(ChaSearchQueryWo *query, ChaDocument *document);

void cha_occurences_result_set_set_revision(ChaOccurrencesResultSet *result_set, ChaRevisionWo *revision);

ChaSearchOccurrence *cha_occurrences_result_set_find_backward(ChaOccurrencesResultSet *result_set, ChaCursorWo *cursor, int *opg_idx);

ChaSearchOccurrence *cha_occurrences_result_set_find_forward(ChaOccurrencesResultSet *result_set, ChaCursorWo *cursor, int *opg_idx);

ChaSearchPageOccurrences *cha_search_query_wo_run_for_page(ChaOccurrencesResultSet *result_set, ChaPageWo *page, int page_index);

ChaSearchPageOccurrences *cha_occurences_result_set_get_page_occurrences(ChaOccurrencesResultSet *result_set, int page_index);

CatArrayWo *cha_occurrences_result_set_enlist_for_line(ChaOccurrencesResultSet *result_set, int page_index, int page_line_index);

G_END_DECLS

#endif /* SEARCH_CHAOCCURRENCESRESULTSET_H_ */
