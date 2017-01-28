/*
   File:    mooresourcecontentwo.c
   Project: moose
   Author:  Douwe Vos
   Date:    Nov 6, 2013
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

#include "mooresourcecontentwo.h"
#include "mooicontent.h"
#include "mooiservicescontent.h"
#include "../dialog/moorenamedialog.h"
#include "../mooservice.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooResourceContentWo"
#include <logging/catlog.h>

struct _WoInfo {
	MooResourceContentWo *original;
	MooResourceContentWo *resource_parent;
	gboolean marked;
};

typedef struct _WoInfo WoInfo;


struct _MooResourceContentWoPrivate {
	CatReadableTreeNode *node;
	int version;
	WoInfo *wo_info;
};

static CatS moo_s_resource_content_key = CAT_S_DEF("MooResourceContentWo");

static void l_content_iface_init(MooIContentInterface *iface);
static void l_services_content_iface_init(MooIServicesContentInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooResourceContentWo, moo_resource_content_wo, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(MOO_TYPE_ICONTENT, l_content_iface_init);
		G_IMPLEMENT_INTERFACE(MOO_TYPE_ISERVICES_CONTENT, l_services_content_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void moo_resource_content_wo_class_init(MooResourceContentWoClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(MooResourceContentWoPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_resource_content_wo_init(MooResourceContentWo *instance) {
	MooResourceContentWoPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, MOO_TYPE_RESOURCE_CONTENT_WO, MooResourceContentWoPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	MooResourceContentWo *instance = MOO_RESOURCE_CONTENT_WO(object);
	MooResourceContentWoPrivate *priv = instance->priv;
	cat_unref_ptr(priv->node);
	if (priv->wo_info) {
		cat_unref_ptr(priv->wo_info->original);
		cat_unref_ptr(priv->wo_info->resource_parent);
		cat_free_ptr(priv->wo_info);
	}
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooResourceContentWo *moo_resource_content_wo_new(CatReadableTreeNode *node, MooResourceContentWo *parent_resource) {
	MooResourceContentWo *result = g_object_new(MOO_TYPE_RESOURCE_CONTENT_WO, NULL);
	cat_ref_anounce(result);
	MooResourceContentWoPrivate *priv = result->priv;
	priv->node = cat_ref_ptr(node);
	priv->version = 1;
	priv->wo_info = g_new0(WoInfo, 1);
	priv->wo_info->resource_parent = cat_ref_ptr(parent_resource);
	priv->wo_info->marked = FALSE;
	return result;
}

MooResourceContentWo *moo_resource_content_wo_ensure_editable(MooResourceContentWo *resource_content, MooResourceContentWo *parent_resource) {
	MooResourceContentWoPrivate *priv = MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(resource_content);
	if (priv->wo_info) {
		return cat_ref_ptr(resource_content);
	}

	MooResourceContentWo *result = g_object_new(MOO_TYPE_RESOURCE_CONTENT_WO, NULL);
	cat_ref_anounce(result);

	MooResourceContentWoPrivate *dpriv = MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(result);
	dpriv->node = cat_ref_ptr(priv->node);
	dpriv->version = priv->version;
	dpriv->wo_info = g_new0(WoInfo, 1);
	dpriv->wo_info->original = cat_ref_ptr(resource_content);
	dpriv->wo_info->resource_parent = cat_ref_ptr(parent_resource);
	return result;
}

gboolean moo_resource_content_wo_is_fixed(MooResourceContentWo *resource_content) {
	return MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(resource_content)->wo_info==NULL;
}


MooResourceContentWo *moo_resource_content_wo_anchor(MooResourceContentWo *resource_content, int version) {
	MooResourceContentWoPrivate *priv = MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(resource_content);
	if (priv->wo_info) {
		priv->version = version;
		gboolean was_modified = TRUE;
		if (priv->wo_info->original) {
			MooResourceContentWoPrivate *opriv = MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(priv->wo_info->original);
			was_modified = opriv->node!=priv->node || priv->wo_info->marked;
		}
		if (was_modified) {
			cat_unref_ptr(priv->wo_info->original);
			cat_unref_ptr(priv->wo_info->resource_parent);
			cat_free_ptr(priv->wo_info);
			return resource_content;
		}

		MooResourceContentWo *result = priv->wo_info->original;
		priv->wo_info->original = NULL;
		cat_unref_ptr(resource_content);
		return result;
	}
	return resource_content;
}


#define CHECK_IF_WRITABLE(rval) \
		if (priv->wo_info==NULL) { \
			cat_log_error("Object is read only:%o", resource_content); \
			return rval; \
		} \


void moo_resource_content_wo_mark_change(MooResourceContentWo *resource_content) {
	MooResourceContentWoPrivate *priv = MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(resource_content);
	CHECK_IF_WRITABLE()
	if (!priv->wo_info->marked) {
		priv->wo_info->marked = TRUE;
		if (priv->wo_info->resource_parent) {
			moo_resource_content_wo_mark_change(priv->wo_info->resource_parent);
		}
	}
}

int moo_resource_content_wo_get_version(MooResourceContentWo *resource_content) {
	MooResourceContentWoPrivate *priv = MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(resource_content);
	return priv->version;
}


VipNode *moo_resource_content_wo_get_viper_node(MooResourceContentWo *resource_content) {
	MooResourceContentWoPrivate *priv = MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(resource_content);
	if (priv->node == NULL) {
		return NULL;
	}
	return (VipNode *) cat_tree_node_get_content((CatTreeNode *) priv->node);
}

void moo_resource_content_wo_set_node(MooResourceContentWo *resource_content, CatReadableTreeNode *node) {
	MooResourceContentWoPrivate *priv = MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(resource_content);
	if (node==priv->node) {
		return;
	}
	VipNode *new_vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) node);
	VipNode *cur_vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) priv->node);
	if (new_vip_node==cur_vip_node) {
		return;
	}
	CHECK_IF_WRITABLE()
	cat_ref_swap(priv->node, node);
	moo_resource_content_wo_mark_change(resource_content);
}

CatReadableTreeNode *moo_resource_content_wo_get_node(MooResourceContentWo *resource_content) {
	MooResourceContentWoPrivate *priv = MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(resource_content);
	return priv->node;
}



CatStringWo *moo_resource_content_wo_key() {
	return cat_ref_ptr(CAT_S(moo_s_resource_content_key));
}


/********************* start MooIContent implementation *********************/

