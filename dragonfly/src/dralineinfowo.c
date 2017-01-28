/*
   File:    dralineinfo.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Apr 4, 2015
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

#include "dralineinfowo.h"
#include "dralinetagwo.h"
#include "attr/draiattributebuilder.h"
#include <chameleon.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraLineInfoWo"
#include <logging/catlog.h>

struct _DraLineInfoWoPrivate {
	CatArrayWo *ab_list;
	CatArrayWo *line_tags;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraLineInfoWo, dra_line_info_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(DraLineInfoWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);


static void dra_line_info_wo_class_init(DraLineInfoWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_line_info_wo_init(DraLineInfoWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraLineInfoWo *instance = DRA_LINE_INFO_WO(object);
	DraLineInfoWoPrivate *priv = dra_line_info_wo_get_instance_private(instance);
	cat_unref_ptr(priv->ab_list);
	cat_unref_ptr(priv->line_tags);
	G_OBJECT_CLASS(dra_line_info_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_line_info_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraLineInfoWo *dra_line_info_wo_new() {
	DraLineInfoWo *result = g_object_new(DRA_TYPE_LINE_INFO_WO, NULL);
	cat_ref_anounce(result);
	DraLineInfoWoPrivate *priv = dra_line_info_wo_get_instance_private(result);
	priv->ab_list = NULL;
	priv->line_tags = NULL;
	cat_wo_construct((CatWo *) result, TRUE);
	return result;
}


CatArrayWo *dra_line_info_wo_get_ab_list(DraLineInfoWo *line_info) {
	DraLineInfoWoPrivate *priv = dra_line_info_wo_get_instance_private(line_info);
	return priv->ab_list;
}

CatArrayWo *dra_line_info_wo_get_line_tags(DraLineInfoWo *line_info) {
	DraLineInfoWoPrivate *priv = dra_line_info_wo_get_instance_private(line_info);
	return priv->line_tags;
}

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_line_info)) { \
			cat_log_error("Object is read only:%o", e_line_info); \
			return rval; \
		} \


void dra_line_info_wo_set_ab_list(DraLineInfoWo *e_line_info, CatArrayWo *new_ab_list) {
	CHECK_IF_WRITABLE();
	DraLineInfoWoPrivate *priv = dra_line_info_wo_get_instance_private(e_line_info);
	cat_ref_swap(priv->ab_list, new_ab_list);
}

void dra_line_info_wo_append_ab_list(DraLineInfoWo *e_line_info, CatArrayWo *new_ab_list) {
	if (new_ab_list==NULL || cat_array_wo_size(new_ab_list)==0) {
		return;
	}
	CHECK_IF_WRITABLE();
	DraLineInfoWoPrivate *priv = dra_line_info_wo_get_instance_private(e_line_info);
	if (priv->ab_list==NULL) {
		priv->ab_list = cat_array_wo_clone(new_ab_list, CAT_CLONE_DEPTH_MAIN);
		return;
	}
	if (cat_array_wo_is_anchored(priv->ab_list)) {
		CatArrayWo *nl = cat_array_wo_create_editable(priv->ab_list);
		cat_unref_ptr(priv->ab_list);
		priv->ab_list = nl;
	}
	cat_array_wo_append_all(priv->ab_list, new_ab_list);
}


void dra_line_info_wo_set_line_tags(DraLineInfoWo *e_line_info, CatArrayWo *new_line_tags) {
	CHECK_IF_WRITABLE();
	DraLineInfoWoPrivate *priv = dra_line_info_wo_get_instance_private(e_line_info);
	cat_ref_swap(priv->line_tags, new_line_tags);
}

void dra_line_info_wo_append_line_tags(DraLineInfoWo *e_line_info, CatArrayWo *new_line_tags) {
	if (new_line_tags==NULL || cat_array_wo_size(new_line_tags)==0) {
		return;
	}
	CHECK_IF_WRITABLE();
	DraLineInfoWoPrivate *priv = dra_line_info_wo_get_instance_private(e_line_info);
	if (priv->line_tags==NULL) {
		priv->line_tags = cat_array_wo_clone(new_line_tags, CAT_CLONE_DEPTH_MAIN);
		return;
	}
	if (cat_array_wo_is_anchored(priv->line_tags)) {
		CatArrayWo *elt = cat_array_wo_create_editable(priv->line_tags);
		cat_unref_ptr(priv->line_tags);
		priv->line_tags = elt;
	}
	cat_array_wo_append_all(priv->line_tags, new_line_tags);
}


static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(DRA_TYPE_LINE_INFO_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	DraLineInfoWoPrivate *priv = dra_line_info_wo_get_instance_private((DraLineInfoWo *) e_uninitialized);

	if (original) {
		DraLineInfoWoPrivate *rpriv = dra_line_info_wo_get_instance_private((DraLineInfoWo *) original);
		priv->ab_list = cat_ref_ptr(rpriv->ab_list);
		priv->line_tags = cat_ref_ptr(rpriv->line_tags);
	} else {
		priv->ab_list = cat_array_wo_new();
		priv->line_tags = cat_array_wo_new();
	}
	return CAT_WO_CLASS(dra_line_info_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static void l_anchor_children(CatWo *wo, int version) {
	DraLineInfoWoPrivate *priv = dra_line_info_wo_get_instance_private((DraLineInfoWo *) wo);
	priv->ab_list = cat_array_wo_anchor(priv->ab_list, version);
	priv->line_tags = cat_array_wo_anchor(priv->line_tags, version);
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const DraLineInfoWoPrivate *priv_a = dra_line_info_wo_get_instance_private((DraLineInfoWo *) wo_a);
	const DraLineInfoWoPrivate *priv_b = dra_line_info_wo_get_instance_private((DraLineInfoWo *) wo_b);
	return cat_array_wo_equal(priv_a->ab_list, priv_b->ab_list, (GEqualFunc) dra_iattribute_builder_equal)
			&& cat_array_wo_equal(priv_a->line_tags, priv_b->line_tags, (GEqualFunc) dra_line_tag_wo_equal);
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(DRA_TYPE_LINE_INFO_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	DraLineInfoWoPrivate *priv = dra_line_info_wo_get_instance_private(DRA_LINE_INFO_WO(e_uninitialized));
	if (wo_source) {
		DraLineInfoWoPrivate *priv_src = dra_line_info_wo_get_instance_private(DRA_LINE_INFO_WO(wo_source));
		priv->ab_list = cat_array_wo_clone(priv_src->ab_list, CAT_CLONE_DEPTH_NONE);
		priv->line_tags = cat_array_wo_clone(priv_src->line_tags, CAT_CLONE_DEPTH_NONE);
	} else {
		priv->ab_list = cat_array_wo_new();
		priv->line_tags = cat_array_wo_new();
	}

	CatWoClass *wocls = CAT_WO_CLASS(dra_line_info_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}

CAT_WO_BASE_C(DraLineInfoWo,dra_line_info_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraLineInfoWo *instance = DRA_LINE_INFO_WO(self);
	DraLineInfoWoPrivate *priv = dra_line_info_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: %s, ab-list=%o]", iname, self, cat_wo_is_anchored((CatWo *) instance) ? "anchored" : "editable", priv->ab_list);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
