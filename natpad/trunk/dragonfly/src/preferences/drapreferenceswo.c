/*
   File:    drapreferenceswo.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jan 1, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#include "drapreferenceswo.h"
#include <woo/catwoprivate.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraPreferencesWo"
#include <logging/catlog.h>

struct _DraPreferencesWoPrivate {
	DraPrefsSpellingWo *spelling_wo;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraPreferencesWo, dra_preferences_wo, CHA_TYPE_PREFERENCES_WO,
		G_ADD_PRIVATE(DraPreferencesWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static void dra_preferences_wo_class_init(DraPreferencesWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_preferences_wo_init(DraPreferencesWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraPreferencesWo *instance = DRA_PREFERENCES_WO(object);
	DraPreferencesWoPrivate *priv = dra_preferences_wo_get_instance_private(instance);
	cat_unref_ptr(priv->spelling_wo);
	G_OBJECT_CLASS(dra_preferences_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_preferences_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraPreferencesWo *dra_preferences_wo_new() {
	DraPreferencesWo *result = g_object_new(DRA_TYPE_PREFERENCES_WO, NULL);
	cat_ref_anounce(result);
	DraPreferencesWoPrivate *priv = dra_preferences_wo_get_instance_private(result);
	cha_preferences_wo_construct((ChaPreferencesWo *) result);
	priv->spelling_wo = NULL;
	return result;
}


DraPrefsSpellingWo *dra_preferences_wo_get_spelling(DraPreferencesWo *prefs_wo) {
	DraPreferencesWoPrivate *priv = dra_preferences_wo_get_instance_private(prefs_wo);
	return priv->spelling_wo;
}


#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_prefs)) { \
			cat_log_error("Object is read only:%o", e_prefs); \
			return rval; \
		} \


void dra_preferences_wo_set_spelling(DraPreferencesWo *e_prefs, DraPrefsSpellingWo *spelling) {
	DraPreferencesWoPrivate *priv = dra_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	cat_ref_swap(priv->spelling_wo, spelling);
}

DraPrefsSpellingWo *dra_preferences_wo_get_editable_spelling(DraPreferencesWo *e_prefs) {
	DraPreferencesWoPrivate *priv = dra_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE(NULL);
	cat_log_trace("before=%O", priv->spelling_wo);
	if (priv->spelling_wo==NULL) {
		priv->spelling_wo = dra_prefs_spelling_wo_new();
	} else if (cat_wo_is_anchored((CatWo *) priv->spelling_wo)) {
		DraPrefsSpellingWo *e = dra_prefs_spelling_wo_create_editable(priv->spelling_wo);
		cat_ref_swap(priv->spelling_wo, e);
		cat_unref_ptr(e);
	}
	cat_log_trace("after=%O", priv->spelling_wo);
	return priv->spelling_wo;
}


static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(DRA_TYPE_PREFERENCES_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	DraPreferencesWoPrivate *priv = dra_preferences_wo_get_instance_private((DraPreferencesWo *) e_uninitialized);
	if (original) {
		DraPreferencesWoPrivate *rpriv = dra_preferences_wo_get_instance_private((DraPreferencesWo *) original);
		priv->spelling_wo = cat_ref_ptr(rpriv->spelling_wo);
	} else {
		priv->spelling_wo = NULL;
	}
	return CAT_WO_CLASS(dra_preferences_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}

static void l_anchor_children(CatWo *wo, int version) {
	DraPreferencesWoPrivate *priv = dra_preferences_wo_get_instance_private((DraPreferencesWo *) wo);
	priv->spelling_wo = dra_prefs_spelling_wo_anchor(priv->spelling_wo, version);
	CatWoClass *wo_class = CAT_WO_CLASS(dra_preferences_wo_parent_class);
	if (wo_class->anchorChildren) {
		wo_class->anchorChildren(wo, version);
	}
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const DraPreferencesWoPrivate *priv_a = dra_preferences_wo_get_instance_private((DraPreferencesWo *) wo_a);
	const DraPreferencesWoPrivate *priv_b = dra_preferences_wo_get_instance_private((DraPreferencesWo *) wo_b);
	gboolean result = (dra_prefs_spelling_wo_equal(priv_a->spelling_wo, priv_b->spelling_wo));
	if (result) {
		result = CAT_WO_CLASS(dra_preferences_wo_parent_class)->equal(wo_a, wo_b);
	}
	cat_log_error("result=%d, spell_a=%O, spell_b=%O", result, priv_a->spelling_wo, priv_b->spelling_wo);
	return result;
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized == NULL) {
		e_uninitialized = g_object_new(DRA_TYPE_PREFERENCES_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}
	DraPreferencesWoPrivate *priv = dra_preferences_wo_get_instance_private((DraPreferencesWo *) e_uninitialized);
	if (wo_source) {
		const DraPreferencesWoPrivate *priv_source = dra_preferences_wo_get_instance_private((DraPreferencesWo *) wo_source);
		priv->spelling_wo = dra_prefs_spelling_wo_clone(priv_source->spelling_wo, CAT_CLONE_DEPTH_NONE);
	} else {
		priv->spelling_wo = dra_prefs_spelling_wo_new();
	}
	CatWoClass *woc = CAT_WO_CLASS(dra_preferences_wo_parent_class);
	if (woc->cloneContent) {
		return woc->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}


CAT_WO_BASE_C(DraPreferencesWo,dra_preferences_wo);


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraPreferencesWo *instance = DRA_PREFERENCES_WO(self);
	DraPreferencesWoPrivate *priv = dra_preferences_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p %s, spelling_wo=%O ## ", iname, self, cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable", priv->spelling_wo);

	CatIStringableInterface *striface = CAT_ISTRINGABLE_GET_INTERFACE(self);
	striface = g_type_interface_peek_parent(striface);
	striface->print(self, append_to);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
