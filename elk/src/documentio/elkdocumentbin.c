/*
   File:    elkdocumentbin.c
   Project: elk
   Author:  Douwe Vos
   Date:    Jul 3, 2015
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

#include "elkdocumentbin.h"
#include "elkasyncsavedas.h"
#include <viper.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkDocumentBin"
#include <logging/catlog.h>

struct _ElkDocumentBinPrivate {
	ChaDocumentManager *document_manager;
	VipService *vip_service;
	VipIResource *vip_resource;
	GFile *tmp_file;
	ChaDocument *document;
	int usage;
	CatStringWo *untitled_name;
	gboolean file_is_read_only;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ElkDocumentBin, elk_document_bin, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ElkDocumentBin)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void elk_document_bin_class_init(ElkDocumentBinClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void elk_document_bin_init(ElkDocumentBin *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ElkDocumentBin *instance = ELK_DOCUMENT_BIN(object);
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(instance);
	cat_unref_ptr(priv->document);
	cat_unref_ptr(priv->document_manager);
	cat_unref_ptr(priv->vip_service);
	cat_unref_ptr(priv->vip_resource);
	G_OBJECT_CLASS(elk_document_bin_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_document_bin_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ElkDocumentBin *elk_document_bin_new(ChaDocumentManager *document_manager, VipService *vip_service, VipIResource *vip_resource) {
	ElkDocumentBin *result = g_object_new(ELK_TYPE_DOCUMENT_BIN, NULL);
	cat_ref_anounce(result);
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(result);
	priv->document_manager = cat_ref_ptr(document_manager);
	priv->vip_resource = cat_ref_ptr(vip_resource);
	priv->vip_service = cat_ref_ptr(vip_service);
	priv->file_is_read_only = TRUE;
	priv->document = NULL;
	priv->tmp_file = NULL;
	priv->usage = 1;
	priv->untitled_name = NULL;
	return result;
}

ElkDocumentBin *elk_document_bin_new_empty(ChaDocumentManager *document_manager, VipService *vip_service, int nr) {
	ElkDocumentBin *result = g_object_new(ELK_TYPE_DOCUMENT_BIN, NULL);
	cat_ref_anounce(result);
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(result);
	priv->document_manager = cat_ref_ptr(document_manager);
	priv->vip_resource = NULL;
	priv->vip_service = cat_ref_ptr(vip_service);
	priv->document = NULL;
	priv->tmp_file = NULL;
	priv->usage = 1;
	priv->file_is_read_only = FALSE;
	priv->untitled_name = cat_string_wo_new();
	cat_string_wo_format(priv->untitled_name, "Untitled %d", nr);
	priv->untitled_name = cat_string_wo_anchor(priv->untitled_name, 0);
	return result;
}

void elk_document_bin_increase_usage(ElkDocumentBin *document_bin) {
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(document_bin);
	priv->usage++;

}

gboolean elk_document_bin_decrease_usage(ElkDocumentBin *document_bin) {
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(document_bin);
	priv->usage--;
	return priv->usage == 0;
}


void elk_document_bin_set_document(ElkDocumentBin *document_bin, ChaDocument *document) {
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(document_bin);
	cat_ref_swap(priv->document, document);
}

ChaDocument *elk_document_bin_get_document(ElkDocumentBin *document_bin) {
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(document_bin);
	return priv->document;
}

ChaDocumentManager *elk_document_bin_get_document_manager(ElkDocumentBin *document_bin) {
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(document_bin);
	return priv->document_manager;
}

gboolean elk_document_bin_update_read_only(ElkDocumentBin *document_bin, gboolean do_udpate) {
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(document_bin);
	if (do_udpate) {
		if (priv->vip_resource) {
			if (VIP_IS_FS_FILE(priv->vip_resource)) {
				GFile *file = NULL;
				VipPath *path = vip_fs_file_get_path((VipFSFile *) priv->vip_resource);
				CatStringWo *t = vip_path_to_string(path);
				file = g_file_new_for_path(cat_string_wo_getchars(t));
				cat_unref_ptr(t);

				GError *error = NULL;
				GFileInfo *info = g_file_query_info(file, G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE, G_FILE_QUERY_INFO_NONE, NULL, &error);
				if (info) {
					priv->file_is_read_only = !g_file_info_get_attribute_boolean(info, G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE);
					cat_unref_ptr(info);
				} else {
					priv->file_is_read_only = FALSE;
				}
			}
		}
	}
	return priv->file_is_read_only;
}


static void l_load(ElkDocumentBin *document_bin, GError **error) {
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(document_bin);
	if (priv->vip_resource) {
		if (VIP_IS_FS_FILE(priv->vip_resource)) {
			GFile *file = NULL;
			VipPath *path = vip_fs_file_get_path((VipFSFile *) priv->vip_resource);
			CatStringWo *t = vip_path_to_string(path);
			file = g_file_new_for_path(cat_string_wo_getchars(t));

			GFileInfo *info = g_file_query_info(file, G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE, G_FILE_QUERY_INFO_NONE, NULL, error);
			if (*error==NULL) {
				priv->file_is_read_only = !g_file_info_get_attribute_boolean(info, G_FILE_ATTRIBUTE_ACCESS_CAN_WRITE);
				cha_document_manager_load(priv->document_manager, priv->document, file);
			}
			cat_unref_ptr(info);

			cat_unref_ptr(t);
		} else if (VIP_IS_IFILE(priv->vip_resource)){
			cat_unref_ptr(priv->tmp_file);
			CatStringWo *resource_name = vip_iresource_get_name((VipIResource *) priv->vip_resource);
			GFile *tmp_file = NULL;
			GFileIOStream *out_io_stream = NULL;
			int last_dot_idx = cat_string_wo_last_index_of(resource_name, '.');
			if (last_dot_idx>=0) {
				CatStringWo *tmp_name = cat_string_wo_new_sub(resource_name, 0, last_dot_idx);
				cat_string_wo_append_chars_len(tmp_name, "_XXXXXX",7);
				last_dot_idx++;
				cat_string_wo_append_chars_len(tmp_name, cat_string_wo_getchars(resource_name)+last_dot_idx, cat_string_wo_length(resource_name)-last_dot_idx);
				tmp_file = g_file_new_tmp(cat_string_wo_getchars(tmp_name), &out_io_stream, error);
				cat_unref_ptr(tmp_name);
			} else {
				tmp_file = g_file_new_tmp("natpad_XXXXXX", &out_io_stream, error);
			}
			if (*error!=NULL) {
				GOutputStream *out_stream = g_io_stream_get_output_stream((GIOStream *) out_io_stream);

				CatIInputStream *in_stream = vip_ifile_open_input_stream(priv->vip_resource);
				void *buffer = g_slice_alloc(65536);

				while(TRUE) {
					int read_cnt = cat_iinput_stream_read_length(in_stream, buffer, 65536);
					cat_log_error("read_cnt=%d", read_cnt);
					if (read_cnt>0) {
						g_output_stream_write(out_stream, buffer, read_cnt, NULL, NULL);
					} else {
						break;
					}
				}
				g_output_stream_flush(out_stream, NULL, NULL);
				g_output_stream_close(out_stream, NULL, NULL);
				g_io_stream_close((GIOStream *) out_io_stream, NULL, NULL);

				g_slice_free1(65536, buffer);

				cha_document_manager_load(priv->document_manager, priv->document, tmp_file);
			}
			priv->tmp_file = tmp_file;
			priv->file_is_read_only = TRUE;
		}
	}
}

ChaDocument *elk_document_bin_get_or_create_document(ElkDocumentBin *document_bin) {
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(document_bin);
	if (priv->document==NULL) {
		priv->document = cha_document_new(priv->document_manager);
//		cha_document_get_editable_revision(priv->document);
//		cha_document_anchor_document(priv->document);
		GError *error=NULL;
		l_load(document_bin, &error);
		if (error) {
			cat_log_error("message=%s", error->message);
		}
	}
	return priv->document;
}


void elk_document_bin_revert(ElkDocumentBin *document_bin) {
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(document_bin);
	if (priv->document) {
		GError *error=NULL;
		l_load(document_bin, &error);
		if (error) {
			cat_log_error("message=%s", error->message);
		}
	}
}

void elk_document_bin_store(ElkDocumentBin *document_bin, gboolean create_backup, ChaIOAsync *async) {
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(document_bin);
	if (priv->document) {
		if (priv->vip_resource) {
			if (VIP_IS_FS_FILE(priv->vip_resource)) {
				GFile *file = NULL;
				VipPath *path = vip_fs_file_get_path((VipFSFile *) priv->vip_resource);
				CatStringWo *t = vip_path_to_string(path);
				file = g_file_new_for_path(cat_string_wo_getchars(t));
				cha_document_manager_write(priv->document_manager, priv->document, file, NULL, async, create_backup);
				cat_unref_ptr(t);
			}
		}
	}
}

void elk_document_bin_store_as(ElkDocumentBin *document_bin, gboolean create_backup, CatStringWo *path, ChaIConverter *converter, ChaIOAsync *async) {
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(document_bin);
	if (priv->document) {
		GFile *file = g_file_new_for_path(cat_string_wo_getchars(path));
		ChaIOAsync *async_m = (ChaIOAsync *) elk_async_saved_as_new(document_bin, path, async);
		cha_document_manager_write(priv->document_manager, priv->document, file, converter, async_m, create_backup);
		cat_unref_ptr(async_m);
	}
}

void elk_document_bin_set_path(ElkDocumentBin *document_bin, CatStringWo *path) {
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(document_bin);
	VipPath *full_path = vip_path_new(path);
	VipCreatePathRequest *cp_req = vip_service_create_path(priv->vip_service, full_path);
	VipNodePath *np = vip_create_path_request_wait_for_path(cp_req);
	VipNode *node = vip_node_path_get_tail(np);
	VipIResource *resource = vip_node_get_content(node);
	if (VIP_IS_IFILE(resource)) {
		cat_ref_swap(priv->vip_resource, resource);
	} else {
		cat_log_warn("NOT A FILE:%o", resource);
	}
	cat_unref_ptr(cp_req);
	cat_unref_ptr(full_path);
}

gboolean elk_document_bin_has_file(const ElkDocumentBin *document_bin) {
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private((ElkDocumentBin *) document_bin);
	cat_log_debug("priv->vip_file=%o", priv->vip_file);
	return priv->vip_resource!=NULL;
}

CatStringWo *elk_document_bin_get_name(ElkDocumentBin *document_bin) {
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(document_bin);
	CatStringWo *result = NULL;
	if (priv->vip_resource) {
		result = vip_iresource_get_name((VipIResource *) priv->vip_resource);
	} else {
		result = priv->untitled_name;
	}
	return result;
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ElkDocumentBin *instance = ELK_DOCUMENT_BIN(self);
	ElkDocumentBinPrivate *priv = elk_document_bin_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p document=%p]", iname, self, priv->document);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

