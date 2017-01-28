/*
   File:    jagpreferenceswo.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Sep 28, 2014
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

#include "jagpreferenceswo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagPreferencesWo"
#include <logging/catlog.h>

CatS jag_s_jaguar_config = CAT_S_DEF("jaguar");
CatS jag_s_java = CAT_S_DEF("java");
CatS jag_s_jdk_or_jre = CAT_S_DEF("jdk-or-jre");
CatS jag_s_jar_path = CAT_S_DEF("jar");

struct _JagPreferencesWoPrivate {
	CatArrayWo *jre_list;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagPreferencesWo, jag_preferences_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(JagPreferencesWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static void jag_preferences_wo_class_init(JagPreferencesWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->equal = l_equal;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_preferences_wo_init(JagPreferencesWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagPreferencesWo *instance = JAG_PREFERENCES_WO(object);
	JagPreferencesWoPrivate *priv = jag_preferences_wo_get_instance_private(instance);
	cat_unref_ptr(priv->jre_list);

	G_OBJECT_CLASS(jag_preferences_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_preferences_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


JagPreferencesWo *jag_preferences_wo_new() {
	JagPreferencesWo *result = g_object_new(JAG_TYPE_PREFERENCES_WO, NULL);
	cat_ref_anounce(result);
	cat_wo_construct((CatWo *) result, TRUE);
	return result;
}


CatArrayWo *jag_preferences_wo_get_jre_list(const JagPreferencesWo *preferences) {
	JagPreferencesWoPrivate *priv = jag_preferences_wo_get_instance_private((JagPreferencesWo *) preferences);
	return priv->jre_list;
}

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_preferences)) { \
			cat_log_error("Object is read only:%o", e_preferences); \
			return rval; \
		} \

CatArrayWo *jag_preferences_wo_get_editable_jre_list(JagPreferencesWo *e_preferences) {
	JagPreferencesWoPrivate *priv = jag_preferences_wo_get_instance_private(e_preferences);
	CHECK_IF_WRITABLE(NULL);
	if (cat_array_wo_is_anchored(priv->jre_list)) {
		CatArrayWo *e_list = cat_array_wo_create_editable(priv->jre_list);
		cat_unref_ptr(priv->jre_list);
		priv->jre_list = e_list;
	}
	return priv->jre_list;
}

static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(JAG_TYPE_PREFERENCES_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	JagPreferencesWoPrivate *priv = jag_preferences_wo_get_instance_private((JagPreferencesWo *) e_uninitialized);
	if (original) {
		JagPreferencesWoPrivate *rpriv = jag_preferences_wo_get_instance_private((JagPreferencesWo *) original);
		priv->jre_list = cat_ref_ptr(rpriv->jre_list);
	} else {
		priv->jre_list = cat_array_wo_new();
	}
	return CAT_WO_CLASS(jag_preferences_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}

static void l_anchor_children(CatWo *wo, int version) {
	JagPreferencesWoPrivate *priv = jag_preferences_wo_get_instance_private((JagPreferencesWo *) wo);
	priv->jre_list = cat_array_wo_anchor(priv->jre_list, version);
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const JagPreferencesWoPrivate *priv_a = jag_preferences_wo_get_instance_private((JagPreferencesWo *) wo_a);
	const JagPreferencesWoPrivate *priv_b = jag_preferences_wo_get_instance_private((JagPreferencesWo *) wo_b);
	return (cat_array_wo_equal(priv_a->jre_list, priv_b->jre_list, NULL));
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(JAG_TYPE_PREFERENCES_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	JagPreferencesWoPrivate *priv = jag_preferences_wo_get_instance_private(JAG_PREFERENCES_WO(e_uninitialized));
	if (wo_source) {
		JagPreferencesWoPrivate *priv_src = jag_preferences_wo_get_instance_private(JAG_PREFERENCES_WO(wo_source));
		priv->jre_list = cat_array_wo_clone(priv_src->jre_list, CAT_CLONE_DEPTH_NONE);
	} else {
		priv->jre_list = cat_array_wo_new();
	}

	CatWoClass *wocls = CAT_WO_CLASS(jag_preferences_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}

CAT_WO_BASE_C(JagPreferencesWo, jag_preferences_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	JagPreferencesWo *instance = JAG_PREFERENCES_WO(self);
	JagPreferencesWoPrivate *priv = jag_preferences_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p: list=%o]", iname, self, priv->jre_list);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/















