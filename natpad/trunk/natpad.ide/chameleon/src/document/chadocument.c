/*
   File:    chadocument.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 23, 2015
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

#include "chadocument.h"
#include "../io/chadocumentmanager.h"
#include "chaenrichmentdatamapwo.h"
#include "page/chafullpagewo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaDocument"
#include <logging/catlog.h>

struct _ChaDocumentPrivate {
	ChaDocumentManager *document_manager;
	int id;
	int version_seq;
	CatAtomicReference *a_revision_saved;
	CatAtomicReference *a_revision_ref;	// <ChaRevisionWo *> last stable edition
	ChaRevisionWo *e_revision;
	ChaEnrichmentDataMapWo *enrichment_map;
	CatWeakList *listeners;
	volatile gboolean read_only;
	volatile gboolean big_file_mode;

	CatLinkedList *e_revision_history;
	int history_index;
	ChaIConverter *input_converter;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaDocument, cha_document, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaDocument)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_notify_new_revision(CatWeakList *listeners, ChaRevisionWo *a_new_rev);
static void l_notify_history_modified(CatWeakList *listeners, int history_index, int history_length);

static void cha_document_class_init(ChaDocumentClass *clazz) {
	clazz->document_sequence = cat_atomic_integer_new();
	cat_ref_ignore(clazz->document_sequence);
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_document_init(ChaDocument *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaDocument *instance = CHA_DOCUMENT(object);
	ChaDocumentPrivate *priv = cha_document_get_instance_private(instance);
	if (priv->a_revision_ref) {
		ChaRevisionWo *a_rev = (ChaRevisionWo *) cat_atomic_reference_get(priv->a_revision_ref);
		if (a_rev) {
			cha_revision_wo_impoverish(a_rev);
			cat_unref_ptr(a_rev);
		}
	}
	cat_unref_ptr(priv->a_revision_ref);
	cat_unref_ptr(priv->a_revision_saved);
	cat_unref_ptr(priv->e_revision);
	cat_unref_ptr(priv->enrichment_map);
	cat_unref_ptr(priv->listeners);
	cat_unref_ptr(priv->e_revision_history);
	G_OBJECT_CLASS(cha_document_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_document_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void cha_document_construct(ChaDocument *document, struct _ChaDocumentManager *document_manager) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	priv->document_manager = document_manager;
	ChaDocumentClass *doc_class = CHA_DOCUMENT_GET_CLASS(document);
	priv->id = cat_atomic_integer_increment(doc_class->document_sequence);
	priv->version_seq = 0;
	priv->enrichment_map = cha_enrichment_data_map_wo_new();
	ChaRevisionWo *a_rev = cha_revision_wo_anchor(cha_revision_wo_new(), priv->version_seq++);
	cha_revision_wo_enrich(a_rev, priv->enrichment_map);
	priv->a_revision_ref = cat_atomic_reference_new_val((GObject *) a_rev);
	priv->a_revision_saved = cat_atomic_reference_new();
	cat_unref_ptr(a_rev);
	priv->e_revision = NULL;
	priv->listeners = cat_weak_list_new();
	priv->big_file_mode = FALSE;
	priv->e_revision_history = cat_linked_list_new();
	priv->history_index = -1;
	priv->big_file_mode = FALSE;
	priv->read_only = FALSE;
	priv->input_converter = cha_document_manager_get_converter(document_manager, NULL);
}


ChaDocument *cha_document_new(struct _ChaDocumentManager *document_manager) {
	ChaDocument *result = g_object_new(CHA_TYPE_DOCUMENT, NULL);
	cat_ref_anounce(result);
	cha_document_construct(result, document_manager);
	return result;
}

ChaIConverter *cha_document_get_input_converter(ChaDocument *document) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	return priv->input_converter;
}

void cha_document_set_input_converter(ChaDocument *document, ChaIConverter *converter) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	priv->input_converter = converter;
}


void cha_document_set_big_file_mode(ChaDocument *document, gboolean big_file_mode) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	priv->big_file_mode = big_file_mode;
}

gboolean cha_document_is_big_file_mode(ChaDocument *document) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	return priv->big_file_mode;
}

struct _ChaDocumentManager *cha_document_get_document_manager(ChaDocument *document) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	return priv->document_manager;
}


int cha_document_get_id(ChaDocument *document) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	return priv->id;
}

ChaRevisionWo *cha_document_get_current_revision_ref(ChaDocument *document) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	return (ChaRevisionWo *) cat_atomic_reference_get(priv->a_revision_ref);
}

ChaEnrichmentDataMapWo *cha_document_get_enrichment_data_map(ChaDocument *document) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	return priv->enrichment_map;
}


gboolean cha_document_is_editable(ChaDocument *document) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	return priv->e_revision!=NULL;
}

ChaRevisionWo *cha_document_get_editable_revision(ChaDocument *document) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	if (priv->e_revision == NULL) {
		ChaRevisionWo *a_rev = (ChaRevisionWo *) cat_atomic_reference_get_unsafe(priv->a_revision_ref);
		priv->e_revision = cha_revision_wo_create_editable(a_rev);
	}
	return priv->e_revision;
}




void cha_document_anchor_document(ChaDocument *document) {
	cha_document_anchor_document_full(document, CHA_DOCUMENT_ANCHOR_MODE_ADD_HISTORY);
}

cat_log_on_debug(
static void l_dump(ChaRevisionWo *a_rev) {
	if (a_rev==NULL) {
		return;
	}
	cat_log_print("DUMP", "%o", a_rev);
	int page_count = cha_revision_wo_page_count(a_rev);
	int page_idx;
	for(page_idx=0; page_idx<2 && page_idx<page_count; page_idx++) {
		ChaPageWo *a_page = cha_revision_wo_page_at(a_rev, page_idx);
		cat_log_print("DUMP", "page[%d] = %o", page_idx, a_page);
		int line_cnt = cha_page_wo_line_count(a_page);
		int line_idx;
		for(line_idx=0; line_idx<line_cnt && line_idx<5; line_idx++) {
			ChaLineWo *line = cha_page_wo_line_at(a_page, line_idx);
			cat_log_print("DUMP", "line[%d] = %o", line_idx, line);

		}
		if (line_cnt>5) {
			cat_log_print("DUMP", ".....");
		}
	}
}
)

void cha_document_anchor_document_full(ChaDocument *document, ChaDocumentAnchorMode anchor_mode) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	cat_log_info("anchoring");
	if (priv->e_revision) {
		if (cha_revision_wo_page_count(priv->e_revision)==0) {
			ChaPageWo *n_page = (ChaPageWo *) cha_full_page_wo_new(NULL);
			cha_revision_wo_append_page(priv->e_revision, n_page);
			cat_unref_ptr(n_page);
		}
		if (cha_revision_wo_page_count(priv->e_revision)==1) {
			ChaPageWo *e_page = cha_revision_wo_editable_page_at(priv->e_revision, 0);
			if (cha_page_wo_line_count(e_page)==0) {
				ChaLineWo *n_line = cha_line_wo_new();
				cha_page_wo_add_line(e_page, n_line);
				cat_unref(n_line);
			}
		}


		ChaCursorWo *new_cursor = cha_revision_wo_get_cursor(priv->e_revision);
		if (new_cursor) {
			ChaLineLocationWo *line_location = cha_cursor_wo_get_line_location(new_cursor);
			int page_index = cha_line_location_wo_get_page_index(line_location);
			int page_count = cha_revision_wo_page_count(priv->e_revision);
			if (page_index>=page_count && page_count>0) {
				ChaPageWo *last_page = cha_revision_wo_page_at(priv->e_revision, page_count-1);
				int page_line_count = cha_page_wo_line_count(last_page);
				ChaLineWo *last_line = cha_page_wo_line_at(last_page, page_line_count-1);
				ChaCursorWo *e_new_cursor = cha_revision_wo_get_editable_cursor(priv->e_revision);
				ChaLineLocationWo *e_line_location = cha_cursor_wo_get_editable_line_location(e_new_cursor);
				cha_line_location_wo_set_page_index(e_line_location, page_count-1);
				cha_line_location_wo_set_page_line_index(e_line_location, page_line_count-1);
				cha_cursor_wo_set_x_cursor_bytes(e_new_cursor, cha_line_wo_byte_count(last_line));
				cat_unref_ptr(last_line);
			} else if (page_index==page_count-1) {
				int page_line_index = cha_line_location_wo_get_page_line_index(line_location);
				ChaPageWo *last_page = cha_revision_wo_page_at(priv->e_revision, page_index);
				int page_line_count = cha_page_wo_line_count(last_page);
				if (page_line_index>=page_line_count && page_line_count>0) {
					ChaLineWo *last_line = cha_page_wo_line_at(last_page, page_line_count-1);
					ChaCursorWo *e_new_cursor = cha_revision_wo_get_editable_cursor(priv->e_revision);
					ChaLineLocationWo *e_line_location = cha_cursor_wo_get_editable_line_location(e_new_cursor);
					cha_line_location_wo_set_page_line_index(e_line_location, page_line_count-1);
					cha_cursor_wo_set_x_cursor_bytes(e_new_cursor, cha_line_wo_byte_count(last_line));
					cat_unref_ptr(last_line);
				}
			}
		}

		ChaRevisionWo *a_rev = (ChaRevisionWo *) cat_atomic_reference_get_unsafe(priv->a_revision_ref);
		ChaRevisionWo *a_new_rev = cha_revision_wo_anchor(priv->e_revision, priv->version_seq++);
		cat_log_info("anchored");


		cat_log_on_debug({
			l_dump(a_new_rev);
		});

		priv->e_revision = NULL;

		gboolean add_to_undo_redo = FALSE;

		if (a_new_rev != a_rev) {

			if (a_rev==NULL || cha_revision_wo_get_content_version(a_new_rev)!=cha_revision_wo_get_content_version(a_rev)) {
				add_to_undo_redo = TRUE;
			}

			cha_revision_wo_enrich(a_new_rev, priv->enrichment_map);
			cat_log_info("enriched: a_new_rev=%p", a_new_rev);
			cha_revision_wo_impoverish(a_rev);
			cat_log_info("impoverished: a_rev=%p", a_rev);
			cat_atomic_reference_set(priv->a_revision_ref, (GObject *) a_new_rev);

			/* notify listeners */
			l_notify_new_revision(priv->listeners, a_new_rev);
		}

