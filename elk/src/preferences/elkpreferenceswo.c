/*
   File:    elkpreferenceswo.c
   Project: elk
   Author:  Douwe Vos
   Date:    Sep 23, 2014
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

#include "elkpreferenceswo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkPreferencesWo"
#include <logging/catlog.h>

CatS elk_s_editor_prefs = CAT_S_DEF("editor.prefs");
CatS elk_s_editor2_prefs = CAT_S_DEF("dra.editor.prefs");

struct _ElkPreferencesWoPrivate {
	int version;
	gboolean one_instance;
	gboolean full_screen;
	gboolean multiple_editors_per_resource;
	gboolean backup_files;
	CatHashMapWo *sub_map;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ElkPreferencesWo, elk_preferences_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(ElkPreferencesWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static void elk_preferences_wo_class_init(ElkPreferencesWoClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->equal = l_equal;
	wo_class->cloneContent = l_clone_content;
}

static void elk_preferences_wo_init(ElkPreferencesWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%o", object);
	cat_log_indent_level++;
	ElkPreferencesWo *instance = ELK_PREFERENCES_WO(object);
	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private(instance);
	cat_log_debug("dispose:%p, sub-map=%p", object, priv->sub_map);
	cat_unref_ptr(priv->sub_map);
	G_OBJECT_CLASS(elk_preferences_wo_parent_class)->dispose(object);
	cat_log_indent_level--;
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(elk_preferences_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ElkPreferencesWo *elk_preferences_wo_new() {
	ElkPreferencesWo *result = g_object_new(ELK_TYPE_PREFERENCES_WO, NULL);
	cat_ref_anounce(result);
	cat_log_debug("anounce-new:%p", result);
	cat_wo_construct((CatWo *) result, TRUE);
	return result;
}


gboolean elk_preferences_wo_one_instance(ElkPreferencesWo *prefs) {
	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private(prefs);
	return priv->one_instance;
}

gboolean elk_preferences_wo_full_screen(ElkPreferencesWo *prefs) {
	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private(prefs);
	return priv->full_screen;
}

gboolean elk_preferences_wo_multiple_resource_editors(ElkPreferencesWo *prefs) {
	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private(prefs);
	return priv->multiple_editors_per_resource;
}

gboolean elk_preferences_wo_backup_files(ElkPreferencesWo *prefs) {
	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private(prefs);
	return priv->backup_files;
}

GObject *elk_preferences_wo_get_sub(ElkPreferencesWo *prefs, CatStringWo *key) {
	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private(prefs);
	return cat_hash_map_wo_get(priv->sub_map, key);
}

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_prefs)) { \
			cat_log_error("Object is read only:%o", e_prefs); \
			return rval; \
		} \


void elk_preferences_wo_set_one_instance(ElkPreferencesWo *e_prefs, gboolean one_instance) {
	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private(e_prefs);
	if (priv->one_instance==one_instance) {
		return;
	}
	CHECK_IF_WRITABLE();
	priv->one_instance = one_instance;
}

void elk_preferences_wo_set_full_screen(ElkPreferencesWo *e_prefs, gboolean full_screen) {
	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private(e_prefs);
	if (priv->full_screen==full_screen) {
		return;
	}
	CHECK_IF_WRITABLE();
	priv->full_screen = full_screen;
}

void elk_preferences_wo_set_sub(ElkPreferencesWo *e_prefs, CatStringWo *key, GObject *sub) {
	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private(e_prefs);
	CHECK_IF_WRITABLE();
	if (cat_hash_map_wo_is_anchored(priv->sub_map)) {
		cat_log_debug("set-sub:%p, current-map=%p", e_prefs, priv->sub_map);
		CatHashMapWo *new_map = cat_hash_map_wo_create_editable(priv->sub_map);
		cat_unref_ptr(priv->sub_map);
		priv->sub_map = new_map;
	}

	cat_hash_map_wo_put(priv->sub_map, (GObject *) key, sub);
	cat_log_debug("%p: adding key:%o", e_prefs, key);
}

void elk_preferences_wo_set_multiple_resource_editors(ElkPreferencesWo *e_prefs, gboolean multiple_resource_editors) {
	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private(e_prefs);
	if (priv->multiple_editors_per_resource==multiple_resource_editors) {
		return;
	}
	CHECK_IF_WRITABLE();
	priv->multiple_editors_per_resource = multiple_resource_editors;
}

void elk_preferences_wo_set_backup_files(ElkPreferencesWo *e_prefs, gboolean backup_files) {
	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private(e_prefs);
	if (priv->backup_files==backup_files) {
		return;
	}
	CHECK_IF_WRITABLE();
	priv->backup_files = backup_files;
}

void elk_preferences_wo_dump(ElkPreferencesWo *prefs) {
	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private(prefs);
	cat_log_print("DUMP", "prefs=%p(%d), priv->sub_map=%p", prefs, ((GObject *) prefs)->ref_count, priv->sub_map);
	CatArrayWo *keys = cat_hash_map_wo_enlist_keys(priv->sub_map, NULL);
	CatIIterator *iter = cat_array_wo_iterator(keys);
	while(cat_iiterator_has_next(iter)) {
		GObject *key = cat_iiterator_next(iter);
		cat_log_print("DUMP", "key=%o", key);
		GObject *value = cat_hash_map_wo_get(priv->sub_map, key);
		cat_log_print("DUMP", "value=%p", value);
		cat_log_print("DUMP", "value=%o", value);
	}
	cat_unref_ptr(iter);
	cat_unref_ptr(keys);
}


static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(ELK_TYPE_PREFERENCES_WO, NULL);
		cat_log_debug("anounce-construct_editable:%p", e_uninitialized);
		cat_ref_anounce(e_uninitialized);
	}

	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private((ElkPreferencesWo *) e_uninitialized);

	if (original) {
		ElkPreferencesWoPrivate *rpriv = elk_preferences_wo_get_instance_private((ElkPreferencesWo *) original);
		priv->multiple_editors_per_resource = rpriv->multiple_editors_per_resource;
		priv->one_instance = rpriv->one_instance;
		priv->full_screen = rpriv->full_screen;
		priv->backup_files = rpriv->backup_files;
		cat_log_debug("constr-editable:%p, current-map=%p", e_uninitialized, priv->sub_map);
		priv->sub_map = cat_hash_map_wo_create_editable(rpriv->sub_map);
	} else {
		priv->full_screen = TRUE;
		priv->multiple_editors_per_resource = FALSE;
		priv->one_instance = TRUE;
		priv->backup_files = TRUE;
		cat_log_debug("constr-editable:%p, current-map=%p", e_uninitialized, priv->sub_map);
		priv->sub_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
	}

	return CAT_WO_CLASS(elk_preferences_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}

static void l_anchor_children(CatWo *wo, int version) {
	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private((ElkPreferencesWo *) wo);
	cat_log_debug("priv->sub_map=%p, ref-cnt = %d", priv->sub_map, ((GObject *) (priv->sub_map))->ref_count);
	priv->sub_map = cat_hash_map_wo_anchor(priv->sub_map, version);

}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	}
	if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	ElkPreferencesWoPrivate *priv_a = elk_preferences_wo_get_instance_private((ElkPreferencesWo *) wo_a);
	ElkPreferencesWoPrivate *priv_b = elk_preferences_wo_get_instance_private((ElkPreferencesWo *) wo_b);
	if ((priv_a->full_screen==priv_b->full_screen)
			&& (priv_a->multiple_editors_per_resource==priv_b->multiple_editors_per_resource)
			&& (priv_a->one_instance==priv_b->one_instance)
			&& (priv_a->backup_files==priv_b->backup_files)
			&& (cat_hash_map_wo_equal(priv_a->sub_map, priv_b->sub_map, NULL))) {
		return TRUE;
	}
	return FALSE;
}


static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(ELK_TYPE_PREFERENCES_WO, NULL);
		cat_ref_anounce(e_uninitialized);
		cat_log_debug("anounced-clone-content:%p", e_uninitialized);
	}

	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private(ELK_PREFERENCES_WO(e_uninitialized));
	if (wo_source) {
		ElkPreferencesWoPrivate *priv_src = elk_preferences_wo_get_instance_private(ELK_PREFERENCES_WO(wo_source));
		cat_log_debug("clone-content:%p, current-map=%p", e_uninitialized, priv->sub_map);
		priv->sub_map = cat_hash_map_wo_clone(priv_src->sub_map, CAT_CLONE_DEPTH_NONE);
		cat_log_debug("clone-content:%p, current-map=%p", e_uninitialized, priv->sub_map);
		priv->full_screen = priv_src->full_screen;
		priv->multiple_editors_per_resource = priv_src->multiple_editors_per_resource;
		priv->one_instance = priv_src->one_instance;
		priv->backup_files = priv_src->backup_files;
	} else {
		priv->full_screen = TRUE;
		priv->multiple_editors_per_resource = FALSE;
		priv->one_instance = TRUE;
		priv->backup_files = TRUE;
		cat_log_debug("clone-content:%p, current-map=%p", e_uninitialized, priv->sub_map);
		priv->sub_map = cat_hash_map_wo_new((GHashFunc) cat_string_wo_hash, (GEqualFunc) cat_string_wo_equal);
		cat_log_debug("clone-content:%p, current-map=%p", e_uninitialized, priv->sub_map);
	}

	CatWoClass *wocls = CAT_WO_CLASS(elk_preferences_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}

CAT_WO_BASE_C(ElkPreferencesWo, elk_preferences_wo)

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ElkPreferencesWo *instance = ELK_PREFERENCES_WO(self);
	ElkPreferencesWoPrivate *priv = elk_preferences_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p(%d), %s[org:%p], fullscreen=%d, multipleeditors=%d, oneinstance=%d, sub_map=%o]", iname, self, cat_wo_get_version((CatWo *) instance), cat_wo_is_anchored((CatWo *) instance) ? "anchored" : "editable", cat_wo_get_original((CatWo *) instance), priv->full_screen, priv->multiple_editors_per_resource, priv->one_instance, priv->sub_map);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/







