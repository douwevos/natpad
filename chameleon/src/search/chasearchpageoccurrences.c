/*
   File:    chasearchpageoccurrences.c
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

#include "chasearchpageoccurrences.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaSearchPageOccurrences"
#include <logging/catlog.h>

struct _ChaSearchPageOccurrencesPrivate {
	CatArrayWo *e_occurrences;	// <CatArrayWo<ChaSearchOccurrence *> *>
	int page_index;
	ChaPageWo *a_page;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaSearchPageOccurrences, cha_search_page_occurrences, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaSearchPageOccurrences)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_search_page_occurrences_class_init(ChaSearchPageOccurrencesClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_search_page_occurrences_init(ChaSearchPageOccurrences *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaSearchPageOccurrences *instance = CHA_SEARCH_PAGE_OCCURRENCES(object);
	ChaSearchPageOccurrencesPrivate *priv = cha_search_page_occurrences_get_instance_private(instance);
	cat_unref_ptr(priv->e_occurrences);
	cat_unref_ptr(priv->a_page);
	G_OBJECT_CLASS(cha_search_page_occurrences_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_search_page_occurrences_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaSearchPageOccurrences *cha_search_page_occurrences_new(ChaPageWo *a_page, int page_index) {
	ChaSearchPageOccurrences *result = g_object_new(CHA_TYPE_SEARCH_PAGE_OCCURRENCES, NULL);
	cat_ref_anounce(result);
	ChaSearchPageOccurrencesPrivate *priv = cha_search_page_occurrences_get_instance_private(result);

	priv->e_occurrences = cat_array_wo_new_size(cha_page_wo_line_count(a_page));
	cat_array_wo_pad(priv->e_occurrences, cha_page_wo_line_count(a_page), NULL);
	priv->a_page = cat_ref_ptr(a_page);
	priv->page_index = page_index;
	return result;
}


void cha_search_page_occurrences_add(ChaSearchPageOccurrences *page_occurrences, ChaSearchOccurrence *occurrence) {
	ChaSearchPageOccurrencesPrivate *priv = cha_search_page_occurrences_get_instance_private(page_occurrences);

	int fidx;
	int lidx;
	int line_count = cat_array_wo_size(priv->e_occurrences);
	cha_search_occurence_get_first_and_last_line_index(occurrence, &fidx, &lidx);
	cat_log_debug("fidx=%d, lidx=%d, line_count=%d", fidx, lidx, line_count);
	while(fidx<line_count && fidx<=lidx) {
		CatArrayWo *line_occurrences = (CatArrayWo *) cat_array_wo_get(priv->e_occurrences, fidx);
		if (line_occurrences==NULL) {
			line_occurrences = cat_array_wo_new();
			cat_array_wo_set(priv->e_occurrences, (GObject*) line_occurrences, lidx, NULL);
			cat_unref(line_occurrences);
		}
		cat_array_wo_append(line_occurrences, (GObject *) occurrence);
		fidx++;
	}
}

int cha_search_page_occurrences_count(const ChaSearchPageOccurrences *page_occurrences) {
	const ChaSearchPageOccurrencesPrivate *priv = cha_search_page_occurrences_get_instance_private((ChaSearchPageOccurrences *) page_occurrences);
	return cat_array_wo_size(priv->e_occurrences);
}

int cha_search_page_occurrences_get_page_index(const ChaSearchPageOccurrences *page_occurrences) {
	const ChaSearchPageOccurrencesPrivate *priv = cha_search_page_occurrences_get_instance_private((ChaSearchPageOccurrences *) page_occurrences);
	return priv->page_index;
}

ChaPageWo *cha_search_page_occurrences_get_page(const ChaSearchPageOccurrences *page_occurrences) {
	const ChaSearchPageOccurrencesPrivate *priv = cha_search_page_occurrences_get_instance_private((ChaSearchPageOccurrences *) page_occurrences);
	return priv->a_page;
}

CatArrayWo *cha_search_page_occurrences_get_at(const ChaSearchPageOccurrences *page_occurrences, int index) {
	const ChaSearchPageOccurrencesPrivate *priv = cha_search_page_occurrences_get_instance_private((ChaSearchPageOccurrences *) page_occurrences);
	return (CatArrayWo *) cat_array_wo_get(priv->e_occurrences, index);
}

CatArrayWo *cha_search_page_occurrences_enlist_for_line(const ChaSearchPageOccurrences *page_occurrences, int line_index) {
	const ChaSearchPageOccurrencesPrivate *priv = cha_search_page_occurrences_get_instance_private((ChaSearchPageOccurrences *) page_occurrences);
	CatArrayWo *result = (CatArrayWo *) cat_array_wo_get(priv->e_occurrences, line_index);
//	CatArrayWo *result = NULL;
//
//	CatIIterator *iter = cat_array_wo_iterator(priv->e_occurrences);
//	while(cat_iiterator_has_next(iter)) {
//		ChaSearchOccurrence *occurrence = (ChaSearchOccurrence *) cat_iiterator_next(iter);
//		if (cha_search_occurence_get_page_line_index(occurrence)==line_index) {
//			if (result == NULL) {
//				result = cat_array_wo_new();
//			}
//			cat_array_wo_append(result, (GObject *) occurrence);
//		}
//	}
	return result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaSearchPageOccurrences *instance = CHA_SEARCH_PAGE_OCCURRENCES(self);
	ChaSearchPageOccurrencesPrivate *priv = cha_search_page_occurrences_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p occurrences=%o]", iname, self, priv->e_occurrences);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
