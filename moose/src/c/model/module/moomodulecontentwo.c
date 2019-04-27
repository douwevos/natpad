/*
   File:    moomodulecontentwo.c
   Project: moose
   Author:  Douwe Vos
   Date:    May 16, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#include "moomodulecontentwo.h"
#include "../moonodewo.h"
#include "../mooicontent.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooModuleContentWo"
#include <logging/catlog.h>

struct _WoInfo {
	MooModuleContentWo *original;
};

typedef struct _WoInfo WoInfo;

struct _MooModuleContentWoPrivate {
	CatReadableTreeNode *base_node;
	int version;
	gboolean refresh_viper_info;
	int settings_file_index;
	VipNode *settings_vip_node;
	gboolean is_model_writable;
	ShoModel *model;
	WoInfo *wo_info;
};

static CatS moo_s_module_content_key = CAT_S_DEF("MooModuleContentWo");

static void l_content_iface_init(MooIContentInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooModuleContentWo, moo_module_content_wo, G_TYPE_OBJECT, {
		G_ADD_PRIVATE(MooModuleContentWo)
		G_IMPLEMENT_INTERFACE(MOO_TYPE_ICONTENT, l_content_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_module_content_wo_class_init(MooModuleContentWoClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_module_content_wo_init(MooModuleContentWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooModuleContentWo *instance = MOO_MODULE_CONTENT_WO(object);
	MooModuleContentWoPrivate *priv = moo_module_content_wo_get_instance_private(instance);
	cat_unref_ptr(priv->base_node);
	cat_unref_ptr(priv->settings_vip_node);
	cat_unref_ptr(priv->model);
	if (priv->wo_info) {
		cat_unref_ptr(priv->wo_info->original);
		cat_free_ptr(priv->wo_info);
	}
	G_OBJECT_CLASS(moo_module_content_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_module_content_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

MooModuleContentWo *moo_module_content_wo_new(CatReadableTreeNode *base_node) {
	MooModuleContentWo *result = g_object_new(MOO_TYPE_MODULE_CONTENT_WO, NULL);
	cat_ref_anounce(result);
	MooModuleContentWoPrivate *priv = moo_module_content_wo_get_instance_private(result);
	priv->refresh_viper_info = TRUE;
	priv->base_node = cat_ref_ptr(base_node);
	priv->model = NULL;
	priv->is_model_writable = FALSE;
	priv->version = 0;
	priv->wo_info = g_new0(WoInfo, 1);
	return result;
}


MooModuleContentWo *moo_module_content_wo_ensure_editable(MooModuleContentWo *module_content) {
	MooModuleContentWoPrivate *priv = moo_module_content_wo_get_instance_private(module_content);
	if(priv->wo_info==NULL) {
		MooModuleContentWo *copy = moo_module_content_wo_new(priv->base_node);
		MooModuleContentWoPrivate *opriv = moo_module_content_wo_get_instance_private(copy);
		opriv->refresh_viper_info = priv->refresh_viper_info;
		opriv->settings_file_index = priv->settings_file_index;
		opriv->settings_vip_node = cat_ref_ptr(priv->settings_vip_node);
		opriv->model = cat_ref_ptr(priv->model);
		opriv->wo_info = g_new0(WoInfo, 1);
		opriv->wo_info->original = cat_ref_ptr(module_content);
		return copy;
	}
	return cat_ref_ptr(module_content);
}

MooModuleContentWo *moo_module_content_wo_anchor(MooModuleContentWo *module_content, int version) {
	MooModuleContentWoPrivate *priv = moo_module_content_wo_get_instance_private(module_content);
	if(priv->wo_info==NULL) {
		return module_content;
	}

	if (priv->wo_info->original) {
		MooModuleContentWoPrivate *opriv = moo_module_content_wo_get_instance_private(priv->wo_info->original);
		gboolean was_modified = priv->is_model_writable
				|| opriv->refresh_viper_info!=priv->refresh_viper_info
				|| opriv->settings_file_index!=priv->settings_file_index
				|| opriv->settings_vip_node!=priv->settings_vip_node;

		if (!was_modified) {
			MooModuleContentWo *result = priv->wo_info->original;
			priv->wo_info->original = NULL;
			cat_unref_ptr(module_content);
			return result;
		}
	}

	cat_unref_ptr(priv->wo_info->original);
	cat_free_ptr(priv->wo_info);
	priv->version = version;
	return module_content;
}



#define CHECK_IF_WRITABLE(robj, rval) \
		if (priv->wo_info==NULL) { \
			cat_log_error("Object is read only:%o", robj); \
			return rval; \
		} \


gboolean moo_module_content_wo_should_refresh_viper_info(MooModuleContentWo *module_content) {
	MooModuleContentWoPrivate *priv = moo_module_content_wo_get_instance_private(module_content);
	return priv->refresh_viper_info;
}

void moo_module_content_wo_set_refresh_viper_info(MooModuleContentWo *e_module_content, gboolean refreshViperInfo) {
	MooModuleContentWoPrivate *priv = moo_module_content_wo_get_instance_private(e_module_content);
	if (refreshViperInfo == priv->refresh_viper_info) {
		return;
	}
	CHECK_IF_WRITABLE(e_module_content,)
	priv->refresh_viper_info = refreshViperInfo;
}

CatReadableTreeNode *moo_module_content_wo_get_base_node(MooModuleContentWo *module_content) {
	MooModuleContentWoPrivate *priv = moo_module_content_wo_get_instance_private(module_content);
	return priv->base_node;
}

void moo_module_content_wo_set_base_node(MooModuleContentWo *e_module_content, CatReadableTreeNode *base_node) {
	MooModuleContentWoPrivate *priv = moo_module_content_wo_get_instance_private(e_module_content);
	if (base_node == priv->base_node) {
		return;
	}
	CHECK_IF_WRITABLE(e_module_content,)
	cat_ref_swap(priv->base_node, base_node);
	priv->refresh_viper_info = TRUE;
}


//@Override
//String toString() {
//	MooModuleContentWoPrivate *priv = moo_module_content_wo_get_instance_private(module_content);
//	return "MooModuleContentWo[viperBaseNode="+viperBaseNode+", refreshViperInfo="+refreshViperInfo+"]";
//}



static ShoModel *l_load_settings(MooModuleContentWo *module_content, VipNode *settingsVipNode) {
	ShoModel *result = NULL;
	if (settingsVipNode!=NULL) {

		VipIFile *content = (VipIFile *) vip_node_get_content(settingsVipNode);
		if (content!=NULL) {
			CatIInputStream *inputStream = vip_ifile_open_input_stream(content);
			if (inputStream!=NULL) {
				ShoParser *shoParser = sho_parser_new();
				result = sho_parser_parse(shoParser, inputStream);
//					cat_log_debug(""+parse.toString());
//				result.dump(System.out);

				cat_unref_ptr(shoParser);
				cat_unref_ptr(inputStream);
			}
		}
	}
	return result;
}

void moo_module_content_wo_transfer_settings(MooService *moo_service, MooModuleContentWo *e_module_content, MooModuleContentWo *transfer_from) {
	MooModuleContentWoPrivate *priv = moo_module_content_wo_get_instance_private(e_module_content);

	MooModuleContentWoPrivate *priv_from = moo_module_content_wo_get_instance_private(transfer_from);
	ShoModel *model = priv_from->model;
	cat_log_debug("transfer_model=%o, model=%o", model, priv->model);
	if (priv->model == model) {
		return;
	}

	CHECK_IF_WRITABLE(e_module_content,)

	moo_module_content_wo_get_writable_model(e_module_content);

	cat_ref_swap(priv->model, model);
	moo_module_content_wo_settings_save(moo_service, e_module_content);
}

void moo_module_content_wo_settings_save(MooService *moo_service, MooModuleContentWo *module_content) {
	MooModuleContentWoPrivate *priv = moo_module_content_wo_get_instance_private(module_content);
	if (priv->model==NULL) {
		cat_log_error("model is NULL");
	}

	VipService *vip_service = moo_service_get_viper_service(moo_service);
//	vip_service_create_path(vip_service, )

	VipNode *base_vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) priv->base_node);
	VipIFile *file = (VipIFile *) vip_node_get_content(base_vip_node);
	if (VIP_IS_FS_MAP(file)) {
		VipFSMap *fs_map = VIP_FS_MAP(file);
		VipPath *path = vip_fs_map_get_path(fs_map);
		path = vip_path_create_child(path, cat_string_wo_new_with("natpad-project.sht"));
		VipCreatePathRequest *cp_req = vip_service_create_path(vip_service, path);
		VipNodePath *n_path = vip_create_path_request_wait_for_path(cp_req);

		ShoSerializer *serializer = sho_serializer_new(priv->model);
		CatStringWo *e_serialized = sho_serialize_to_char_array(serializer);
		cat_log_debug("saving settings:%o", e_serialized);


		VipWriteFileRequest *w_req = vip_write_file_request_new_simple(vip_service, vip_node_path_get_tail_node(n_path), e_serialized);
		vip_service_post(vip_service, (WorRequest *) w_req);
		vip_write_file_request_wait_to_finish(w_req);
		cat_unref_ptr(e_serialized);
		cat_unref_ptr(serializer);
		cat_unref_ptr(w_req);
	}
}

void moo_module_content_wo_set_settings_info(MooModuleContentWo *e_module_content, int settingsFileIndex, VipNode *settingsVipNode) {
	MooModuleContentWoPrivate *priv = moo_module_content_wo_get_instance_private(e_module_content);
	if ((priv->settings_file_index == settingsFileIndex) && (settingsVipNode==priv->settings_vip_node)) {
		return;
	}
	CHECK_IF_WRITABLE(e_module_content,)

	priv->settings_file_index = settingsFileIndex;
	if (settingsVipNode != priv->settings_vip_node) {
		cat_ref_swap(priv->settings_vip_node, settingsVipNode);
		ShoModel *nw_model = l_load_settings(e_module_content, settingsVipNode);
		cat_ref_swap(priv->model, nw_model);
		cat_unref_ptr(nw_model);
	}
}

ShoModel *moo_module_content_wo_get_model(MooModuleContentWo *module_content) {
	MooModuleContentWoPrivate *priv = moo_module_content_wo_get_instance_private(module_content);
	return priv->model;
}

ShoModel *moo_module_content_wo_get_writable_model(MooModuleContentWo *e_module_content) {
	MooModuleContentWoPrivate *priv = moo_module_content_wo_get_instance_private(e_module_content);
	CHECK_IF_WRITABLE(e_module_content, NULL)
	if (!priv->is_model_writable) {
		ShoModel *writable_model;
		if (priv->model == NULL) {
			writable_model = sho_model_new();
		} else {
			writable_model = sho_model_deep_copy(priv->model);
		}
		cat_ref_swap(priv->model, writable_model);
		cat_unref_ptr(writable_model);
		priv->is_model_writable = TRUE;
	}
	return priv->model;
}

CatStringWo *moo_module_content_wo_key() {
	return CAT_S(moo_s_module_content_key);
}


/********************* start MooIContent implementation *********************/

static CatStringWo *l_content_get_key(MooIContent *self) {
	return moo_module_content_wo_key();
}

static MooIContent *l_content_anchor(MooIContent *self, int version) {
	return (MooIContent *) moo_module_content_wo_anchor((MooModuleContentWo *) self, version);
}

static void l_content_iface_init(MooIContentInterface *iface) {
	iface->getKey = l_content_get_key;
	iface->anchor = l_content_anchor;
}

/********************* end MooIContent implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	MooModuleContentWo *instance = MOO_MODULE_CONTENT_WO(self);
	MooModuleContentWoPrivate *priv = moo_module_content_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p: %s, %s, %s, base-node=%p, sho-model=%o]", iname, self, priv->wo_info ? "editable" : "anchored", priv->refresh_viper_info ? "needs-refresh" : "is-up-to-date", priv->is_model_writable ? "sho-writable" : "sho-readonly", priv->base_node, priv->model);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
