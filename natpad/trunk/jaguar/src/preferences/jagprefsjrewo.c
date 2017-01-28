/*
   File:    jagprefsjrewo.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Oct 9, 2014
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

#include "jagprefsjrewo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPrefsJreWo"
#include <logging/catlog.h>

struct _JagPrefsJreWoPrivate {
	CatStringWo *name;
	CatStringWo *path;
	gboolean is_jdk;
	CatArrayWo *library_paths;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPrefsJreWo, jag_prefs_jre_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(JagPrefsJreWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static void jag_prefs_jre_wo_class_init(JagPrefsJreWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->equal = l_equal;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_prefs_jre_wo_init(JagPrefsJreWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPrefsJreWo *instance = JAG_PREFS_JRE_WO(object);
	JagPrefsJreWoPrivate *priv = jag_prefs_jre_wo_get_instance_private(instance);
	cat_unref_ptr(priv->library_paths);
	cat_unref_ptr(priv->name);
	cat_unref_ptr(priv->path);
	G_OBJECT_CLASS(jag_prefs_jre_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_prefs_jre_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPrefsJreWo *jag_prefs_jre_wo_new() {
	JagPrefsJreWo *result = g_object_new(JAG_TYPE_PREFS_JRE_WO, NULL);
	cat_ref_anounce(result);
	cat_wo_construct((CatWo *) result, TRUE);
	return result;
}


CatStringWo *jag_prefs_jre_wo_get_name(const JagPrefsJreWo *prefs) {
	const JagPrefsJreWoPrivate *priv = jag_prefs_jre_wo_get_instance_private((JagPrefsJreWo *) prefs);
	return priv->name;
}

CatStringWo *jag_prefs_jre_wo_get_path(const JagPrefsJreWo *prefs) {
	const JagPrefsJreWoPrivate *priv = jag_prefs_jre_wo_get_instance_private((JagPrefsJreWo *) prefs);
	return priv->path;
}

CatArrayWo *jag_prefs_jre_wo_get_library_paths(const JagPrefsJreWo *prefs) {
	const JagPrefsJreWoPrivate *priv = jag_prefs_jre_wo_get_instance_private((JagPrefsJreWo *) prefs);
	return priv->library_paths;
}

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_prefs)) { \
			cat_log_error("Object is read only:%o", e_prefs); \
			return rval; \
		} \

void jag_prefs_jre_wo_set_name(JagPrefsJreWo *e_prefs, CatStringWo *new_name) {
	JagPrefsJreWoPrivate *priv = jag_prefs_jre_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	cat_ref_swap(priv->name, cat_string_wo_anchor(new_name, 0));
}

void jag_prefs_jre_wo_set_path(JagPrefsJreWo *e_prefs, CatStringWo *new_path) {
	JagPrefsJreWoPrivate *priv = jag_prefs_jre_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	cat_ref_swap(priv->path, new_path);
}

void jag_prefs_jre_wo_set_library_paths(JagPrefsJreWo *e_prefs, CatArrayWo *new_paths) {
	JagPrefsJreWoPrivate *priv = jag_prefs_jre_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	cat_ref_swap(priv->library_paths, new_paths);
}


static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(JAG_TYPE_PREFS_JRE_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	JagPrefsJreWoPrivate *priv = jag_prefs_jre_wo_get_instance_private((JagPrefsJreWo *) e_uninitialized);
	if (original) {
		JagPrefsJreWoPrivate *rpriv = jag_prefs_jre_wo_get_instance_private((JagPrefsJreWo *) original);
		priv->is_jdk = rpriv->is_jdk;
		priv->library_paths = cat_ref_ptr(rpriv->library_paths);
		priv->name = cat_ref_ptr(rpriv->name);
		priv->path = cat_ref_ptr(rpriv->path);
	} else {
		priv->is_jdk = FALSE;
		priv->library_paths = NULL;
		priv->name = NULL;
		priv->path = NULL;
	}
	return CAT_WO_CLASS(jag_prefs_jre_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}

static void l_anchor_children(CatWo *wo, int version) {
	JagPrefsJreWoPrivate *priv = jag_prefs_jre_wo_get_instance_private((JagPrefsJreWo *) wo);
	priv->library_paths = cat_array_wo_anchor(priv->library_paths, version);
	priv->name = cat_string_wo_anchor(priv->name, version);
	priv->path = cat_string_wo_anchor(priv->path, version);
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const JagPrefsJreWoPrivate *priv_a = jag_prefs_jre_wo_get_instance_private((JagPrefsJreWo *) wo_a);
	const JagPrefsJreWoPrivate *priv_b = jag_prefs_jre_wo_get_instance_private((JagPrefsJreWo *) wo_b);
	return (cat_string_wo_equal(priv_a->name, priv_b->name)
			&& cat_string_wo_equal(priv_a->path, priv_b->path)
			&& cat_array_wo_equal(priv_a->library_paths, priv_b->library_paths, NULL)
			&& (priv_a->is_jdk == priv_b->is_jdk));
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(JAG_TYPE_PREFS_JRE_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	JagPrefsJreWoPrivate *priv = jag_prefs_jre_wo_get_instance_private(JAG_PREFS_JRE_WO(e_uninitialized));
	if (wo_source) {
		JagPrefsJreWoPrivate *priv_src = jag_prefs_jre_wo_get_instance_private(JAG_PREFS_JRE_WO(wo_source));
		priv->is_jdk = priv_src->is_jdk;
		priv->name = cat_string_wo_clone(priv_src->name, CAT_CLONE_DEPTH_NONE);
		priv->path = cat_string_wo_clone(priv_src->path, CAT_CLONE_DEPTH_NONE);
		priv->library_paths = cat_array_wo_clone(priv_src->library_paths, CAT_CLONE_DEPTH_NONE);

	} else {
		priv->is_jdk = FALSE;
		priv->library_paths = NULL;
		priv->name = NULL;
		priv->path = NULL;
	}

	CatWoClass *wocls = CAT_WO_CLASS(jag_prefs_jre_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}

CAT_WO_BASE_C(JagPrefsJreWo, jag_prefs_jre_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPrefsJreWo *instance = JAG_PREFS_JRE_WO(self);
	JagPrefsJreWoPrivate *priv = jag_prefs_jre_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p: %s, name=%o, path=%o, library_paths=%o]", iname, self, cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable", priv->name, priv->path, priv->library_paths);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

