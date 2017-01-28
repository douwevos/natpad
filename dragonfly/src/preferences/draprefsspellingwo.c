/*
   File:    draprefsspellingwo.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 30, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#include "draprefsspellingwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraPrefsSpellingWo"
#include <logging/catlog.h>

struct _DraPrefsSpellingWoPrivate {
	gboolean is_enabled;
	CatStringWo *dictionary_name;
	int max_suggestions;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraPrefsSpellingWo, dra_prefs_spelling_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(DraPrefsSpellingWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static void dra_prefs_spelling_wo_class_init(DraPrefsSpellingWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_prefs_spelling_wo_init(DraPrefsSpellingWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraPrefsSpellingWo *instance = DRA_PREFS_SPELLING_WO(object);
	DraPrefsSpellingWoPrivate *priv = dra_prefs_spelling_wo_get_instance_private(instance);
	cat_unref_ptr(priv->dictionary_name);
	G_OBJECT_CLASS(dra_prefs_spelling_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_prefs_spelling_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraPrefsSpellingWo *dra_prefs_spelling_wo_new() {
	DraPrefsSpellingWo *result = g_object_new(DRA_TYPE_PREFS_SPELLING_WO, NULL);
	cat_ref_anounce(result);
	cat_wo_construct((CatWo *) result, TRUE);
	return result;
}

gboolean dra_prefs_spelling_wo_is_enabled(const DraPrefsSpellingWo *spelling) {
	DraPrefsSpellingWoPrivate *priv = dra_prefs_spelling_wo_get_instance_private((DraPrefsSpellingWo *) spelling);
	return priv->is_enabled;
}

CatStringWo *dra_prefs_spelling_wo_get_dictionary_name(const DraPrefsSpellingWo *spelling) {
	DraPrefsSpellingWoPrivate *priv = dra_prefs_spelling_wo_get_instance_private((DraPrefsSpellingWo *) spelling);
	return priv->dictionary_name;
}

int dra_prefs_spelling_wo_get_max_suggestions(const DraPrefsSpellingWo *spelling) {
	DraPrefsSpellingWoPrivate *priv = dra_prefs_spelling_wo_get_instance_private((DraPrefsSpellingWo *) spelling);
	return priv->max_suggestions;
}

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_spelling)) { \
			cat_log_error("Object is read only:%o", e_spelling); \
			return rval; \
		} \


void dra_prefs_spelling_wo_set_enabled(DraPrefsSpellingWo *e_spelling, gboolean is_enabled) {
	DraPrefsSpellingWoPrivate *priv = dra_prefs_spelling_wo_get_instance_private(e_spelling);
	CHECK_IF_WRITABLE();
	priv->is_enabled = is_enabled;
}

void dra_prefs_spelling_wo_set_dictionary_name(DraPrefsSpellingWo *e_spelling, CatStringWo *dict_name) {
	DraPrefsSpellingWoPrivate *priv = dra_prefs_spelling_wo_get_instance_private(e_spelling);
	CHECK_IF_WRITABLE();
	cat_ref_swap(priv->dictionary_name, dict_name);
}

void dra_prefs_spelling_wo_set_max_suggestions(DraPrefsSpellingWo *e_spelling, int max_suggestions) {
	DraPrefsSpellingWoPrivate *priv = dra_prefs_spelling_wo_get_instance_private(e_spelling);
	CHECK_IF_WRITABLE();
	priv->max_suggestions = max_suggestions;
}

static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(DRA_TYPE_PREFS_SPELLING_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	DraPrefsSpellingWoPrivate *priv = dra_prefs_spelling_wo_get_instance_private((DraPrefsSpellingWo *) e_uninitialized);
	if (original) {
		DraPrefsSpellingWoPrivate *rpriv = dra_prefs_spelling_wo_get_instance_private((DraPrefsSpellingWo *) original);
		priv->is_enabled = rpriv->is_enabled;
		priv->dictionary_name = cat_ref_ptr(rpriv->dictionary_name);
		priv->max_suggestions = rpriv->max_suggestions;
	} else {
		priv->is_enabled = TRUE;
		priv->dictionary_name = NULL;
		priv->max_suggestions = 5;
	}
	return CAT_WO_CLASS(dra_prefs_spelling_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const DraPrefsSpellingWoPrivate *priv_a = dra_prefs_spelling_wo_get_instance_private((DraPrefsSpellingWo *) wo_a);
	const DraPrefsSpellingWoPrivate *priv_b = dra_prefs_spelling_wo_get_instance_private((DraPrefsSpellingWo *) wo_b);
	return (priv_a->is_enabled==priv_b->is_enabled)
				&& (cat_string_wo_equal(priv_a->dictionary_name, priv_b->dictionary_name))
				&& priv_a->max_suggestions==priv_b->max_suggestions;
}


static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized == NULL) {
		e_uninitialized = g_object_new(DRA_TYPE_PREFS_SPELLING_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}
	DraPrefsSpellingWoPrivate *priv = dra_prefs_spelling_wo_get_instance_private((DraPrefsSpellingWo *) e_uninitialized);
	if (wo_source) {
		const DraPrefsSpellingWoPrivate *priv_src = dra_prefs_spelling_wo_get_instance_private((DraPrefsSpellingWo *) wo_source);
		priv->dictionary_name = cat_string_wo_clone(priv_src->dictionary_name, CAT_CLONE_DEPTH_NONE);
		priv->is_enabled = priv_src->is_enabled;
		priv->max_suggestions = priv_src->max_suggestions;
	} else {
		priv->dictionary_name = NULL;
		priv->is_enabled = TRUE;
		priv->max_suggestions = 5;
	}
	CatWoClass *woc = CAT_WO_CLASS(dra_prefs_spelling_wo_parent_class);
	if (woc->cloneContent) {
		return woc->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}


CAT_WO_BASE_C(DraPrefsSpellingWo,dra_prefs_spelling_wo);


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraPrefsSpellingWo *instance = DRA_PREFS_SPELLING_WO(self);
	DraPrefsSpellingWoPrivate *priv = dra_prefs_spelling_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p %s, is_enabled=%d, dict=%O]", iname, self, cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable", priv->is_enabled, priv->dictionary_name);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