//		add_history = FALSE;
		int length = cat_linked_list_size(priv->e_revision_history);
		if ((length==0 || add_to_undo_redo) && anchor_mode==CHA_DOCUMENT_ANCHOR_MODE_ADD_HISTORY) {


			cat_log_debug("e_revision_history=%o, history_index=%d", priv->e_revision_history, priv->history_index);

			if (priv->history_index+1<length) {
				cat_linked_listed_remove_tail(priv->e_revision_history, priv->history_index+1);
				length = cat_linked_list_size(priv->e_revision_history);
			}
			cat_log_debug("e_revision_history=%o, history_index=%d", priv->e_revision_history, priv->history_index);

//			if (length>0) {
//				ChaRevisionWo *a_last = cat_linked_list_get(priv->e_revision_history, length-1);
//				ChaRevisionChangeFlags f = cha_revision_wo_compare(a_last, a_new_rev);
//			}

			cat_linked_list_append(priv->e_revision_history, (GObject *) a_new_rev);
			priv->history_index++;
			cat_log_debug("e_revision_history=%o, history_index=%d", priv->e_revision_history, priv->history_index);
			if (cat_linked_list_size(priv->e_revision_history)>10) {
				cat_linked_list_remove_first(priv->e_revision_history, NULL);
				priv->history_index--;
			}
			cat_log_debug("e_revision_history=%o, history_index=%d", priv->e_revision_history, priv->history_index);
			/* notify listeners */
			l_notify_history_modified(priv->listeners, priv->history_index, cat_linked_list_size(priv->e_revision_history));
		} else if (anchor_mode==CHA_DOCUMENT_ANCHOR_MODE_REPLACE_LAST_HISTORY) {
			int length = cat_linked_list_size(priv->e_revision_history);
			cat_log_debug("e_revision_history=%o, history_index=%d", priv->e_revision_history, priv->history_index);

			if (priv->history_index+1<length) {
				cat_linked_listed_remove_tail(priv->e_revision_history, priv->history_index+1);
			}
			cat_log_debug("e_revision_history=%o, history_index=%d", priv->e_revision_history, priv->history_index);

			length = cat_linked_list_size(priv->e_revision_history);
			if (length>0) {
				cat_linked_list_remove_last(priv->e_revision_history, NULL);
			}
			cat_linked_list_append(priv->e_revision_history, (GObject *) a_new_rev);

			/* notify listeners */
			l_notify_history_modified(priv->listeners, priv->history_index, cat_linked_list_size(priv->e_revision_history));
		}

		cat_unref_ptr(a_new_rev);
		cat_log_trace("done");
	}
}

