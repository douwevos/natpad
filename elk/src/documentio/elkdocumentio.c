/*
   File:    elkdocumentio.c
   Project: elk
   Author:  Douwe Vos
   Date:    Mar 10, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#include "elkdocumentio.h"
#include <caterpillar.h>
#include <viper.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkDocumentIO"
#include <logging/catlog.h>

struct _ElkDocumentIOPrivate {
	VipService *vip_service;
	ChaDocumentManager *document_manager;

	/* TODO: map is only filled but never cleaned */
	CatHashMapWo *e_document_map;

	CatAtomicInteger *sequence;
};

G_DEFINE_TYPE_WITH_PRIVATE(ElkDocumentIO, elk_document_io, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void elk_document_io_class_init(ElkDocumentIOClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void elk_document_io_init(ElkDocumentIO *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkDocumentIO *instance = ELK_DOCUMENT_IO(object);
	ElkDocumentIOPrivate *priv = elk_document_io_get_instance_private(instance);
	cat_unref_ptr(priv->vip_service);
	cat_unref_ptr(priv->e_document_map);
	cat_unref_ptr(priv->document_manager);
	cat_unref_ptr(priv->sequence);
	G_OBJECT_CLASS(elk_document_io_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_document_io_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ElkDocumentIO *elk_document_io_new(VipService *vip_service, ChaDocumentManager *document_manager) {
	ElkDocumentIO *result = g_object_new(ELK_TYPE_DOCUMENT_IO, NULL);
	cat_ref_anounce(result);
	ElkDocumentIOPrivate *priv = elk_document_io_get_instance_private(result);
	priv->vip_service = cat_ref_ptr(vip_service);
	priv->document_manager = cat_ref_ptr(document_manager);
	priv->e_document_map = cat_hash_map_wo_new((GHashFunc) vip_path_hash, (GEqualFunc) vip_path_equal);
	priv->sequence = cat_atomic_integer_new();
	return result;
}


ElkDocumentBin *elk_document_io_open_document_for_file(ElkDocumentIO *document_io, VipIFile *vip_file) {
	cat_log_debug("document_io=%p", document_io);
	ElkDocumentIOPrivate *priv = elk_document_io_get_instance_private(document_io);
	cat_log_debug("vip_file=%o", vip_file);
	VipPath *file_path = vip_iresource_path((VipIResource *) vip_file);
	cat_log_debug("file_path=%o", file_path);
	ElkDocumentBin *result = (ElkDocumentBin *) cat_hash_map_wo_get(priv->e_document_map, file_path);
	// TODO unref file_path ??
	if (result==NULL) {
		result = elk_document_bin_new(priv->document_manager, priv->vip_service, vip_file);
		cat_hash_map_wo_put(priv->e_document_map, (GObject *) file_path, (GObject *) result);
		cat_unref(result);
	} else {
		elk_document_bin_increase_usage(result);
	}
	return result;
}

ElkDocumentBin *elk_document_io_open_new_document(ElkDocumentIO *document_io) {
	ElkDocumentIOPrivate *priv = elk_document_io_get_instance_private(document_io);
	CatStringWo *name = cat_string_wo_new();
	int nr = cat_atomic_integer_increment(priv->sequence);
	cat_string_wo_format(name, "<new-%d>", nr);
	VipPath *file_path = vip_path_new(name);
	cat_log_debug("file_path=%o", file_path);
	ElkDocumentBin *result = (ElkDocumentBin *) cat_hash_map_wo_get(priv->e_document_map, file_path);
	// TODO unref file_path ??
	if (result==NULL) {
		result = elk_document_bin_new_empty(priv->document_manager, priv->vip_service, nr);
		cat_hash_map_wo_put(priv->e_document_map, (GObject *) file_path, (GObject *) result);
		cat_unref(result);
	} else {
		elk_document_bin_increase_usage(result);
	}
	cat_unref_ptr(file_path);
	return result;
}
