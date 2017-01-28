/*
   File:    terpreferenceswo.c
   Project: tern
   Author:  Douwe Vos
   Date:    Oct 26, 2014
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

#include "terpreferenceswo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerPreferencesWo"
#include <logging/catlog.h>

struct _TerPreferencesWoPrivate {
	CatHashMapWo *clazz_map;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TerPreferencesWo, ter_preferences_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(TerPreferencesWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static void ter_preferences_wo_class_init(TerPreferencesWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->equal = l_equal;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_preferences_wo_init(TerPreferencesWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TerPreferencesWo *instance = TER_PREFERENCES_WO(object);
	TerPreferencesWoPrivate *priv = ter_preferences_wo_get_instance_private(instance);
	cat_unref_ptr(priv->clazz_map);
	G_OBJECT_CLASS(ter_preferences_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(ter_preferences_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


TerPreferencesWo *ter_preferences_wo_new() {
	TerPreferencesWo *result = g_object_new(TER_TYPE_PREFERENCES_WO, NULL);
	cat_ref_anounce(result);
	TerPreferencesWoPrivate *priv = ter_preferences_wo_get_instance_private(result);
	priv->clazz_map = NULL;
	cat_wo_construct((CatWo *) result, TRUE);
	return result;
}

TerPrefsClazzWo *ter_preferences_wo_get_clazz(const TerPreferencesWo *preferences, const CatStringWo *clazz_name) {
	const TerPreferencesWoPrivate *priv = ter_preferences_wo_get_instance_private((TerPreferencesWo *) preferences);
	return (TerPrefsClazzWo *) cat_hash_map_wo_get(priv->clazz_map, (void *) clazz_name);
}

CatHashMapWo *ter_preferences_wo_get_document_clazzes(const TerPreferencesWo *preferences) {
	const TerPreferencesWoPrivate *priv = ter_preferences_wo_get_instance_private((TerPreferencesWo *) preferences);
	return priv->clazz_map;
}



#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_preferences)) { \
			cat_log_error("Object is read only:%o", e_preferences); \
			return rval; \
		} \


void ter_preferences_wo_set_clazz(TerPreferencesWo *e_preferences, TerPrefsClazzWo *clazz) {
	TerPreferencesWoPrivate *priv = ter_preferences_wo_get_instance_private(e_preferences);
	CHECK_IF_WRITABLE();
	if (cat_hash_map_wo_is_anchored(priv->clazz_map)) {
		CatHashMapWo *nmap = cat_hash_map_wo_clone(priv->clazz_map, CAT_CLONE_DEPTH_MAIN);
		cat_ref_swap(priv->clazz_map, nmap);
	}
	cat_hash_map_wo_put(priv->clazz_map, (GObject *) ter_prefs_clazz_wo_get_name(clazz), (GObject *) clazz);
}



static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(TER_TYPE_PREFERENCES_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	TerPreferencesWoPrivate *priv = ter_preferences_wo_get_instance_private((TerPreferencesWo *) e_uninitialized);
	if (original) {
		TerPreferencesWoPrivate *rpriv = ter_preferences_wo_get_instance_private((TerPreferencesWo *) original);
		priv->clazz_map = cat_hash_map_wo_clone(rpriv->clazz_map, CAT_CLONE_DEPTH_NONE);
	} else {
		priv->clazz_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	}
	return CAT_WO_CLASS(ter_preferences_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}

static void l_anchor_children(CatWo *wo, int version) {
	TerPreferencesWoPrivate *priv = ter_preferences_wo_get_instance_private((TerPreferencesWo *) wo);
	priv->clazz_map = cat_hash_map_wo_anchor(priv->clazz_map, version);
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const TerPreferencesWoPrivate *priv_a = ter_preferences_wo_get_instance_private((TerPreferencesWo *) wo_a);
	const TerPreferencesWoPrivate *priv_b = ter_preferences_wo_get_instance_private((TerPreferencesWo *) wo_b);
	return cat_hash_map_wo_equal(priv_a->clazz_map, priv_b->clazz_map, NULL);
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(TER_TYPE_PREFERENCES_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	TerPreferencesWoPrivate *priv = ter_preferences_wo_get_instance_private(TER_PREFERENCES_WO(e_uninitialized));
	if (wo_source) {
		TerPreferencesWoPrivate *priv_src = ter_preferences_wo_get_instance_private(TER_PREFERENCES_WO(wo_source));
		priv->clazz_map = cat_hash_map_wo_clone(priv_src->clazz_map, CAT_CLONE_DEPTH_NONE);
	} else {
		priv->clazz_map = NULL;
	}


	CatWoClass *wocls = CAT_WO_CLASS(ter_preferences_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;

}

CAT_WO_BASE_C(TerPreferencesWo, ter_preferences_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *e_append_to) {
	TerPreferencesWo *instance = TER_PREFERENCES_WO(self);
	TerPreferencesWoPrivate *priv = ter_preferences_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(e_append_to, "%s[%p(rc=%d): %s clazz-map=%o]", iname, self, ((GObject *) self)->ref_count, cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable", priv->clazz_map);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