static void l_notify_new_saved_version(CatWeakList *listeners, ChaRevisionWo *a_new_rev_saved);

void cha_document_set_saved_revision(ChaDocument *document, ChaRevisionWo *a_revision) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	cat_atomic_reference_set(priv->a_revision_saved, (GObject *) a_revision);
	l_notify_new_saved_version(priv->listeners, a_revision);
}

gboolean cha_document_is_saved_revision(const ChaDocument *document) {
	const ChaDocumentPrivate *priv = cha_document_get_instance_private((ChaDocument *) document);
	ChaRevisionWo *saved_ref = (ChaRevisionWo *) cat_atomic_reference_get(priv->a_revision_saved);
	gboolean result = FALSE;
	if (saved_ref) {
		int saved_pl_version = cha_revision_wo_get_page_list_version(saved_ref);
		ChaRevisionWo *cur_ref = (ChaRevisionWo *) cat_atomic_reference_get(priv->a_revision_ref);
		result = saved_pl_version==cha_revision_wo_get_page_list_version(cur_ref);
		cat_log_debug("saved_plist_version=%d, cur_plist_version=%d", saved_pl_version, cha_revision_wo_get_page_list_version(cur_ref));
		cat_unref_ptr(cur_ref);
		cat_unref_ptr(saved_ref);
	}
	return result;
}

