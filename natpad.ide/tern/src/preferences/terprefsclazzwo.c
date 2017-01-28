/*
   File:    terprefsclazzwo.c
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

#include "terprefsclazzwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerPrefsClazzWo"
#include <logging/catlog.h>


struct _TerPrefsClazzWoPrivate {
	CatStringWo *name;
	CatStringWo *grammar_file;
	CatStringWo *extra_characters;
	CatArrayWo *pattern_list;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TerPrefsClazzWo, ter_prefs_clazz_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(TerPrefsClazzWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static void ter_prefs_clazz_wo_class_init(TerPrefsClazzWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->constructEditable = l_construct_editable;
	wo_class->equal = l_equal;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_prefs_clazz_wo_init(TerPrefsClazzWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TerPrefsClazzWo *instance = TER_PREFS_CLAZZ_WO(object);
	TerPrefsClazzWoPrivate *priv = ter_prefs_clazz_wo_get_instance_private(instance);

	cat_unref_ptr(priv->name);
	cat_unref_ptr(priv->grammar_file);
	cat_unref_ptr(priv->extra_characters);
	cat_unref_ptr(priv->pattern_list);

	G_OBJECT_CLASS(ter_prefs_clazz_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(ter_prefs_clazz_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


TerPrefsClazzWo *ter_prefs_clazz_wo_new(CatStringWo *name) {
	TerPrefsClazzWo *result = g_object_new(TER_TYPE_PREFS_CLAZZ_WO, NULL);
	cat_ref_anounce(result);
	TerPrefsClazzWoPrivate *priv = ter_prefs_clazz_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->name = cat_ref_ptr(name);
	priv->pattern_list = name==NULL? NULL : cat_array_wo_new();
	priv->grammar_file = NULL;
	priv->extra_characters = NULL;
	return result;
}




CatStringWo *ter_prefs_clazz_wo_get_name(const TerPrefsClazzWo *prefs_clazz) {
	const TerPrefsClazzWoPrivate *priv = ter_prefs_clazz_wo_get_instance_private((TerPrefsClazzWo *) prefs_clazz);
	return priv->name;
}

CatStringWo *ter_prefs_clazz_wo_get_grammar_file(const TerPrefsClazzWo *prefs_clazz) {
	const TerPrefsClazzWoPrivate *priv = ter_prefs_clazz_wo_get_instance_private((TerPrefsClazzWo *) prefs_clazz);
	return priv->grammar_file;
}

CatStringWo *ter_prefs_clazz_wo_get_extra_characters(const TerPrefsClazzWo *prefs_clazz) {
	const TerPrefsClazzWoPrivate *priv = ter_prefs_clazz_wo_get_instance_private((TerPrefsClazzWo *) prefs_clazz);
	return priv->extra_characters;
}

CatArrayWo *ter_prefs_clazz_wo_get_pattern_list(const TerPrefsClazzWo *prefs_clazz) {
	const TerPrefsClazzWoPrivate *priv = ter_prefs_clazz_wo_get_instance_private((TerPrefsClazzWo *) prefs_clazz);
	return priv->pattern_list;
}

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_prefs_clazz)) { \
			cat_log_error("Object is read only:%o", e_prefs_clazz); \
			return rval; \
		} \


void ter_prefs_clazz_wo_set_name(TerPrefsClazzWo *e_prefs_clazz, CatStringWo *name) {
	TerPrefsClazzWoPrivate *priv = ter_prefs_clazz_wo_get_instance_private(e_prefs_clazz);
	CHECK_IF_WRITABLE();
	cat_ref_swap(priv->name, name);
}

void ter_prefs_clazz_wo_set_grammar_file(TerPrefsClazzWo *e_prefs_clazz, CatStringWo *grammar_file) {
	TerPrefsClazzWoPrivate *priv = ter_prefs_clazz_wo_get_instance_private(e_prefs_clazz);
	CHECK_IF_WRITABLE();
	cat_ref_swap(priv->grammar_file, grammar_file);
}


void ter_prefs_clazz_wo_set_extra_characters(TerPrefsClazzWo *e_prefs_clazz, CatStringWo *extra_characters) {
	TerPrefsClazzWoPrivate *priv = ter_prefs_clazz_wo_get_instance_private(e_prefs_clazz);
	CHECK_IF_WRITABLE();
	cat_ref_swap(priv->extra_characters, extra_characters);
}

void ter_prefs_clazz_wo_add_pattern(TerPrefsClazzWo *e_prefs_clazz, CatStringWo *filename_pattern) {
	TerPrefsClazzWoPrivate *priv = ter_prefs_clazz_wo_get_instance_private(e_prefs_clazz);
	CHECK_IF_WRITABLE();
	cat_array_wo_append(priv->pattern_list, (GObject *) filename_pattern);
}

void ter_prefs_clazz_wo_set_pattern_at(TerPrefsClazzWo *e_prefs_clazz, CatStringWo *filename_pattern, int at_index) {
	TerPrefsClazzWoPrivate *priv = ter_prefs_clazz_wo_get_instance_private(e_prefs_clazz);
	CHECK_IF_WRITABLE();
	cat_array_wo_set(priv->pattern_list, (GObject *) filename_pattern, at_index, NULL);
}

void ter_prefs_clazz_wo_remove_pattern(TerPrefsClazzWo *e_prefs_clazz, CatStringWo *pattern_to_remove) {
	TerPrefsClazzWoPrivate *priv = ter_prefs_clazz_wo_get_instance_private(e_prefs_clazz);
	CHECK_IF_WRITABLE();
	int idx = cat_array_wo_find_index_ext(priv->pattern_list, (GObject *) pattern_to_remove, -1, (GEqualFunc) cat_string_wo_equal);
	if (idx>=0) {
		cat_array_wo_remove(priv->pattern_list, idx, NULL);
	}
}


static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(TER_TYPE_PREFS_CLAZZ_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	TerPrefsClazzWoPrivate *priv = ter_prefs_clazz_wo_get_instance_private((TerPrefsClazzWo *) e_uninitialized);
	if (original) {
		TerPrefsClazzWoPrivate *rpriv = ter_prefs_clazz_wo_get_instance_private((TerPrefsClazzWo *) original);
		priv->name = cat_ref_ptr(rpriv->name);
		priv->grammar_file = cat_ref_ptr(rpriv->grammar_file);
		priv->extra_characters = cat_ref_ptr(rpriv->extra_characters);
		priv->pattern_list = cat_ref_ptr(rpriv->pattern_list);
	} else {
		priv->name = NULL;
		priv->grammar_file = NULL;
		priv->extra_characters = NULL;
		priv->pattern_list = NULL;
	}
	return CAT_WO_CLASS(ter_prefs_clazz_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const TerPrefsClazzWoPrivate *priv_a = ter_prefs_clazz_wo_get_instance_private((TerPrefsClazzWo *) wo_a);
	const TerPrefsClazzWoPrivate *priv_b = ter_prefs_clazz_wo_get_instance_private((TerPrefsClazzWo *) wo_b);
	return cat_string_wo_equal(priv_a->name, priv_b->name)
			&& cat_string_wo_equal(priv_a->grammar_file, priv_b->grammar_file)
			&& cat_string_wo_equal(priv_a->extra_characters, priv_b->extra_characters)
			&& cat_array_wo_equal(priv_a->pattern_list, priv_b->pattern_list, NULL);
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(TER_TYPE_PREFS_CLAZZ_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	TerPrefsClazzWoPrivate *priv = ter_prefs_clazz_wo_get_instance_private(TER_PREFS_CLAZZ_WO(e_uninitialized));
	if (wo_source) {
		TerPrefsClazzWoPrivate *priv_src = ter_prefs_clazz_wo_get_instance_private(TER_PREFS_CLAZZ_WO(wo_source));
		priv->name = cat_string_wo_clone(priv_src->name, CAT_CLONE_DEPTH_NONE);
		priv->grammar_file = cat_string_wo_clone(priv_src->grammar_file, CAT_CLONE_DEPTH_NONE);
		priv->extra_characters = cat_string_wo_clone(priv_src->extra_characters, CAT_CLONE_DEPTH_NONE);
		priv->pattern_list = cat_array_wo_clone(priv_src->pattern_list, CAT_CLONE_DEPTH_NONE);
	} else {
		priv->name = NULL;
		priv->grammar_file = NULL;
		priv->extra_characters = NULL;
		priv->pattern_list = NULL;
	}

	CatWoClass *wocls = CAT_WO_CLASS(ter_prefs_clazz_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}


CAT_WO_BASE_C(TerPrefsClazzWo, ter_prefs_clazz_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *e_append_to) {
	TerPrefsClazzWo *instance = TER_PREFS_CLAZZ_WO(self);
	TerPrefsClazzWoPrivate *priv = ter_prefs_clazz_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(e_append_to, "%s[%p: %s grammar_file=%o]", iname, self, cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable", priv->grammar_file);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
