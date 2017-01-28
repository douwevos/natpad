/*
 File:    chaloadfilerequest.c
 Project: chameleon
 Author:  Douwe Vos
 Date:    Feb 26, 2015
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

#include "chaloadfilerequest.h"
#include "chadocumentmanager.h"
#include "../document/chammap.h"
#include "../document/chapagewo.h"
#include "../document/page/chafullpagewo.h"
#include "../document/page/chammappagewo.h"
#include <gio/gio.h>
#include <libguess.h>


#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaLoadFileRequest"
#include <logging/catlog.h>

#define MAX_FILE_LENGTH_FULL_MODE 10*1024*1024

struct _ChaLoadFileRequestPrivate {
	ChaDocument *document;
	GFile *file;
	CatArrayWo *unwritten_pages;
	ChaMMap *map_data;
	char *raw_data;
	long long raw_data_length;
	void *page_start;
	int page_line_count;
	int flush_after;
	gboolean big_file_mode;
	CatArrayWo *lines;
	ChaConvertRequest convert_request;

};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaLoadFileRequest, cha_load_file_request, WOR_TYPE_REQUEST,
        G_ADD_PRIVATE(ChaLoadFileRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init));

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void cha_load_file_request_class_init(ChaLoadFileRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;
}

static void cha_load_file_request_init(ChaLoadFileRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaLoadFileRequest *instance = CHA_LOAD_FILE_REQUEST(object);
	ChaLoadFileRequestPrivate *priv = cha_load_file_request_get_instance_private(instance);
	cat_unref_ptr(priv->document);
	cat_unref_ptr(priv->file);
	cat_unref_ptr(priv->unwritten_pages);
	cat_unref_ptr(priv->lines);
	G_OBJECT_CLASS(cha_load_file_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_load_file_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ChaLoadFileRequest *cha_load_file_request_new(ChaDocument *document, GFile *file) {
	ChaLoadFileRequest *result = g_object_new(CHA_TYPE_LOAD_FILE_REQUEST, NULL);
	cat_ref_anounce(result);
	ChaLoadFileRequestPrivate *priv = cha_load_file_request_get_instance_private(result);
	wor_request_construct((WorRequest *) result);
	priv->document = cat_ref_ptr(document);
	priv->file = cat_ref_ptr(file);
	priv->unwritten_pages = cat_array_wo_new();
	return result;
}


struct l_clear {
	ChaDocument *document;
	goffset file_length;
};

static gboolean l_idle_clear_doc(gpointer user_data) {
	struct l_clear *clr = (struct l_clear *) user_data;
	ChaDocument *document = clr->document;
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);
	cha_document_set_big_file_mode(document, clr->file_length > MAX_FILE_LENGTH_FULL_MODE);
	cha_revision_wo_clear(e_revision);
//	cha_revision_wo_init_disc_page(e_revision, clr->file_length);
	cha_document_anchor_document(document);
	cat_unref_ptr(document);
	cat_free_ptr(user_data);
	return FALSE;
}

struct l_set_pages {
	ChaDocument *document;
	CatArrayWo *disc_pages;
	gboolean is_last;
	ChaMMap *mmap_data;
};

static gboolean l_idle_set_loaded_pages(gpointer user_data) {
	struct l_set_pages *set_pages = (struct l_set_pages *) user_data;
	ChaDocument *document = set_pages->document;
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);
	if (cha_revision_wo_page_count(e_revision)==1) {
		ChaPageWo *page = cha_revision_wo_page_at(e_revision, 0);
		int pg_cnt = cha_page_wo_line_count(page);
		gboolean is_empty = FALSE;
		if (pg_cnt==1) {
			ChaLineWo *line = cha_page_wo_line_at(page, 0);
			if (cha_line_wo_byte_count(line) == 0) {
				is_empty = TRUE;
			}
			cat_unref_ptr(line);
		} else if (pg_cnt == 0) {
			is_empty = TRUE;
		}
		if (is_empty) {
			cha_revision_wo_clear(e_revision);
		}
	}
	CatIIterator *iter = cat_array_wo_iterator(set_pages->disc_pages);
	while (cat_iiterator_has_next(iter)) {
		ChaPageWo *page = (ChaPageWo *) cat_iiterator_next(iter);
		cha_revision_wo_append_page(e_revision, page);
	}

	ChaLoadToken *next_load_token = NULL;
	if (!set_pages->is_last) {
		next_load_token = cha_load_token_new();
	}

	ChaLoadToken *last_load_token = cha_revision_wo_get_load_token(e_revision);
	if (last_load_token) {
		cha_load_token_set_loaded_pages(last_load_token, set_pages->disc_pages, next_load_token);
	} else {
		cat_log_debug("no load token in last revision:revision=%o", e_revision);
	}
	cha_revision_wo_set_load_token(e_revision, next_load_token);
	cat_unref_ptr(next_load_token);

	cha_document_anchor_document_full(document, CHA_DOCUMENT_ANCHOR_MODE_REPLACE_LAST_HISTORY);
	cha_document_set_saved_revision(document, e_revision);
	cat_log_debug("set_pages->is_last=%d, document=%o", set_pages->is_last, document);

	if (set_pages->is_last && set_pages->mmap_data) {
		cha_mmap_call_dontneed(set_pages->mmap_data);
	}

	cat_unref_ptr(iter);
	cat_unref_ptr(document);
	cat_unref_ptr(set_pages->mmap_data);
	cat_unref_ptr(set_pages->disc_pages);
	cat_free_ptr(set_pages);
	return FALSE;
}

static gboolean l_scanned_line_big_file_mode(char *off_line_start, char *off_line_end, ChaLineEnd line_end, void *data) {
	ChaLoadFileRequest *load_file = (ChaLoadFileRequest *) data;
	ChaLoadFileRequestPrivate *priv = cha_load_file_request_get_instance_private(load_file);

	void *start_ptr = cha_mmap_get_data(priv->map_data);
	char *end_ptr = (char*) (start_ptr + cha_mmap_get_length(priv->map_data));
	gboolean is_last_line = FALSE;
	switch(line_end) {
		case CHA_LINE_END_NONE : is_last_line = TRUE; break;
		case CHA_LINE_END_LF :
		case CHA_LINE_END_CR :
			is_last_line = off_line_end + 1 >= end_ptr; break;
		case CHA_LINE_END_LFCR :
		case CHA_LINE_END_CRLF :
			is_last_line = off_line_end + 2 >= end_ptr; break;
	}
	cat_log_debug("is_last_line=%d", is_last_line);

	if (priv->page_line_count == CHA_PAGE_SIZE_PREF || is_last_line) {

		goffset pg_offset = (priv->page_start - start_ptr);
		gsize length = off_line_start - (char *) priv->page_start;

		if (is_last_line) {
			length = off_line_end - (char *) priv->page_start;
			priv->page_line_count++;
		}

		ChaPageWo *new_page = (ChaPageWo *) cha_mmap_page_wo_new(priv->map_data, pg_offset, length, priv->page_line_count);
		cat_array_wo_append(priv->unwritten_pages, (GObject *) new_page);

		priv->page_start = off_line_start;
		priv->page_line_count = 0;

		if (cat_array_wo_size(priv->unwritten_pages) > priv->flush_after || is_last_line) {
			cat_log_info("pg_offset=%ld :: %ld", pg_offset, pg_offset / (1024 * 1024));
			struct l_set_pages *clr = g_new(struct l_set_pages, 1);
			clr->document = cat_ref_ptr(priv->document);
			clr->disc_pages = priv->unwritten_pages;
			clr->is_last = is_last_line;
			clr->mmap_data = cat_ref_ptr(priv->map_data);
			priv->unwritten_pages = cat_array_wo_new();
			g_idle_add((GSourceFunc) l_idle_set_loaded_pages, clr);
			priv->flush_after = 1000;
		}

	}
	priv->page_line_count++;
	return TRUE;
}

static gboolean l_scanned_line_full_mode(char *off_line_start, char *off_line_end, ChaLineEnd line_end, void *data) {
	ChaLoadFileRequest *load_file = (ChaLoadFileRequest *) data;
	ChaLoadFileRequestPrivate *priv = cha_load_file_request_get_instance_private(load_file);


	void *start_ptr = priv->raw_data;
	char *end_ptr = (char*) (start_ptr + priv->raw_data_length);
	gboolean is_last_line = FALSE;
	switch(line_end) {
		case CHA_LINE_END_NONE : is_last_line = TRUE; break;
		case CHA_LINE_END_LF :
		case CHA_LINE_END_CR :
			is_last_line = off_line_end + 1 >= end_ptr; break;
		case CHA_LINE_END_LFCR :
		case CHA_LINE_END_CRLF :
			is_last_line = off_line_end + 2 >= end_ptr; break;
	}

	ChaIConverter *converter = cha_document_get_input_converter(priv->document);
	priv->convert_request.output = NULL;
	priv->convert_request.forward_conversion = TRUE;
	priv->convert_request.text = off_line_start;
	priv->convert_request.text_length = off_line_end-off_line_start;
	cha_iconverter_convert(converter, &(priv->convert_request));
//	CatStringWo *text = cha_charset_converter_convert(converter, off_line_start, off_line_end-off_line_start, NULL, NULL);

	CatStringWo *text = cat_string_wo_anchor(priv->convert_request.output, 0);
	priv->convert_request.output = NULL;

//	CatStringWo *text = (CatStringWo *) cat_string_wo_new_anchored(off_line_start, off_line_end-off_line_start);
	cat_log_trace("line_end=%d text=%s, is_last_line=%d", line_end, text, is_last_line);
	ChaLineWo *line = cha_line_wo_new_anchored(text, line_end);
	cat_array_wo_append(priv->lines, (GObject *) line);

	cat_unref_ptr(line);

	if (cat_array_wo_size(priv->lines) == CHA_PAGE_SIZE_PREF || is_last_line) {
		ChaPageWo *new_page = (ChaPageWo *) cha_full_page_wo_new(priv->lines);
		cat_array_wo_append(priv->unwritten_pages, (GObject *) new_page);
		cat_unref_ptr(new_page);
		priv->lines = cat_array_wo_new_size(CHA_PAGE_SIZE_PREF);

		priv->page_start = off_line_start;

		if (cat_array_wo_size(priv->unwritten_pages) > priv->flush_after || is_last_line) {
			struct l_set_pages *clr = g_new(struct l_set_pages, 1);
			clr->document = cat_ref_ptr(priv->document);
			clr->disc_pages = priv->unwritten_pages;
			clr->is_last = is_last_line;
			clr->mmap_data = cat_ref_ptr(priv->map_data);
			priv->unwritten_pages = cat_array_wo_new();
			g_idle_add((GSourceFunc) l_idle_set_loaded_pages, clr);
			priv->flush_after = 1000;
		}
	}
	return TRUE;
}

static void l_run_request(WorRequest *request) {
	ChaLoadFileRequest *instance = CHA_LOAD_FILE_REQUEST(request);
	ChaLoadFileRequestPrivate *priv = cha_load_file_request_get_instance_private(instance);

	cat_log_info("start loading");

	GFileInfo *info = g_file_query_info(priv->file, G_FILE_ATTRIBUTE_STANDARD_SIZE, G_FILE_QUERY_INFO_NONE, NULL, NULL);
	cat_log_debug("info=%o", info);
	if (info==NULL) {
		return;
	}
	goffset file_length = g_file_info_get_attribute_uint64(info, G_FILE_ATTRIBUTE_STANDARD_SIZE);
	cat_unref_ptr(info);

	priv->raw_data = NULL;
	priv->raw_data_length = file_length;


	priv->big_file_mode = file_length > MAX_FILE_LENGTH_FULL_MODE;
	struct l_clear *clr = g_new(struct l_clear, 1);
	clr->document = cat_ref_ptr(priv->document);
	clr->file_length = file_length;
	g_idle_add((GSourceFunc) l_idle_clear_doc, clr);



	if (!priv->big_file_mode) {
		cat_log_debug("length=%ld",file_length);
		gsize contents_length = 0;
		char *contents = NULL;
		GError *error = NULL;
		if (g_file_load_contents(priv->file, NULL, &contents, &contents_length, NULL, &error)) {
			cat_log_on_debug({
				CatStringWo *txt_snapshot = cat_string_wo_new_data_len(contents, 30);
				cat_log_debug("did read it!! %d contents=%o", contents_length, txt_snapshot);
				cat_unref_ptr(txt_snapshot);
			});
			priv->raw_data = contents;
		} else {
			cat_log_error("error=%s", error->message);
		}
		priv->map_data = NULL;
	} else {
		ChaMMap *mmap_data = cha_mmap_new(priv->file);
		void *ptr_in_data = cha_mmap_get_data(mmap_data);
		priv->map_data = mmap_data;
		priv->raw_data = ptr_in_data;
		priv->page_start = ptr_in_data;
	}

	cat_log_info("loading finished");

	priv->flush_after = 20;
	priv->page_line_count = 0;




	int buflen = priv->raw_data_length;
	if (buflen>8192) {
		buflen = 8192;
	}
	const char *encoding = libguess_determine_encoding(priv->raw_data, buflen, GUESS_REGION_RU);
	cat_log_error("encoding=%s", encoding);

	ChaDocumentManager *document_manager = cha_document_get_document_manager(priv->document);
	ChaIConverter *converter = cha_document_manager_get_converter(document_manager, encoding);

	cha_document_set_input_converter(priv->document, converter);


	if (priv->big_file_mode) {
		cha_scan_lines(priv->raw_data, priv->raw_data + priv->raw_data_length, l_scanned_line_big_file_mode, request);
	} else {
		priv->lines = cat_array_wo_new_size(CHA_PAGE_SIZE_PREF);
		cha_scan_lines(priv->raw_data, priv->raw_data + priv->raw_data_length, l_scanned_line_full_mode, request);
	}

	cat_log_info("scanned lines");

	if (priv->map_data==NULL) {
		cat_free_ptr(priv->raw_data);
	}

}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaLoadFileRequest *instance = CHA_LOAD_FILE_REQUEST(self);
	ChaLoadFileRequestPrivate *priv = cha_load_file_request_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p document=%o]", iname, self, priv->document);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