static void l_notify_mode_changed(ChaDocument *document);

void cha_document_set_read_only(ChaDocument *document, gboolean read_only) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	if (priv->read_only!=read_only) {
		priv->read_only = read_only;
		l_notify_mode_changed(document);
	}
}

gboolean cha_document_is_read_only(const ChaDocument *document) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private((ChaDocument *) document);
	return priv->read_only;
}


void cha_document_post_enrichment_slot_notify(ChaDocument *document, ChaRevisionWo *a_revision, GObject *slot_key, gpointer user_data) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	/* notify listeners */
	CatIIterator *iter = cat_weak_list_iterator(priv->listeners);
	while(cat_iiterator_has_next(iter)) {
		ChaIDocumentListener *listener = (ChaIDocumentListener *) cat_iiterator_next(iter);
		ChaIDocumentListenerInterface *iface = CHA_IDOCUMENT_LISTENER_GET_INTERFACE(listener);
		if (iface->onEnrichmentSlotNotify) {
			iface->onEnrichmentSlotNotify(listener, a_revision, slot_key, user_data);
		}
	}
	cat_unref_ptr(iter);
}

int cha_document_register_enrichment_slot(ChaDocument *document, GObject *slot_key) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	ChaEnrichmentDataMapWo *e_map = cha_enrichment_data_map_wo_create_editable(priv->enrichment_map);
	int index = cha_enrichment_data_map_wo_add_slot(e_map, slot_key);
	if (index>=0) {
		cat_log_debug("e_map=%O", e_map);

		e_map = cha_enrichment_data_map_wo_anchor(e_map, cat_wo_get_version(CAT_WO(e_map))+1);
		cat_log_debug("slot added: rev=%p, slot_key=%o, index=%d, old-map=%p, map=%o", cat_atomic_reference_get_unsafe(priv->a_revision_ref), slot_key, index, priv->enrichment_map, e_map);
		ChaRevisionWo *a_rev = (ChaRevisionWo *) cat_atomic_reference_get_unsafe(priv->a_revision_ref);
		cha_revision_wo_enrichment_remap(a_rev, priv->enrichment_map, e_map, index, -1);
		cat_ref_swap(priv->enrichment_map, e_map);

		/* notify listeners */
		CatIIterator *iter = cat_weak_list_iterator(priv->listeners);
		while(cat_iiterator_has_next(iter)) {
			ChaIDocumentListener *listener = (ChaIDocumentListener *) cat_iiterator_next(iter);
			ChaIDocumentListenerInterface *iface = CHA_IDOCUMENT_LISTENER_GET_INTERFACE(listener);
			if (iface->onSlotRegistered) {
				iface->onSlotRegistered(listener, a_rev, e_map, slot_key, index);
			}
		}
		cat_unref_ptr(iter);
	}
	cat_unref_ptr(e_map);
	return index;
}

