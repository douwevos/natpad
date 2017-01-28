/*
   File:    catwo.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Oct 6, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#include "catwoprivate.h"
#include "../catistringable.h"
#include "../woo/catstringwo.h"

#include "../logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "CatWo"
#include "../logging/catlog.h"

struct _CatWoPrivate {
	struct CatWoInfo *wo_info;
	int version;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(CatWo, cat_wo, G_TYPE_OBJECT,
		G_ADD_PRIVATE(CatWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_wo_anchor(CatWo *wo, int version);
static CatWo *l_wo_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_release_edit_info(CatWo *wo, struct CatWoInfo *info);
static struct CatWoInfo *l_create_edit_info(CatWo *e_wo, const CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version) {}

static void cat_wo_class_init(CatWoClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	clazz->anchor = l_wo_anchor;
	clazz->constructEditable = l_wo_construct_editable;
	clazz->releaseEditInfo = l_release_edit_info;
	clazz->createEditInfo = l_create_edit_info;
	clazz->anchorChildren = l_anchor_children;
}

static void cat_wo_init(CatWo *instance) {
}

static void l_dispose(GObject *object) {
	CatWo *instance = CAT_WO(object);
	CatWoPrivate *priv = cat_wo_get_instance_private(instance);
	if (priv->wo_info!=NULL) {
		CAT_WO_GET_CLASS(instance)->releaseEditInfo(instance, priv->wo_info);
	}
	G_OBJECT_CLASS(cat_wo_parent_class)->dispose(object);
}

static void l_finalize(GObject *object) {
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cat_wo_parent_class)->finalize(object);
}


void cat_wo_construct(CatWo *wo, gboolean editable) {
	CatWoPrivate *priv = cat_wo_get_instance_private(wo);
	priv->version = 0;
	if (editable) {
		CAT_WO_GET_CLASS(wo)->constructEditable(wo, NULL, NULL);
	} else {
		priv->wo_info = NULL;
	}
}

struct CatWoInfo *cat_wo_get_info(CatWo *wo) {
	CatWoPrivate *priv = cat_wo_get_instance_private(wo);
	return priv->wo_info;
}


CatWo *cat_wo_create_editable(CatWo *wo) {
	if (wo==NULL) {
		return NULL;
	}
	if (!cat_wo_is_anchored(wo)) {
		return cat_ref_ptr(wo);
	}
	CatWoClass *wo_class = CAT_WO_GET_CLASS(wo);
	return wo_class->constructEditable(NULL, wo, NULL);
}

CatWo *cat_wo_anchor(CatWo *wo, int version) {
	if (wo==NULL) {
		return NULL;
	}
	CatWoClass *wo_class = CAT_WO_GET_CLASS(wo);
	return wo_class->anchor(wo, version);
}

CatWo *cat_wo_get_original(CatWo *wo) {
	CatWoPrivate *priv = cat_wo_get_instance_private(wo);
	if (priv->wo_info) {
		return priv->wo_info->original;
	}
	return NULL;
}

int cat_wo_get_version(const CatWo *wo) {
	const CatWoPrivate *priv = cat_wo_get_instance_private((CatWo *) wo);
	return priv->version;
}

void cat_wo_set_version(CatWo *e_wo, int version) {
	if (cat_wo_is_anchored(e_wo)) {
		cat_log_warn("object is read only:%o", e_wo);
		return;
	}
	CatWoPrivate *priv = cat_wo_get_instance_private(e_wo);
	priv->version = version;
}


gboolean cat_wo_is_anchored(const CatWo *wo) {
	const CatWoPrivate *priv = cat_wo_get_instance_private((CatWo *) wo);
	return priv->wo_info==NULL;
}

gboolean cat_wo_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	}
	if ((wo_a==NULL) || (wo_b==NULL)) {
		return FALSE;
	}

	CatWoClass *wo_class = CAT_WO_GET_CLASS(wo_a);
	return wo_class->equal(wo_a, wo_b);
}

//CatStringWo *cat_string_wo_clone(const CatStringWo *source, CatCloneDepth clone_depth) {
//	if (source == NULL) {
//		return NULL;
//	} else if ((clone_depth==CAT_CLONE_DEPTH_NONE || clone_depth==CAT_CLONE_DEPTH_AS_ANCHORED) && cat_wo_is_anchored((CatWo *) source)) {
//		return cat_ref_ptr((CatStringWo *) source);
//	}
//	const CatStringWoPrivate *spriv = cat_string_wo_get_instance_private((CatStringWo *) source);
//
//	CatStringWo *result = cat_string_wo_new();
//	CatStringWoPrivate *priv = cat_string_wo_get_instance_private(result);
//	if (spriv->size>0) {
//		if (spriv->data_size<0) {
//			priv->data_size = spriv->size+10;
//		}  else {
//			priv->data_size = spriv->data_size;
//		}
//		priv->size = spriv->size;
//		priv->data = g_malloc(priv->data_size+1);
//		memcpy(priv->data, spriv->data, priv->size);
//		priv->data[priv->size] = 0;
//	}
//	if (clone_depth==CAT_CLONE_DEPTH_AS_ANCHORED) {
//		result = cat_string_wo_anchor(result, cat_wo_get_version((CatWo *) source)+1);
//	}
//	return result;
//}
//


CatWo *cat_wo_clone(const CatWo *wo_source, CatCloneDepth clone_depth) {
	if (wo_source==NULL) {
		return NULL;
	} else if ((clone_depth==CAT_CLONE_DEPTH_NONE || clone_depth==CAT_CLONE_DEPTH_AS_ANCHORED) && cat_wo_is_anchored((CatWo *) wo_source)) {
		return cat_ref_ptr(wo_source);
	}

	CatWo *result = (CatWo *) g_object_new(G_OBJECT(wo_source)->g_type_instance.g_class->g_type, NULL);
	CatWoPrivate *priv = cat_wo_get_instance_private(result);
	priv->version = cat_wo_get_version(wo_source);
	priv->wo_info = NULL;
	CatWoClass *wo_class = CAT_WO_GET_CLASS(wo_source);
	if (clone_depth==CAT_CLONE_DEPTH_FULL || clone_depth==CAT_CLONE_DEPTH_MAIN) {
		priv->wo_info = wo_class->createEditInfo(result, wo_source, NULL);
	}
	wo_class->cloneContent(result, wo_source);
	return result;
}


static CatWo *l_wo_anchor(CatWo *wo, int version) {
	cat_log_debug("anchoring:%o", wo);
	CatWoPrivate *priv = cat_wo_get_instance_private(wo);
	if (priv->wo_info == NULL) {
		return wo;
	}

	CatWoClass *wo_class = CAT_WO_GET_CLASS(wo);

	/* anchor all sub elements */
	wo_class->anchorChildren(wo,version);

	gboolean was_modified = TRUE;
