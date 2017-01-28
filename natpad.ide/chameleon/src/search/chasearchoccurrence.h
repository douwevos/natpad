/*
   File:    chasearchoccurrence.h
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

#ifndef SEARCH_CHASEARCHOCCURRENCE_H_
#define SEARCH_CHASEARCHOCCURRENCE_H_

#include "../document/chacursorwo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define CHA_TYPE_SEARCH_OCCURRENCE              (cha_search_occurrence_get_type())
#define CHA_SEARCH_OCCURRENCE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cha_search_occurrence_get_type(), ChaSearchOccurrence))
#define CHA_SEARCH_OCCURRENCE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHA_TYPE_SEARCH_OCCURRENCE, ChaSearchOccurrenceClass))
#define CHA_IS_SEARCH_OCCURRENCE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHA_TYPE_SEARCH_OCCURRENCE))
#define CHA_IS_SEARCH_OCCURRENCE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHA_TYPE_SEARCH_OCCURRENCE))
#define CHA_SEARCH_OCCURRENCE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHA_TYPE_SEARCH_OCCURRENCE, ChaSearchOccurrenceClass))


typedef struct _ChaSearchOccurrence               ChaSearchOccurrence;
typedef struct _ChaSearchOccurrencePrivate        ChaSearchOccurrencePrivate;
typedef struct _ChaSearchOccurrenceClass          ChaSearchOccurrenceClass;


struct _ChaSearchOccurrence {
	GObject parent;
};

struct _ChaSearchOccurrenceClass {
	GObjectClass parent_class;
};


GType cha_search_occurrence_get_type();

ChaSearchOccurrence *cha_search_occurrence_new(int page_line_index, int column_start, int column_end, int row_count);

gboolean cha_search_occurrence_is_after(const ChaSearchOccurrence *occurrence, int row, int column);
gboolean cha_search_occurrence_is_before(const ChaSearchOccurrence *occurrence, int row, int column);

void cha_search_occurence_create_cursors(const ChaSearchOccurrence *occurrence, int page_index, ChaCursorWo **o_start_cur, ChaCursorWo **o_end_cur);

void cha_search_occurence_get_first_and_last_line_index(const ChaSearchOccurrence *occurrence, int *first_pl_index, int *last_pl_index);
void cha_search_occurence_get_column_start_and_end(const ChaSearchOccurrence *occurrence, int *column_start, int *column_end);
int cha_search_occurence_get_page_line_index(const ChaSearchOccurrence *occurrence);


G_END_DECLS

#endif /* SEARCH_CHASEARCHOCCURRENCE_H_ */