void cha_document_unregister_enrichment_slot(ChaDocument *document, GObject *slot_key) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	ChaEnrichmentDataMapWo *e_map = cha_enrichment_data_map_wo_create_editable(priv->enrichment_map);
	int index = cha_enrichment_data_map_wo_remove_slot(e_map, slot_key);
	if (index>=0) {
		e_map = cha_enrichment_data_map_wo_anchor(e_map, cat_wo_get_version(CAT_WO(e_map))+1);
		ChaRevisionWo *a_rev = (ChaRevisionWo *) cat_atomic_reference_get_unsafe(priv->a_revision_ref);
		cha_revision_wo_enrichment_remap(a_rev, priv->enrichment_map, e_map, -1, index);
		cat_ref_swap(priv->enrichment_map, e_map);
	}
	cat_unref_ptr(e_map);
}

static void l_set_history_ref(ChaDocumentPrivate *priv, ChaRevisionWo *a_new_rev) {
	ChaRevisionWo *a_rev = (ChaRevisionWo *) cat_atomic_reference_get_unsafe(priv->a_revision_ref);
	cat_unref_ptr(priv->e_revision);
	if (a_new_rev != a_rev) {

		cha_revision_wo_enrich(a_new_rev, priv->enrichment_map);
		cat_log_trace("enriched: a_new_rev=%o", a_new_rev);
		cha_revision_wo_impoverish(a_rev);
		cat_log_trace("impoverished: a_rev=%o", a_rev);
		cat_atomic_reference_set(priv->a_revision_ref, (GObject *) a_new_rev);

		cat_log_info("was set:");

		/* notify listeners */
		l_notify_new_revision(priv->listeners, a_new_rev);
	}

	/* notify listeners */
	l_notify_history_modified(priv->listeners, priv->history_index, cat_linked_list_size(priv->e_revision_history));
	cat_log_trace("done");

}

void cha_document_undo(ChaDocument *document) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	if (priv->history_index>0) {
		priv->history_index--;
		ChaRevisionWo *a_new_rev = (ChaRevisionWo *) cat_linked_list_get(priv->e_revision_history, priv->history_index);
		l_set_history_ref(priv, a_new_rev);
	}
}

void cha_document_redo(ChaDocument *document) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	int hist_size = cat_linked_list_size(priv->e_revision_history);
	if (priv->history_index<hist_size-1) {
		priv->history_index++;
		ChaRevisionWo *a_new_rev = (ChaRevisionWo *) cat_linked_list_get(priv->e_revision_history, priv->history_index);
		l_set_history_ref(priv, a_new_rev);
	}
}

void cha_document_get_history_info(const ChaDocument *document, int *history_index, int *history_length) {
	const ChaDocumentPrivate *priv = cha_document_get_instance_private((ChaDocument *) document);
	if (history_index) {
		*history_index = priv->history_index;
	}
	if (history_length) {
		*history_length = cat_linked_list_size(priv->e_revision_history);
	}
}


int cha_document_hash_code(ChaDocument *document) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	return priv->id;
}

gboolean cha_document_equal(ChaDocument *document_a, ChaDocument *document_b) {
	return document_a==document_b;
}


