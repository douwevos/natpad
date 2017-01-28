/*
   File:    chaprefstemplatewo.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Oct 25, 2014
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

#include "chaprefstemplatewo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaPrefsTemplateWo"
#include <logging/catlog.h>

struct _ChaPrefsTemplateWoPrivate {
	CatStringWo *name;
	CatStringWo *description;
	CatStringWo *template;
};


static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaPrefsTemplateWo, cha_prefs_template_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(ChaPrefsTemplateWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);


static void cha_prefs_template_wo_class_init(ChaPrefsTemplateWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->equal = l_equal;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_prefs_template_wo_init(ChaPrefsTemplateWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaPrefsTemplateWo *instance = CHA_PREFS_TEMPLATE_WO(object);
	ChaPrefsTemplateWoPrivate *priv = cha_prefs_template_wo_get_instance_private(instance);

	cat_unref_ptr(priv->name);
	cat_unref_ptr(priv->description);
	cat_unref_ptr(priv->template);
	G_OBJECT_CLASS(cha_prefs_template_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_prefs_template_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaPrefsTemplateWo *cha_prefs_template_wo_new() {
	ChaPrefsTemplateWo *result = g_object_new(CHA_TYPE_PREFS_TEMPLATE_WO, NULL);
	cat_ref_anounce(result);
	cat_wo_construct((CatWo *) result, TRUE);
	return result;
}


CatStringWo *cha_prefs_template_wo_get_name(const ChaPrefsTemplateWo *template) {
	const ChaPrefsTemplateWoPrivate *priv = cha_prefs_template_wo_get_instance_private((ChaPrefsTemplateWo *) template);
	return priv->name;
}

CatStringWo *cha_prefs_template_wo_get_description(const ChaPrefsTemplateWo *template) {
	const ChaPrefsTemplateWoPrivate *priv = cha_prefs_template_wo_get_instance_private((ChaPrefsTemplateWo *) template);
	return priv->description;
}

CatStringWo *cha_prefs_template_wo_get_template(const ChaPrefsTemplateWo *template) {
	const ChaPrefsTemplateWoPrivate *priv = cha_prefs_template_wo_get_instance_private((ChaPrefsTemplateWo *) template);
	return priv->template;
}


#define CHECK_IF_WRITABLE(rval) \
		if (priv->wo_info==NULL) { \
			cat_log_error("Object is read only:%o", e_template); \
			return rval; \
		} \


void cha_prefs_template_wo_set_name(ChaPrefsTemplateWo *e_template, CatStringWo *new_name) {
	ChaPrefsTemplateWoPrivate *priv = cha_prefs_template_wo_get_instance_private(e_template);
	cat_ref_swap(priv->name, new_name);
}

void cha_prefs_template_wo_set_description(ChaPrefsTemplateWo *e_template, CatStringWo *new_description) {
	ChaPrefsTemplateWoPrivate *priv = cha_prefs_template_wo_get_instance_private(e_template);
	cat_ref_swap(priv->description, new_description);
}

void cha_prefs_template_wo_set_template(ChaPrefsTemplateWo *e_template, CatStringWo *new_template) {
	ChaPrefsTemplateWoPrivate *priv = cha_prefs_template_wo_get_instance_private(e_template);
	cat_ref_swap(priv->template, new_template);
}



static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_PREFS_TEMPLATE_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaPrefsTemplateWoPrivate *priv = cha_prefs_template_wo_get_instance_private((ChaPrefsTemplateWo *) e_uninitialized);
	if (original) {
		ChaPrefsTemplateWoPrivate *rpriv = cha_prefs_template_wo_get_instance_private((ChaPrefsTemplateWo *) original);
		priv->name = cat_ref_ptr(rpriv->name);
		priv->description = cat_ref_ptr(rpriv->description);
		priv->template = cat_ref_ptr(rpriv->template);
	} else {
		priv->name = cat_string_wo_new();
		priv->description = cat_string_wo_new();
		priv->template = cat_string_wo_new();
	}
	return CAT_WO_CLASS(cha_prefs_template_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}

static void l_anchor_children(CatWo *wo, int version) {
	ChaPrefsTemplateWoPrivate *priv = cha_prefs_template_wo_get_instance_private((ChaPrefsTemplateWo *) wo);
	priv->name = cat_string_wo_anchor(priv->name, version);
	priv->description = cat_string_wo_anchor(priv->description, version);
	priv->template = cat_string_wo_anchor(priv->template, version);
}

static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const ChaPrefsTemplateWoPrivate *priv_a = cha_prefs_template_wo_get_instance_private((ChaPrefsTemplateWo *) wo_a);
	const ChaPrefsTemplateWoPrivate *priv_b = cha_prefs_template_wo_get_instance_private((ChaPrefsTemplateWo *) wo_b);
	return cat_string_wo_equal(priv_a->name, priv_b->name)
			&& cat_string_wo_equal(priv_a->description, priv_b->description)
			&& cat_string_wo_equal(priv_a->template, priv_b->template);
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_PREFS_TEMPLATE_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaPrefsTemplateWoPrivate *priv = cha_prefs_template_wo_get_instance_private(CHA_PREFS_TEMPLATE_WO(e_uninitialized));
	if (wo_source) {
		ChaPrefsTemplateWoPrivate *priv_src = cha_prefs_template_wo_get_instance_private(CHA_PREFS_TEMPLATE_WO(wo_source));
		priv->description = cat_string_wo_clone(priv_src->description, CAT_CLONE_DEPTH_NONE);
		priv->name = cat_string_wo_clone(priv_src->name, CAT_CLONE_DEPTH_NONE);
		priv->template = cat_string_wo_clone(priv_src->template, CAT_CLONE_DEPTH_NONE);
	} else {
		priv->name = cat_string_wo_new();
		priv->description = cat_string_wo_new();
		priv->template = cat_string_wo_new();
	}

	CatWoClass *wocls = CAT_WO_CLASS(cha_prefs_template_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}

CAT_WO_BASE_C(ChaPrefsTemplateWo, cha_prefs_template_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaPrefsTemplateWo *instance = CHA_PREFS_TEMPLATE_WO(self);
	ChaPrefsTemplateWoPrivate *priv = cha_prefs_template_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p: %s, name=%o]", iname, self, cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable", priv->name);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
