/*
   File:    chasearchoccurrence.c
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

#include "chasearchoccurrence.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaSearchOccurrence"
#include <logging/catlog.h>

struct _ChaSearchOccurrencePrivate {
	int page_line_index;
	int column_start;
	int column_end;
	int row_count;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaSearchOccurrence, cha_search_occurrence, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaSearchOccurrence)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_search_occurrence_class_init(ChaSearchOccurrenceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_search_occurrence_init(ChaSearchOccurrence *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ChaSearchOccurrence *instance = CHA_SEARCH_OCCURRENCE(object);
//	ChaSearchOccurrencePrivate *priv = cha_search_occurrence_get_instance_private(instance);
	G_OBJECT_CLASS(cha_search_occurrence_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_search_occurrence_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaSearchOccurrence *cha_search_occurrence_new(int page_line_index, int column_start, int column_end, int row_count) {
	ChaSearchOccurrence *result = g_object_new(CHA_TYPE_SEARCH_OCCURRENCE, NULL);
	cat_ref_anounce(result);
	ChaSearchOccurrencePrivate *priv = cha_search_occurrence_get_instance_private(result);
	priv->page_line_index = page_line_index;
	priv->column_start = column_start;
	priv->column_end = column_end;
	priv->row_count = row_count;
	return result;
}

gboolean cha_search_occurrence_is_after(const ChaSearchOccurrence *occurrence, int row, int column) {
	const ChaSearchOccurrencePrivate *priv = cha_search_occurrence_get_instance_private((ChaSearchOccurrence *) occurrence);
	if (row>priv->page_line_index) {
		return FALSE;
	} else if (row<priv->page_line_index) {
		return TRUE;
	}

	if (column<=priv->column_start) {
		return TRUE;
	}
	return FALSE;
}


gboolean cha_search_occurrence_is_before(const ChaSearchOccurrence *occurrence, int row, int column) {
	const ChaSearchOccurrencePrivate *priv = cha_search_occurrence_get_instance_private((ChaSearchOccurrence *) occurrence);
	if (row<priv->page_line_index) {
		return FALSE;
	} else if (row>priv->page_line_index) {
		return TRUE;
	}

	if (column>priv->column_start) {
		return TRUE;
	}
	return FALSE;
}


void cha_search_occurence_create_cursors(const ChaSearchOccurrence *occurrence, int page_index, ChaCursorWo **o_start_cur, ChaCursorWo **o_end_cur) {
	const ChaSearchOccurrencePrivate *priv = cha_search_occurrence_get_instance_private((ChaSearchOccurrence *) occurrence);
	ChaLineLocationWo *ll_start = cha_line_location_wo_new_with(page_index, priv->page_line_index);
	ChaCursorWo *cursor = cha_cursor_wo_new_ll_offset(ll_start, priv->column_start);
	*o_start_cur = cha_cursor_wo_anchor(cursor, 0);
	cat_unref_ptr(ll_start);

	ChaLineLocationWo *ll_end = cha_line_location_wo_new_with(page_index, priv->page_line_index+priv->row_count);
	cursor = cha_cursor_wo_new_ll_offset(ll_end, priv->column_end);
	*o_end_cur = cha_cursor_wo_anchor(cursor, 0);
	cat_unref_ptr(ll_end);
}

void cha_search_occurence_get_first_and_last_line_index(const ChaSearchOccurrence *occurrence, int *first_pl_index, int *last_pl_index) {
	const ChaSearchOccurrencePrivate *priv = cha_search_occurrence_get_instance_private((ChaSearchOccurrence *) occurrence);
	*first_pl_index = priv->page_line_index;
	*last_pl_index = priv->page_line_index+priv->row_count;
}

void cha_search_occurence_get_column_start_and_end(const ChaSearchOccurrence *occurrence, int *column_start, int *column_end) {
	const ChaSearchOccurrencePrivate *priv = cha_search_occurrence_get_instance_private((ChaSearchOccurrence *) occurrence);
	*column_start = priv->column_start;
	*column_end = priv->column_end;
}


int cha_search_occurence_get_page_line_index(const ChaSearchOccurrence *occurrence) {
	const ChaSearchOccurrencePrivate *priv = cha_search_occurrence_get_instance_private((ChaSearchOccurrence *) occurrence);
	return priv->page_line_index;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaSearchOccurrence *instance = CHA_SEARCH_OCCURRENCE(self);
	ChaSearchOccurrencePrivate *priv = cha_search_occurrence_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p page-line-index=%d, column-start=%d, column-end=%d]", iname, self, priv->page_line_index, priv->column_start, priv->column_end);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