static void l_notify_new_revision(CatWeakList *listeners, ChaRevisionWo *a_new_rev) {
	cat_log_info("notify new-rev:");
	CatIIterator *iter = cat_weak_list_iterator(listeners);
	cat_log_info("got iter");
	while(cat_iiterator_has_next(iter)) {
		ChaIDocumentListener *listener = (ChaIDocumentListener *) cat_iiterator_next(iter);
		ChaIDocumentListenerInterface *iface = CHA_IDOCUMENT_LISTENER_GET_INTERFACE(listener);
		if (iface->onNewRevision) {
			cat_log_info("listener: %o", listener);
			iface->onNewRevision(listener, a_new_rev);
		}
	}
	cat_unref_ptr(iter);
}

static void l_notify_history_modified(CatWeakList *listeners, int history_index, int history_length) {
	cat_log_debug("history_index=%d, history_length=%d", history_index, history_length);
	CatIIterator *iter = cat_weak_list_iterator(listeners);
	while(cat_iiterator_has_next(iter)) {
		ChaIDocumentListener *listener = (ChaIDocumentListener *) cat_iiterator_next(iter);
		ChaIDocumentListenerInterface *iface = CHA_IDOCUMENT_LISTENER_GET_INTERFACE(listener);
		if (iface->onHistoryModified) {
			cat_log_trace("listener: %o", listener);
			iface->onHistoryModified(listener, history_index, history_length);
		}
	}
	cat_unref_ptr(iter);
}

static void l_notify_new_saved_version(CatWeakList *listeners, ChaRevisionWo *a_new_rev_saved) {
	CatIIterator *iter = cat_weak_list_iterator(listeners);
	while(cat_iiterator_has_next(iter)) {
		ChaIDocumentListener *listener = (ChaIDocumentListener *) cat_iiterator_next(iter);
		ChaIDocumentListenerInterface *iface = CHA_IDOCUMENT_LISTENER_GET_INTERFACE(listener);
		if (iface->onNewSavedRevision) {
			cat_log_trace("listener: %o", listener);
			iface->onNewSavedRevision(listener, a_new_rev_saved);
		}
	}
	cat_unref_ptr(iter);
}


static void l_notify_mode_changed(ChaDocument *document) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	ChaModeInfo mode_info;
	mode_info.big_file_mode = priv->big_file_mode;
	mode_info.read_only = priv->read_only;
	CatIIterator *iter = cat_weak_list_iterator(priv->listeners);
	while(cat_iiterator_has_next(iter)) {
		ChaIDocumentListener *listener = (ChaIDocumentListener *) cat_iiterator_next(iter);
		ChaIDocumentListenerInterface *iface = CHA_IDOCUMENT_LISTENER_GET_INTERFACE(listener);
		if (iface->onModeChanged) {
			cat_log_trace("listener: %o", listener);
			iface->onModeChanged(listener, &mode_info);
		}
	}
	cat_unref_ptr(iter);
}

void cha_document_add_listener(ChaDocument *document, ChaIDocumentListener *listener) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	cat_weak_list_append_once(priv->listeners, (GObject *) listener);
}

void cha_document_remove_listener(ChaDocument *document, ChaIDocumentListener *listener) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private(document);
	cat_weak_list_remove(priv->listeners, (GObject *) listener);
}