static CatStringWo *l_content_get_key(MooIContent *self) {
	return moo_resource_content_wo_key();
}

static MooIContent *l_content_anchor(MooIContent *self, int version) {
	return (MooIContent *) moo_resource_content_wo_anchor((MOO_RESOURCE_CONTENT_WO(self)), version);
}



static void l_content_iface_init(MooIContentInterface *iface) {
	iface->getKey = l_content_get_key;
	iface->anchor = l_content_anchor;
}

/********************* end MooIContent implementation *********************/


/********************* begin MooIServicesContent implementation *********************/

static gboolean l_services_can_refresh(MooIServicesContent *self) {
	return TRUE;
}

static void l_services_refresh(MooIServicesContent *self, struct _MooService *moo_service, struct _MooNodeWo *node) {
	MooResourceContentWoPrivate *priv = MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(self);
	if (priv->node==NULL) {
		return;
	}
	VipService *vip_service = moo_service_get_viper_service(moo_service);
	vip_service_refresh_node(vip_service, priv->node);
}

static gboolean l_services_can_rename(MooIServicesContent *self) {
	MooResourceContentWoPrivate *priv = MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(self);
	if (priv->node==NULL) {
		return FALSE;
	}
	VipNode *vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) priv->node);
	if (vip_node) {
		VipIResource *resource = vip_node_get_content(vip_node);
		return vip_iresource_can_rename(resource);
	}
	return FALSE;
}


static gboolean l_services_rename(MooIServicesContent *self, struct _MooService *moo_service, struct _MooNodeWo *node, LeaFrame *frame) {
	MooResourceContentWoPrivate *priv = MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(self);
	if (priv->node==NULL) {
		return FALSE;
	}

	VipService *vip_service = moo_service_get_viper_service(moo_service);
	VipNode *vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) priv->node);
	CatStringWo *a_initial_name = vip_node_get_name(vip_node);

	MooRenameDialog *rename_dialog = moo_rename_dialog_new(frame);
	gboolean keep_retrying = TRUE;
	while(keep_retrying) {
		int response = moo_rename_dialog_run(rename_dialog, a_initial_name);
		if (response==GTK_RESPONSE_OK) {
			CatStringWo *a_new_name = moo_rename_dialog_get_new_name(rename_dialog);
			if (!cat_string_wo_equal(a_new_name, a_initial_name)) {
				VipRenameNodeRequest *rename_request = vip_service_rename_node(vip_service, priv->node, a_new_name);
				if (vip_rename_node_request_wait_for_result(rename_request)) {
					keep_retrying = FALSE;
				} else {
					moo_rename_dialog_set_message(rename_dialog, cat_string_wo_new_with("Was not able to rename resource"));
				}
				cat_unref_ptr(rename_request);
			} else {
				moo_rename_dialog_set_message(rename_dialog, cat_string_wo_new_with("please enter a new name"));
			}
		} else {
			keep_retrying = FALSE;
		}
	}
	gtk_widget_destroy(GTK_WIDGET(rename_dialog));
	return TRUE;
}


static gboolean l_services_can_delete(MooIServicesContent *self) {
	MooResourceContentWoPrivate *priv = MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(self);
	if (priv->node) {
		VipNode *vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) priv->node);
		if (vip_node) {
			VipIResource *resource = vip_node_get_content(vip_node);
			return vip_iresource_can_delete(resource);
		}
	}
	return FALSE;
}


static gboolean l_services_delete(MooIServicesContent *self, struct _MooService *moo_service, struct _MooNodeWo *node, LeaFrame *frame) {
	MooResourceContentWoPrivate *priv = MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(self);
	gboolean result = FALSE;
	if (priv->node) {
		VipNode *vip_node = (VipNode *) cat_tree_node_get_content((CatTreeNode *) priv->node);
		if (vip_node) {
			VipService *vip_service = moo_service_get_viper_service(moo_service);
			VipDeleteNodeRequest *delete_request = vip_service_delete_node(vip_service, priv->node);
			result = vip_delete_node_request_wait_for_result(delete_request);
		}
	}
	return result;
}


static void l_services_content_iface_init(MooIServicesContentInterface *iface) {
	iface->canRefresh = l_services_can_refresh;
	iface->refresh = l_services_refresh;
	iface->canRename = l_services_can_rename;
	iface->rename = l_services_rename;
	iface->canDelete = l_services_can_delete;
	iface->delete = l_services_delete;
}

/********************* end MooIServicesContent implementation *********************/


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	MooResourceContentWoPrivate *priv = MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p: node=%o, version=%d]", iname, self, priv->node ? cat_tree_node_get_content((CatTreeNode *) priv->node) : NULL, priv->version);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
