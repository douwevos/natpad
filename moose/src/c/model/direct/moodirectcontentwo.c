/*
   File:    moodirectcontentwo.c
   Project: moose
   Author:  Douwe Vos
   Date:    Oct 4, 2015
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

#include "moodirectcontentwo.h"
#include "../mooicontent.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "MooDirectContentWo"
#include <logging/catlog.h>

struct _MooDirectContentWoPrivate {
	void *dummy;
};


static CatS moo_s_direct_content_key = CAT_S_DEF("MooDirectContentWo");

static void l_content_iface_init(MooIContentInterface *iface);
static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(MooDirectContentWo, moo_direct_content_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(MooDirectContentWo)
		G_IMPLEMENT_INTERFACE(MOO_TYPE_ICONTENT, l_content_iface_init);
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);


static void moo_direct_content_wo_class_init(MooDirectContentWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void moo_direct_content_wo_init(MooDirectContentWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(moo_direct_content_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(moo_direct_content_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


MooDirectContentWo *moo_direct_content_wo_new() {
	MooDirectContentWo *result = g_object_new(MOO_TYPE_DIRECT_CONTENT_WO, NULL);
	cat_ref_anounce(result);
//	MooDirectContentWoPrivate *priv = moo_direct_content_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	return result;
}

static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(MOO_TYPE_DIRECT_CONTENT_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}
	return CAT_WO_CLASS(moo_direct_content_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}



static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	return TRUE;
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(MOO_TYPE_DIRECT_CONTENT_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

//	MooDirectContentWoPrivate *priv = moo_direct_content_wo_get_instance_private(CHA_FORM_WO(e_uninitialized));
//	if (wo_source) {
//		MooDirectContentWoPrivate *priv_src = moo_direct_content_wo_get_instance_private(CHA_FORM_WO(wo_source));
//		priv->fields = cat_wo_clone(priv_src->fields, CAT_CLONE_DEPTH_NONE);
//	} else {
//		priv->fields = cat_array_wo_new();
//	}

	CatWoClass *wocls = CAT_WO_CLASS(moo_direct_content_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}


CAT_WO_BASE_C(MooDirectContentWo,moo_direct_content_wo);

CatStringWo *moo_direct_content_wo_key() {
	return CAT_S(moo_s_direct_content_key);
}


/********************* start MooIContent implementation *********************/



static CatStringWo *l_content_get_key(MooIContent *self) {
	return moo_direct_content_wo_key();
}

static MooIContent *l_content_anchor(MooIContent *self, int version) {
	return (MooIContent *) moo_direct_content_wo_anchor((MooDirectContentWo *) self, version);
}


static void l_content_iface_init(MooIContentInterface *iface) {
	iface->getKey = l_content_get_key;
	iface->anchor = l_content_anchor;
}

/********************* end MooIContent implementation *********************/

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	MooDirectContentWo *instance = MOO_DIRECT_CONTENT_WO(self);
//	MooDirectContentWoPrivate *priv = moo_direct_content_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p(%d): %s]", iname, self, cat_wo_get_version((CatWo *) self), cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable");
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