CatArrayWo *cha_document_create_line_list(const ChaDocument *document, const CatStringWo *text) {
	ChaDocumentPrivate *priv = cha_document_get_instance_private((ChaDocument *) document);
	ChaIConverter *converter = priv->input_converter;
	if (cat_string_wo_length(text)==0) {
		return NULL;
	}
	int len = cat_string_wo_length(text);
	CatArrayWo *result = cat_array_wo_new_size(len / 60);	// rough estimation of how much lines we expect.

	int idx = 0;
	int idx_13 = 0;
	int idx_10 = 0;
	idx_13 = cat_string_wo_index_of(text, (gchar) 0xd);
	idx_10 = cat_string_wo_index_of(text, (gchar) 0xa);
	const char *text_ptr = cat_string_wo_getchars(text);
	while(TRUE) {
		cat_log_debug("idx_13=%d, idx_10=%d", idx_13, idx_10);
		if (idx_13<0) {
			if (idx_10<0) {
				int length = cat_string_wo_length(text)-idx;
				if (length>0) {
					CatStringWo *new_line = cat_string_wo_new_with_len(text_ptr+idx, length);
//					CatStringWo *new_line = cha_charset_converter_convert(converter, text_ptr+idx, length, NULL, NULL);
					ChaLineWo *e_line = cha_line_wo_new_with(new_line, CHA_LINE_END_NONE);
					cat_log_debug("last e_line=%o", e_line);
					cat_array_wo_append(result, (GObject *) e_line);
					cat_unref_ptr(e_line);
				}
				break;
			}
			int length = idx_10-idx;
			CatStringWo *new_line = cat_string_wo_new_with_len(text_ptr+idx, length);
//			CatStringWo *new_line = cha_charset_converter_convert(converter, text_ptr+idx, length, NULL, NULL);
			ChaLineWo *e_line = cha_line_wo_new_with(new_line, CHA_LINE_END_LF);
			cat_array_wo_append(result, (GObject *) e_line);
			cat_unref_ptr(e_line);
			idx = idx_10+1;
			idx_10 = cat_string_wo_index_of_from(text, (gchar) 0xa, idx);
		} else if (idx_10<0) {
			int length = idx_13-idx;
			CatStringWo *new_line = cat_string_wo_new_with_len(text_ptr+idx, length);
//			CatStringWo *new_line = cha_charset_converter_convert(converter, text_ptr+idx, length, NULL, NULL);
			ChaLineWo *e_line = cha_line_wo_new_with(new_line, CHA_LINE_END_CR);
			cat_array_wo_append(result, (GObject *) e_line);
			cat_unref_ptr(e_line);
			idx = idx_13+1;
			idx_13 = cat_string_wo_index_of_from(text, (gchar) 0xd, idx);
		} else {
			if (idx_10<idx_13) {
				int length = idx_10-idx;
				CatStringWo *new_line = cat_string_wo_new_with_len(text_ptr+idx, length);
//				CatStringWo *new_line = cha_charset_converter_convert(converter, text_ptr+idx, length, NULL, NULL);
				ChaLineEnd line_end = CHA_LINE_END_LF;
				if (idx_10+1==idx_13) {
					line_end = CHA_LINE_END_LFCR;
					idx = idx_13+1;
					idx_10 = cat_string_wo_index_of_from(text, (gchar) 0xa, idx);
					idx_13 = cat_string_wo_index_of_from(text, (gchar) 0xd, idx);
				} else {
					idx = idx_10+1;
					idx_10 = cat_string_wo_index_of_from(text, (gchar) 0xa, idx);
				}
				ChaLineWo *e_line = cha_line_wo_new_with(new_line, line_end);
				cat_array_wo_append(result, (GObject *) e_line);
				cat_unref_ptr(e_line);
			} else {
				int length = idx_13-idx;
				CatStringWo *new_line = cat_string_wo_new_with_len(text_ptr+idx, length);
//				CatStringWo *new_line = cha_charset_converter_convert(converter, text_ptr+idx, length, NULL, NULL);
				ChaLineEnd line_end = CHA_LINE_END_CR;
				if (idx_13+1==idx_10) {
					line_end = CHA_LINE_END_CRLF;
					idx = idx_10+1;
					idx_10 = cat_string_wo_index_of_from(text, (gchar) 0xa, idx);
					idx_13 = cat_string_wo_index_of_from(text, (gchar) 0xd, idx);
				} else {
					idx = idx_13+1;
					idx_13 = cat_string_wo_index_of_from(text, (gchar) 0xd, idx);
				}
				ChaLineWo *e_line = cha_line_wo_new_with(new_line, line_end);
				cat_array_wo_append(result, (GObject *) e_line);
				cat_unref_ptr(e_line);
			}
		}
	}
	return result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaDocument *instance = CHA_DOCUMENT(self);
	ChaDocumentPrivate *priv = cha_document_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	ChaRevisionWo *a_rev_ref = (ChaRevisionWo *) cat_atomic_reference_get(priv->a_revision_ref);
	cat_string_wo_format(append_to, "%s[%p id=%d, a_revision=%o]", iname, self, priv->id, a_rev_ref);
	cat_unref_ptr(a_rev_ref);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

