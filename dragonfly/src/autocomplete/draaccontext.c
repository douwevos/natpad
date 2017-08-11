/*
   File:    draaccontext.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 18, 2015
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

#include "draaccontext.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraAcContext"
#include <logging/catlog.h>

struct _DraAcContextPrivate {
	ChaEditor *editor;
	CatWeakList *listeners;
	ChaRevisionWo *a_revision;
	ChaCursorWo *cursor_start;
	ChaCursorWo *cursor_pre_end;
	ChaCursorWo *cursor_filter;
	long long int cursor_row;
	int cursor_column;
	CatStringWo *a_entry_filter_text;
	CatArrayWo *pre_filtered_entries;
	CatArrayWo *e_entries;

	CatLock *lock;
	int content_requests_pending;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraAcContext, dra_ac_context, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraAcContext)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_ac_context_class_init(DraAcContextClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_ac_context_init(DraAcContext *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraAcContext *instance = DRA_AC_CONTEXT(object);
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(instance);
	cat_unref_ptr(priv->a_entry_filter_text);
	cat_unref_ptr(priv->a_revision);
	cat_unref_ptr(priv->cursor_filter);
	cat_unref_ptr(priv->cursor_pre_end);
	cat_unref_ptr(priv->cursor_start);
	cat_unref_ptr(priv->e_entries);
	cat_unref_ptr(priv->listeners);
	cat_unref_ptr(priv->lock);
	cat_unref_ptr(priv->pre_filtered_entries);
	G_OBJECT_CLASS(dra_ac_context_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_ac_context_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static CatStringWo *l_extract_filter_text_from_text(ChaRevisionWo *a_rev, ChaCursorWo *cursor, int *mxpos, int *first_char) {

	CatStringWo *entry_filter_text = cat_string_wo_new();
	ChaLineLocationWo *ll = cha_cursor_wo_get_line_location(cursor);
	ChaPageWo *a_page = cha_revision_wo_page_at(a_rev, cha_line_location_wo_get_page_index(ll));
	const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(a_page, cha_line_location_wo_get_page_line_index(ll), FALSE);
	int txt_len = utf8_text.text_len;
	const char *txt_data = utf8_text.text;
	int start_alpha = 0;
	if (txt_len>0) {
		const CatStringWo *line = cat_string_wo_new_data_len(txt_data, txt_len);
		int xpos = cha_cursor_wo_get_x_cursor_bytes(cursor);
		if (xpos>txt_len) {
			xpos = txt_len;
		}
		if (mxpos) {
			*mxpos = xpos;
		}
		int xidx = 0;

		while(xidx<xpos) {
			gunichar uch = cat_string_wo_unichar_at(line, &xidx);
			if (uch==-1) {
				cat_log_warn("invalid utf8 character detected");
				xidx++;
				start_alpha = xidx;
			} else {
				if (!(g_unichar_isalpha(uch) || uch=='.' || uch=='_')) {
					start_alpha = xidx;
				}
			}
		}

		if (start_alpha<xpos) {
			cat_string_wo_append_chars_len(entry_filter_text, txt_data+start_alpha, xpos-start_alpha);
		}
		cat_unref_ptr(line);
	}
	if (first_char) {
		*first_char = start_alpha;
	}
	cha_utf8_text_cleanup(&utf8_text);
	return entry_filter_text;
}

DraAcContext *dra_ac_context_new(ChaEditor *editor) {
	DraAcContext *result = g_object_new(DRA_TYPE_AC_CONTEXT, NULL);
	cat_ref_anounce(result);
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(result);
	priv->listeners = cat_weak_list_new();
	priv->editor = cat_ref_ptr(editor);
	ChaDocumentView *document_view = cha_editor_get_document_view(editor);
	ChaRevisionWo *a_rev = cha_document_view_get_revision(document_view);
	priv->e_entries = cat_array_wo_new();
	priv->pre_filtered_entries = cat_ref_ptr(priv->e_entries);
	priv->lock = cat_lock_new();
	priv->a_revision = cat_ref_ptr(a_rev);
	ChaCursorWo *cursor_at_start = cha_cursor_wo_clone(cha_revision_wo_get_cursor(a_rev), CAT_CLONE_DEPTH_NONE);
	priv->cursor_pre_end = cha_cursor_wo_anchor(cursor_at_start, 0);
	priv->cursor_filter = cat_ref_ptr(priv->cursor_pre_end);

	priv->cursor_row = cha_revision_wo_calculate_row(a_rev, cha_cursor_wo_get_line_location(cursor_at_start));
	priv->cursor_column = 0;

	int first_ch = 0;
	CatStringWo *entry_filter_text = l_extract_filter_text_from_text(a_rev, priv->cursor_pre_end, &priv->cursor_column, &first_ch);
	priv->a_entry_filter_text = cat_string_wo_anchor(entry_filter_text, 0);
	priv->cursor_start = cha_cursor_wo_clone(cursor_at_start, CAT_CLONE_DEPTH_MAIN);
	cha_cursor_wo_set_x_cursor_bytes(priv->cursor_start, first_ch);
	return result;
}



gboolean dra_ac_context_set_next_revision(DraAcContext *context, ChaRevisionWo *a_new_revision) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	ChaCursorWo *cursor_next = cha_revision_wo_get_cursor(a_new_revision);
	ChaCursorOrder order = cha_cursor_wo_compare(priv->cursor_pre_end, cursor_next);
	if (order==CHA_CURSOR_LEFT || order==CHA_CURSOR_SAME) {

		CatStringWo *entry_filter_text = l_extract_filter_text_from_text(a_new_revision, cursor_next, &priv->cursor_column, NULL);
		if (cat_string_wo_beginswith(entry_filter_text, priv->a_entry_filter_text)) {
			cat_unref_ptr(priv->cursor_filter);
			priv->cursor_filter = cha_cursor_wo_clone(cursor_next, CAT_CLONE_DEPTH_NONE);
			priv->cursor_filter = cha_cursor_wo_anchor(priv->cursor_filter, 0);

			if (priv->e_entries==priv->pre_filtered_entries) {
				cat_unref_ptr(priv->e_entries);
				priv->e_entries = cat_array_wo_new();
			} else {
				cat_array_wo_clear(priv->e_entries);
			}

			CatIIterator *iter = cat_array_wo_iterator(priv->pre_filtered_entries);
			while(cat_iiterator_has_next(iter)) {
				DraAcEntry *entry = (DraAcEntry *) cat_iiterator_next(iter);
				CatStringWo *filter_text = dra_ac_entry_get_filter_text(entry);
				if (cat_string_wo_beginswith(filter_text, entry_filter_text)) {
					cat_array_wo_append(priv->e_entries, (GObject *) entry);
				}
			}
			cat_unref_ptr(iter);

			if (cat_array_wo_size(priv->e_entries)==0) {
				return FALSE;
			}

			CatIIterator *iter2 = cat_weak_list_iterator(priv->listeners);
			while(cat_iiterator_has_next(iter2)) {
				DraIAcContextListener *listener = (DraIAcContextListener *) cat_iiterator_next(iter2);
				DRA_IAC_CONTEXT_LISTENER_GET_INTERFACE(listener)->entryListChanged(listener);
			}

			return TRUE;
		}
	}
	return FALSE;
}

ChaCursorWo *dra_ac_context_get_filter_cursor(DraAcContext *context) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	return priv->cursor_filter;
}

ChaCursorWo *dra_ac_context_get_start_cursor(DraAcContext *context) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	return priv->cursor_start;
}
ChaCursorWo *dra_ac_context_get_end_cursor(DraAcContext *context) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	return priv->cursor_pre_end;
}


ChaEditor *dra_ac_context_get_editor(DraAcContext *context) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	return priv->editor;
}

ChaRevisionReader *dra_ac_context_create_revision_reader(DraAcContext *context) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	return cha_revision_reader_new(priv->a_revision, NULL, NULL);
}

void dra_ac_context_get_cursor_location(DraAcContext *context, int *column, long long int *row) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	*column = priv->cursor_column;
	*row = priv->cursor_row;
}

ChaCursorWo *dra_ac_context_create_cursor(DraAcContext *context, int column_x_bytes, long long int row) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	ChaLineLocationWo *ll = cha_revision_wo_calculate_line_location(priv->a_revision, row);
	ChaCursorWo *result = cha_cursor_wo_new_ll_offset(ll, column_x_bytes);
	cat_unref_ptr(ll);
	return result;
}


void dra_ac_context_provider_content_requested(DraAcContext *context) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	cat_lock_lock(priv->lock);
	priv->content_requests_pending++;
	cat_lock_unlock(priv->lock);
}

void dra_ac_context_provider_finished(DraAcContext *context) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	cat_lock_lock(priv->lock);
	priv->content_requests_pending--;
	if (priv->content_requests_pending==0) {
		cat_lock_notify_all(priv->lock);
	}
	cat_lock_unlock(priv->lock);
}

void dra_ac_context_wait_for_providers_to_finish(DraAcContext *context) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	int64_t s = cat_date_current_time()+2200;

	cat_lock_lock(priv->lock);
	while(priv->content_requests_pending>0) {
		int64_t wt = s-cat_date_current_time();
		if (wt<=0) {
			break;
		}
		cat_lock_wait_timed(priv->lock, wt);
	}
	cat_lock_unlock(priv->lock);
}

// TODO insert sorted
// TODO create and test for anchored entries list
void dra_ac_add_entry(DraAcContext *context, DraAcEntry *entry) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	cat_lock_lock(priv->lock);

	CatStringWo *filter_text = dra_ac_entry_get_filter_text(entry);

	if (cat_string_wo_beginswith(filter_text, priv->a_entry_filter_text)) {
		cat_array_wo_append(priv->e_entries, (GObject *) entry);
	}

	cat_lock_unlock(priv->lock);
}


DraAcEntry *dra_ac_context_get_at(DraAcContext *context, int index) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	return (DraAcEntry *) cat_array_wo_get(priv->e_entries, index);
}

int dra_ac_context_count(DraAcContext *context) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	return cat_array_wo_size(priv->e_entries);
}

CatIIterator *dra_ac_context_iterator(DraAcContext *context) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	return cat_array_wo_iterator(priv->e_entries);
}

void dra_ac_context_add_listener(DraAcContext *context, DraIAcContextListener *listener) {
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(context);
	cat_weak_list_append_once(priv->listeners, (GObject *) listener);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraAcContext *instance = DRA_AC_CONTEXT(self);
	DraAcContextPrivate *priv = dra_ac_context_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p e_entries=%o]", iname, self, priv->e_entries);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
