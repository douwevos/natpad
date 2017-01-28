/*
   File:    chasearchquerywo.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Sep 9, 2015
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

#include "chasearchquerywo.h"
#include "chasearchoccurrence.h"
#include "chasearchpageoccurrences.h"
#include <string.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaSearchQueryWo"
#include <logging/catlog.h>

struct _ChaSearchQueryWoPrivate {
	CatStringWo *a_text;
	gboolean match_case;
	gboolean match_words;
	gboolean regexp;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaSearchQueryWo, cha_search_query_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(ChaSearchQueryWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);


static void cha_search_query_wo_class_init(ChaSearchQueryWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_search_query_wo_init(ChaSearchQueryWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaSearchQueryWo *instance = CHA_SEARCH_QUERY_WO(object);
	ChaSearchQueryWoPrivate *priv = cha_search_query_wo_get_instance_private(instance);
	cat_unref_ptr(priv->a_text);
	G_OBJECT_CLASS(cha_search_query_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_search_query_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaSearchQueryWo *cha_search_query_wo_new() {
	ChaSearchQueryWo *result = g_object_new(CHA_TYPE_SEARCH_QUERY_WO, NULL);
	cat_ref_anounce(result);
	ChaSearchQueryWoPrivate *priv = cha_search_query_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->a_text = NULL;
	priv->match_case = FALSE;
	priv->match_words = FALSE;
	priv->regexp = FALSE;
	return result;
}

CatStringWo *cha_search_query_wo_get_text(const ChaSearchQueryWo *query) {
	const ChaSearchQueryWoPrivate *priv = cha_search_query_wo_get_instance_private((ChaSearchQueryWo *) query);
	return priv->a_text;
}

gboolean cha_search_query_wo_get_regexp(const ChaSearchQueryWo *query) {
	const ChaSearchQueryWoPrivate *priv = cha_search_query_wo_get_instance_private((ChaSearchQueryWo *) query);
	return priv->regexp;
}

gboolean cha_search_query_wo_get_match_case(const ChaSearchQueryWo *query)  {
	const ChaSearchQueryWoPrivate *priv = cha_search_query_wo_get_instance_private((ChaSearchQueryWo *) query);
	return priv->match_case;
}

gboolean cha_search_query_wo_get_match_words(const ChaSearchQueryWo *query)  {
	const ChaSearchQueryWoPrivate *priv = cha_search_query_wo_get_instance_private((ChaSearchQueryWo *) query);
	return priv->match_words;
}

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_query)) { \
			cat_log_error("Object is read only:%o", e_query); \
			return rval; \
		} \

void cha_search_query_wo_set_text(ChaSearchQueryWo *e_query, CatStringWo *text) {
	CHECK_IF_WRITABLE()
	ChaSearchQueryWoPrivate *priv = cha_search_query_wo_get_instance_private(e_query);
	cat_ref_swap(priv->a_text, text);
}

void cha_search_query_wo_set_regexp(ChaSearchQueryWo *e_query, gboolean regexp) {
	CHECK_IF_WRITABLE()
	ChaSearchQueryWoPrivate *priv = cha_search_query_wo_get_instance_private(e_query);
	priv->regexp = regexp;
}

void cha_search_query_wo_set_match_case(ChaSearchQueryWo *e_query, gboolean match_case) {
	CHECK_IF_WRITABLE()
	ChaSearchQueryWoPrivate *priv = cha_search_query_wo_get_instance_private(e_query);
	priv->match_case = match_case;
}

void cha_search_query_wo_set_match_words(ChaSearchQueryWo *e_query, gboolean match_words) {
	CHECK_IF_WRITABLE()
	ChaSearchQueryWoPrivate *priv = cha_search_query_wo_get_instance_private(e_query);
	priv->match_words = match_words;
}


static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_SEARCH_QUERY_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaSearchQueryWoPrivate *priv = cha_search_query_wo_get_instance_private((ChaSearchQueryWo *) e_uninitialized);
	if (original) {
		ChaSearchQueryWoPrivate *rpriv = cha_search_query_wo_get_instance_private((ChaSearchQueryWo *) original);
		priv->a_text = cat_string_wo_clone(rpriv->a_text, CAT_CLONE_DEPTH_MAIN);
		priv->match_case = rpriv->match_case;
		priv->match_words = rpriv->match_words;
		priv->regexp = rpriv->regexp;
	} else {
		priv->a_text = NULL;
		priv->match_case = FALSE;
		priv->match_words = FALSE;
		priv->regexp = FALSE;
	}
	return CAT_WO_CLASS(cha_search_query_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const ChaSearchQueryWoPrivate *priv_a = cha_search_query_wo_get_instance_private((ChaSearchQueryWo *) wo_a);
	const ChaSearchQueryWoPrivate *priv_b = cha_search_query_wo_get_instance_private((ChaSearchQueryWo *) wo_b);
	return (cat_string_wo_equal(priv_a->a_text, priv_b->a_text) &&
					(priv_a->match_case == priv_b->match_case) &&
					(priv_a->match_words == priv_b->match_words) &&
					(priv_a->regexp == priv_b->regexp));
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_SEARCH_QUERY_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaSearchQueryWoPrivate *priv = cha_search_query_wo_get_instance_private(CHA_SEARCH_QUERY_WO(e_uninitialized));
	if (wo_source) {
		ChaSearchQueryWoPrivate *priv_src = cha_search_query_wo_get_instance_private(CHA_SEARCH_QUERY_WO(wo_source));
		priv->a_text = cat_string_wo_clone(priv_src->a_text, CAT_CLONE_DEPTH_NONE);
		priv->match_case = priv_src->match_case;
		priv->match_words = priv_src->match_words;
		priv->regexp = priv_src->regexp;
	} else {
		priv->a_text = NULL;
		priv->match_case = FALSE;
		priv->match_words = FALSE;
		priv->regexp = FALSE;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cha_search_query_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}


CAT_WO_BASE_C(ChaSearchQueryWo,cha_search_query_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaSearchQueryWo *instance = CHA_SEARCH_QUERY_WO(self);
	ChaSearchQueryWoPrivate *priv = cha_search_query_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p(%d): %s text=%o]", iname, self, cat_wo_get_version((CatWo *) self), cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable", priv->a_text);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

