/*
   File:    chasearchservice.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Oct 16, 2015
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

#include "chasearchservice.h"
#include "chasearchdialog.h"
#include "chasearchoccurrence.h"
#include "chasearchpageoccurrences.h"
#include "chaoccurrencesresultset.h"


#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaSearchService"
#include <logging/catlog.h>

struct _ChaSearchServicePrivate {
	ChaSearchQueryWo *a_query;
	CatWeakReference *editor_ref;
	ChaOccurrencesResultSet *occurrences_result_set;
	ChaSearchDialog *dialog;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaSearchService, cha_search_service, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaSearchService)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_search_service_class_init(ChaSearchServiceClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_search_service_init(ChaSearchService *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaSearchService *instance = CHA_SEARCH_SERVICE(object);
	ChaSearchServicePrivate *priv = cha_search_service_get_instance_private(instance);
	cat_unref_ptr(priv->a_query);
	cat_unref_ptr(priv->dialog);
	cat_unref_ptr(priv->editor_ref);
	cat_unref_ptr(priv->occurrences_result_set);
	G_OBJECT_CLASS(cha_search_service_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_search_service_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void cha_search_service_construct(ChaSearchService *service) {
	ChaSearchServicePrivate *priv = cha_search_service_get_instance_private(service);
	priv->a_query = NULL;
	priv->occurrences_result_set = NULL;
	priv->editor_ref = cat_weak_reference_new(NULL);
	priv->dialog = NULL;
}


ChaSearchService *cha_search_service_new() {
	ChaSearchService *result = g_object_new(CHA_TYPE_SEARCH_SERVICE, NULL);
	cat_ref_anounce(result);
	cha_search_service_construct(result);
	return result;
}

void cha_search_service_set_editor(ChaSearchService *search_service, struct _ChaEditor *editor) {
	ChaSearchServicePrivate *priv = cha_search_service_get_instance_private(search_service);
	ChaEditor *cur_editor = (ChaEditor *) cat_weak_reference_get(priv->editor_ref);
	if (editor==cur_editor) {
		if (editor==NULL) {
			cat_unref_ptr(priv->occurrences_result_set);
		}
		cat_unref_ptr(cur_editor);
		return;
	}

	cat_weak_reference_set(priv->editor_ref, (GObject *) editor);
	cat_unref_ptr(priv->occurrences_result_set);
}

ChaSearchQueryWo *cha_search_service_get_query(ChaSearchService *search_service) {
	ChaSearchServicePrivate *priv = cha_search_service_get_instance_private(search_service);
	return priv->a_query;
}

void cha_search_service_set_query(ChaSearchService *search_service, ChaSearchQueryWo *ca_query) {
	ChaSearchServicePrivate *priv = cha_search_service_get_instance_private(search_service);
	if (cha_search_query_wo_equal(priv->a_query, ca_query)) {
		cat_unref_ptr(ca_query);
		return;
	}

	ca_query = cha_search_query_wo_anchor(ca_query, 0);
	cat_unref_ptr(priv->a_query);
	priv->a_query = ca_query;

	cat_unref_ptr(priv->occurrences_result_set);
}

void cha_search_service_find(ChaSearchService *search_service, gboolean backward_search) {
	ChaSearchServicePrivate *priv = cha_search_service_get_instance_private(search_service);
	ChaEditor *editor = (ChaEditor *) cat_weak_reference_get(priv->editor_ref);
	cat_log_debug("editor=%o", editor);
	if (editor==NULL || priv->a_query==NULL) {
		cat_unref_ptr(editor);
		return;
	}

	ChaDocument *document = cha_editor_get_document(editor);
	cat_log_debug("document=%o", document);
	ChaRevisionWo *a_rev = cha_document_get_current_revision_ref(document);

	if (priv->occurrences_result_set==NULL) {
		priv->occurrences_result_set = cha_occurrences_result_set_new(priv->a_query, NULL);
	}

	cha_occurences_result_set_set_revision(priv->occurrences_result_set, a_rev);

	if (priv->occurrences_result_set) {

		ChaDocumentView *document_view = cha_editor_get_document_view(editor);
		ChaCursorWo *cursor = cha_revision_wo_get_cursor(a_rev);


		int page_index = 0;

		ChaSearchOccurrence *occurence = NULL;

		if (backward_search) {
			ChaSelection *selection = cha_document_view_get_selection(document_view);
			if (selection) {
				ChaCursorWo *sel_s = cha_selection_get_start(selection);
				ChaCursorWo *sel_e = cha_selection_get_end(selection);

				// TODO type is incorrect
				ChaCursorOrder or = cha_cursor_wo_compare(sel_s, sel_e);
				if ((or==CHA_CURSOR_ABOVE) || (or==CHA_CURSOR_LEFT)) {
					cursor = sel_s;
				} else {
					cursor = sel_e;
				}
				cat_log_debug("cursor=%o, sel_e=%o, sel_s=%o", cursor, sel_e, sel_s);
			}
			occurence = cha_occurrences_result_set_find_backward(priv->occurrences_result_set, cursor, &page_index);
		} else {
			occurence = cha_occurrences_result_set_find_forward(priv->occurrences_result_set, cursor, &page_index);
		}

		cat_log_debug("occurence=%o", occurence);

		if (occurence!=NULL) {
			ChaCursorWo *o_start_cur = NULL;
			ChaCursorWo *o_end_cur = NULL;
			cha_search_occurence_create_cursors(occurence, page_index, &o_start_cur, &o_end_cur);

			cat_log_debug("o_start_cur=%o, o_end_cur=%o", o_start_cur, o_end_cur);


			gboolean is_editable = cha_document_is_editable(document);
			ChaRevisionWo *e_rev = cha_document_get_editable_revision(document);
			cha_revision_wo_set_cursor(e_rev, o_end_cur);

			cha_document_view_clear_selection(document_view, e_rev);
			ChaSelection *selection = cha_document_view_get_or_create_selection(document_view, e_rev, o_start_cur, FALSE);
			cha_selection_set_end(selection, o_end_cur);

			cha_document_view_notify_selection_changed(document_view);


			if (!is_editable) {
				cat_log_debug("anchoring");
				cha_document_anchor_document(document);
				cha_document_view_invalidate_lines(document_view);
				cha_document_view_move_view_to_focus(document_view, TRUE);
			}
		}

	}
	cat_unref_ptr(a_rev);

	cat_unref_ptr(editor);
}

void cha_search_service_mark(ChaSearchService *search_service) {
	ChaSearchServicePrivate *priv = cha_search_service_get_instance_private(search_service);
	ChaEditor *editor = (ChaEditor *) cat_weak_reference_get(priv->editor_ref);
	cat_log_debug("editor=%o", editor);
	if (editor==NULL || priv->a_query==NULL) {
		cat_unref_ptr(editor);
		return;
	}

	ChaDocumentView *document_view = cha_editor_get_document_view(editor);
	ChaDocument *document = cha_editor_get_document(editor);
	cat_log_debug("document=%o", document);
//	ChaRevisionWo *a_rev = cha_document_get_current_revision_ref(document);
//	int content_version = cha_revision_wo_get_content_version(a_rev);

	ChaOccurrencesResultSet *mark_result_set = cha_occurrences_result_set_new(priv->a_query, NULL);
	gboolean is_editable = cha_document_is_editable(document);
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);
	cha_occurences_result_set_set_revision(mark_result_set, e_revision);


	int page_cnt = cha_revision_wo_page_count(e_revision);
	int page_idx;
	for(page_idx=0; page_idx<page_cnt; page_idx++) {
		ChaPageWo *e_page = NULL;
		ChaSearchPageOccurrences *pg_occurrences = cha_occurences_result_set_get_page_occurrences(mark_result_set, page_idx);
		int page_ln_cnt = cha_search_page_occurrences_count(pg_occurrences);
		int page_ln_idx;
		for(page_ln_idx=0; page_ln_idx<page_ln_cnt; page_ln_idx++) {
			CatArrayWo *line_ocs = cha_search_page_occurrences_get_at(pg_occurrences, page_ln_idx);
			if (line_ocs!=NULL && cat_array_wo_size(line_ocs)>0) {
				if (e_page==NULL) {
					e_page = cha_revision_wo_editable_page_at(e_revision, page_idx);
				}
				cha_page_wo_set_line_mark(e_page, page_ln_idx);
			}
		}
	}

	if (!is_editable) {
		cat_log_debug("anchoring");
		cha_document_anchor_document(document);
		cha_document_view_invalidate_lines(document_view);
	}

}



void cha_search_service_show_dialog(ChaSearchService *search_service, GtkWindow *owner, struct _ChaEditor *editor) {
	ChaSearchServicePrivate *priv = cha_search_service_get_instance_private(search_service);
	if (priv->dialog==NULL) {
		priv->dialog = cha_search_dialog_new(search_service);
		cat_ref_sink_ptr(priv->dialog);
	}
	cha_search_dialog_set_editor(priv->dialog, editor);
	cha_search_dialog_show(priv->dialog, owner);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaSearchService *instance = CHA_SEARCH_SERVICE(self);
	ChaSearchServicePrivate *priv = cha_search_service_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p query=%o]", iname, self, priv->a_query);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
