/*
   File:    chadocumentmanager.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 25, 2015
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

#include "chadocumentmanager.h"
#include "chaloadfilerequest.h"
#include "chawritefilerequest.h"
#include "../charset/chacharsetconverterfactory.h"
#include <worm.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaDocumentManager"
#include <logging/catlog.h>

struct _ChaDocumentManagerPrivate {
	WorService *wor_service;
	ChaCharsetConverterFactory *converter_factory;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaDocumentManager, cha_document_manager, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaDocumentManager)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_document_manager_class_init(ChaDocumentManagerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_document_manager_init(ChaDocumentManager *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaDocumentManager *instance = CHA_DOCUMENT_MANAGER(object);
	ChaDocumentManagerPrivate *priv = cha_document_manager_get_instance_private(instance);
	if (priv->wor_service) {
		wor_service_request_shutdown(priv->wor_service);
		cat_unref_ptr(priv->wor_service);
	}
	cat_unref_ptr(priv->converter_factory);
	G_OBJECT_CLASS(cha_document_manager_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_document_manager_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaDocumentManager *cha_document_manager_new() {
	ChaDocumentManager *result = g_object_new(CHA_TYPE_DOCUMENT_MANAGER, NULL);
	cat_ref_anounce(result);
	ChaDocumentManagerPrivate *priv = cha_document_manager_get_instance_private(result);
	priv->wor_service = wor_service_new_ext(6, 256);
	priv->converter_factory = cha_charset_converter_factory_new();
	return result;
}

ChaIConverter *cha_document_manager_get_converter(ChaDocumentManager *document_manager, const char *charset_name) {
	ChaDocumentManagerPrivate *priv = cha_document_manager_get_instance_private(document_manager);
	return cha_charset_converter_factory_get(priv->converter_factory, charset_name);
}


ChaCharsetConverterFactory *cha_document_manager_get_converter_factory(ChaDocumentManager *document_manager) {
	ChaDocumentManagerPrivate *priv = cha_document_manager_get_instance_private(document_manager);
	return priv->converter_factory;
}


void cha_document_manager_load(ChaDocumentManager *document_manager, ChaDocument *document, GFile *file) {
	ChaDocumentManagerPrivate *priv = cha_document_manager_get_instance_private(document_manager);
	ChaLoadFileRequest *load_file_req = cha_load_file_request_new(document, file);
	wor_service_post_request(priv->wor_service, (WorRequest *) load_file_req);
	cat_unref_ptr(load_file_req);
}

void cha_document_manager_write(ChaDocumentManager *document_manager, ChaDocument *document, GFile *file, ChaIConverter *output_converter, ChaIOAsync *async, gboolean create_backup) {
	ChaDocumentManagerPrivate *priv = cha_document_manager_get_instance_private(document_manager);
	ChaWriteFileRequest *write_file_req = cha_write_file_request_new(document, file, output_converter, async, create_backup);
	wor_service_post_request(priv->wor_service, (WorRequest *) write_file_req);
	cat_unref_ptr(write_file_req);
}


void cha_document_manager_post(ChaDocumentManager *document_manager, WorRequest *request) {
	ChaDocumentManagerPrivate *priv = cha_document_manager_get_instance_private(document_manager);
	wor_service_post_request(priv->wor_service, request);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaDocumentManager *instance = CHA_DOCUMENT_MANAGER(self);
	ChaDocumentManagerPrivate *priv = cha_document_manager_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p, high_prio_wor_service=%p]", iname, self, priv->wor_service);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
