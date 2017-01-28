/*
   File:    draoccurrencesrequest.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Nov 18, 2015
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

#include "draoccurrencesrequest.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraOccurrencesRequest"
#include <logging/catlog.h>

struct _DraOccurrencesRequestPrivate {
	DraEditor *editor;
	ChaRevisionWo *a_revision;
	CatStringWo *occurrence_text;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraOccurrencesRequest, dra_occurrences_request, WOR_TYPE_REQUEST,
		G_ADD_PRIVATE(DraOccurrencesRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void dra_occurrences_request_class_init(DraOccurrencesRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void dra_occurrences_request_init(DraOccurrencesRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraOccurrencesRequest *instance = DRA_OCCURRENCES_REQUEST(object);
	DraOccurrencesRequestPrivate *priv = dra_occurrences_request_get_instance_private(instance);
	cat_unref_ptr(priv->editor);
	cat_unref_ptr(priv->a_revision);
	cat_unref_ptr(priv->occurrence_text);
	G_OBJECT_CLASS(dra_occurrences_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_occurrences_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraOccurrencesRequest *dra_occurrences_request_new(DraEditor *editor, ChaRevisionWo *a_revision) {
	DraOccurrencesRequest *result = g_object_new(DRA_TYPE_OCCURRENCES_REQUEST, NULL);
	cat_ref_anounce(result);
	DraOccurrencesRequestPrivate *priv = dra_occurrences_request_get_instance_private(result);
	wor_request_construct((WorRequest *) result);
	priv->editor = cat_ref_ptr(editor);
	priv->a_revision = cat_ref_ptr(a_revision);
	priv->occurrence_text = NULL;

	return result;
}



static gboolean l_done(gpointer user_data) {
	DraOccurrencesRequest *request = DRA_OCCURRENCES_REQUEST(user_data);
	DraOccurrencesRequestPrivate *priv = dra_occurrences_request_get_instance_private(request);
	ChaDocument *document = cha_editor_get_document((ChaEditor *) priv->editor);
	ChaRevisionWo *a_cur_rev = cha_document_get_current_revision_ref(document);
	if (a_cur_rev == priv->a_revision) {
		dra_editor_set_occurrence_text(priv->editor, priv->occurrence_text);
	}
	cat_unref_ptr(a_cur_rev);
	cat_unref_ptr(request);
	return FALSE;
}

static gboolean l_test_occ_char(DraOccurrencesRequestPrivate *priv, gunichar uch) {
	return g_unichar_isalnum(uch) || uch=='.' || uch=='_';
}

static void l_run_request(WorRequest *request) {
	DraOccurrencesRequestPrivate *priv = dra_occurrences_request_get_instance_private((DraOccurrencesRequest *) request);

	ChaDocument *document = cha_editor_get_document((ChaEditor *) priv->editor);
	ChaRevisionWo *a_most_recent_revision = cha_document_get_current_revision_ref(document);
	int content_version = cha_revision_wo_get_page_list_version(a_most_recent_revision);
	cat_unref_ptr(a_most_recent_revision);
	int initial_content_version = cha_revision_wo_get_page_list_version(priv->a_revision);
	cat_log_debug("last_content_version=%d, content_version==%d", priv->last_content_version, content_version);
	if (initial_content_version != content_version) {
		return;
	}


	ChaCursorWo *a_cursor = cha_revision_wo_get_cursor(priv->a_revision);
	ChaLineLocationWo *a_ll = cha_cursor_wo_get_line_location(a_cursor);
	ChaPageWo *a_page = cha_revision_wo_page_at(priv->a_revision, cha_line_location_wo_get_page_index(a_ll));

	const ChaUtf8Text utf8_text = cha_page_wo_utf8_at(a_page, cha_line_location_wo_get_page_line_index(a_ll), FALSE);

	int txt_length = utf8_text.text_len;
	const CatStringWo *a_text = cat_string_wo_new_data_len(utf8_text.text, txt_length);
	int x_last = cha_cursor_wo_get_x_cursor_bytes(a_cursor);
	int x_end = x_last;
	int x_begin = x_last;
	gunichar uch;
//	cat_log_error("uch=%c", uch);
	while(x_end<txt_length) {
		int new_end = x_end;
		uch = cat_string_wo_unichar_at(a_text, &new_end);
		if (!l_test_occ_char(priv, uch) || uch==-1) {
			break;
		}
		x_end = new_end;
	}

	while(x_begin>0) {
		int new_begin = x_begin;
		uch = cat_string_wo_unichar_before(a_text, &new_begin);
		if (!l_test_occ_char(priv, uch) || uch==-1) {
			break;
		}
		x_begin = new_begin;
	}

	if (x_end>x_begin+1) {
		priv->occurrence_text = cat_string_wo_new_sub(a_text, x_begin, x_end-x_begin);
		cat_log_debug("oc-text=%o", priv->occurrence_text);
	}


	cat_unref_ptr(a_text);
	cha_utf8_text_cleanup(&utf8_text);
	cat_ref_ptr(request);
	g_idle_add(l_done, request);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
