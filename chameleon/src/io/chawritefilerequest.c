/*
   File:    chawritefilerequest.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Aug 28, 2015
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

#include "chawritefilerequest.h"
#include "../document/charevisionwo.h"
#include "../document/chapagewo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaWriteFileRequest"
#include <logging/catlog.h>

struct _ChaWriteFileRequestPrivate {
	ChaDocument *document;
	GFile *file;
	ChaIConverter *output_converter;
	ChaIOAsync *async;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaWriteFileRequest, cha_write_file_request, WOR_TYPE_REQUEST,
		G_ADD_PRIVATE(ChaWriteFileRequest)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_run_request(WorRequest *request);

static void cha_write_file_request_class_init(ChaWriteFileRequestClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	WorRequestClass *wor_class = WOR_REQUEST_CLASS(clazz);
	wor_class->runRequest = l_run_request;

}

static void cha_write_file_request_init(ChaWriteFileRequest *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaWriteFileRequest *instance = CHA_WRITE_FILE_REQUEST(object);
	ChaWriteFileRequestPrivate *priv = cha_write_file_request_get_instance_private(instance);
	cat_unref_ptr(priv->document);
	cat_unref_ptr(priv->output_converter);
	cat_unref_ptr(priv->file);
	cat_unref_ptr(priv->async);
	G_OBJECT_CLASS(cha_write_file_request_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_write_file_request_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaWriteFileRequest *cha_write_file_request_new(ChaDocument *document, GFile *file, ChaIConverter *output_converter, ChaIOAsync *async) {
	ChaWriteFileRequest *result = g_object_new(CHA_TYPE_WRITE_FILE_REQUEST, NULL);
	cat_ref_anounce(result);
	ChaWriteFileRequestPrivate *priv = cha_write_file_request_get_instance_private(result);
	wor_request_construct((WorRequest *) result);
	priv->document = cat_ref_ptr(document);
	priv->file = cat_ref_ptr(file);
	priv->output_converter = cat_ref_ptr(output_converter);
	priv->async = cat_ref_ptr(async);
	return result;
}

static void l_run_request(WorRequest *request) {
	ChaWriteFileRequest *instance = CHA_WRITE_FILE_REQUEST(request);
	ChaWriteFileRequestPrivate *priv = cha_write_file_request_get_instance_private(instance);
	ChaRevisionWo *rev = cha_document_get_current_revision_ref(priv->document);
	GFileOutputStream *out_stream = g_file_replace(priv->file, NULL, TRUE, 0, NULL, NULL);
	ChaWriteReq write_req;
	write_req.error = NULL;
	write_req.out_stream = (GOutputStream *) out_stream;
	write_req.needs_conversion = FALSE;
	write_req.charset_converter = priv->output_converter;
	ChaIConverter *input_converter = cha_document_get_input_converter(priv->document);

	if (priv->output_converter!=input_converter) {
		write_req.needs_conversion = TRUE;
		if (priv->output_converter==NULL) {
			write_req.charset_converter = input_converter;
		}
	}

	write_req.force_line_end = cha_document_get_line_end_user(priv->document);

	if (out_stream) {
		int page_count = cha_revision_wo_page_count(rev);
		int page_idx;
		for(page_idx=0; page_idx<page_count; page_idx++) {
			ChaPageWo *a_page = cha_revision_wo_page_at(rev, page_idx);
			if (!cha_page_wo_write_to_stream(a_page, &write_req)) {
				break;
			}
		}
		g_output_stream_close((GOutputStream *) out_stream, NULL, NULL);
		cha_io_async_finished(priv->async, TRUE, write_req.error);
		cha_document_set_saved_revision(priv->document, rev);
	}
	cat_unref_ptr(rev);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaWriteFileRequest *instance = CHA_WRITE_FILE_REQUEST(self);
	ChaWriteFileRequestPrivate *priv = cha_write_file_request_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p document=%p]", iname, self, priv->document);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