//	if (wo_class->checkForModifications) {
//		was_modified = wo_class->checkForModifications(wo);
//	} else {
		was_modified = !wo_class->equal(priv->wo_info->original, wo);
//	}


	if (was_modified) {
		/* return edited version */
		wo_class->releaseEditInfo(wo, priv->wo_info);
		priv->version = version;
		cat_log_debug("Returning edited as anchored:%o", wo);
		return wo;
	}
	/* return original, release edited version */
	CatWo *result = priv->wo_info->original;
	priv->wo_info->original = NULL;
	cat_unref_ptr(wo);
	cat_log_debug("Returning original as anchored:%o", result);
	return result;
}

static CatWo *l_wo_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (info==NULL) {
		info = g_malloc0(sizeof(struct CatWoInfo));
	}
	CatWoPrivate *priv = cat_wo_get_instance_private(e_uninitialized);
	priv->wo_info = info;
	info->original = cat_ref_ptr(original);
	return e_uninitialized;
}


static void l_release_edit_info(CatWo *wo, struct CatWoInfo *info) {
	CatWoPrivate *priv = cat_wo_get_instance_private(wo);
	cat_unref_ptr(info->original);
	priv->wo_info = NULL;
	g_free(info);
}

static struct CatWoInfo *l_create_edit_info(CatWo *e_wo, const CatWo *original, struct CatWoInfo *info) {
	if (info==NULL) {
		info = g_malloc0(sizeof(struct CatWoInfo));
	}
	CatWoPrivate *priv = cat_wo_get_instance_private(e_wo);
	priv->wo_info = info;
	info->original = cat_ref_ptr(original);
	return info;
}




/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	CatWo *instance = CAT_WO(self);
//	CatWoPrivate *priv = cat_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/







