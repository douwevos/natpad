/*
   File:    chaformfieldwo.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Aug 7, 2015
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

#include "chaformfieldwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaFormFieldWo"
#include <logging/catlog.h>

struct _ChaFormFieldWoPrivate {
	ChaCursorMWo *a_start_cursor;
	ChaCursorMWo *a_end_cursor;
	int field_index;
	int valid_hash;
	gboolean editable;
	gboolean marked_modified;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaFormFieldWo, cha_form_field_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(ChaFormFieldWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);


static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);


static void cha_form_field_wo_class_init(ChaFormFieldWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_form_field_wo_init(ChaFormFieldWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaFormFieldWo *instance = CHA_FORM_FIELD_WO(object);
	ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private(instance);
	cat_unref_ptr(priv->a_start_cursor);
	cat_unref_ptr(priv->a_end_cursor);
	G_OBJECT_CLASS(cha_form_field_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_form_field_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaFormFieldWo *cha_form_field_wo_new(ChaCursorMWo *a_start_cursor, ChaCursorMWo *a_end_cursor, int field_index, gboolean editable) {
	ChaFormFieldWo *result = g_object_new(CHA_TYPE_FORM_FIELD_WO, NULL);
	cat_ref_anounce(result);
	ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->a_start_cursor = cha_cursorm_wo_clone(a_start_cursor, CAT_CLONE_DEPTH_AS_ANCHORED);
	priv->a_end_cursor = cha_cursorm_wo_clone(a_end_cursor, CAT_CLONE_DEPTH_AS_ANCHORED);
	priv->field_index = field_index;
	priv->editable = editable;
	priv->marked_modified = FALSE;
	return result;
}

ChaCursorMWo *cha_form_field_wo_get_start(const ChaFormFieldWo *field) {
	const ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private((ChaFormFieldWo *) field);
	return priv->a_start_cursor;
}

ChaCursorMWo *cha_form_field_wo_get_end(const ChaFormFieldWo *field) {
	const ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private((ChaFormFieldWo *) field);
	return priv->a_end_cursor;
}

int cha_form_field_wo_get_field_index(const ChaFormFieldWo *field) {
	const ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private((ChaFormFieldWo *) field);
	return priv->field_index;
}

gboolean cha_form_field_wo_is_marked_as_modified(const ChaFormFieldWo *field) {
	const ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private((ChaFormFieldWo *) field);
	return priv->marked_modified;
}

void cha_form_field_wo_mark_as_modified(ChaFormFieldWo *field) {
	ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private((ChaFormFieldWo *) field);
	priv->marked_modified = TRUE;
}

gboolean cha_form_field_wo_on_row(const ChaFormFieldWo *field, int row) {
	const ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private((ChaFormFieldWo *) field);
	unsigned long long int start_row = cha_cursorm_wo_get_row(priv->a_start_cursor);
	unsigned long long int end_row = cha_cursorm_wo_get_row(priv->a_end_cursor);
	if (start_row==row) {
		return TRUE;
	} else if (end_row==row) {
		return TRUE;
	}
	if (start_row<row && end_row>row) {
		return TRUE;
	}
	return FALSE;
}

gboolean cha_form_field_wo_test_cursor_in_field(const ChaFormFieldWo *field, const ChaCursorMWo *cursor) {
	const ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private((ChaFormFieldWo *) field);
	ChaCursorOrder ora = cha_cursorm_wo_order(priv->a_start_cursor, cursor);
	cat_log_debug("ora=%d", ora);
	if (ora==CHA_CURSOR_SAME || ora==CHA_CURSOR_RIGHT|| ora==CHA_CURSOR_BELOW) {
		ChaCursorOrder orb = cha_cursorm_wo_order(priv->a_end_cursor, cursor);
		cat_log_debug("orb=%d", orb);
		if (orb==CHA_CURSOR_SAME || orb==CHA_CURSOR_LEFT || orb==CHA_CURSOR_ABOVE) {
			return TRUE;
		}
	}
	return FALSE;
}

ChaFormFieldCoverage cha_form_field_wo_test_coverage(const ChaFormFieldWo *field, const ChaCursorMWo *other_start, const ChaCursorMWo *other_end) {
	const ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private((ChaFormFieldWo *) field);
	ChaCursorOrder ora_start = cha_cursorm_wo_order(priv->a_start_cursor, other_start);

	ChaCursorOrder ora_end = cha_cursorm_wo_order(priv->a_end_cursor, other_end);

	cat_log_debug("ora_start=%s, ora_end=%s", cha_cursorm_wo_order_text(ora_start), cha_cursorm_wo_order_text(ora_end));

	if (ora_start==CHA_CURSOR_SAME && ora_end==CHA_CURSOR_SAME) {
		return CHA_FFC_SAME_SAME;
	}

	if ((ora_start==CHA_CURSOR_RIGHT || ora_start==CHA_CURSOR_BELOW || ora_start==CHA_CURSOR_SAME)
			&& (ora_end==CHA_CURSOR_ABOVE || ora_end==CHA_CURSOR_LEFT || ora_end==CHA_CURSOR_SAME)) {
		return CHA_FFC_OVERLAPS_FULL;
	}

	if ((ora_start==CHA_CURSOR_LEFT || ora_start==CHA_CURSOR_ABOVE || ora_start==CHA_CURSOR_SAME)
			&& (ora_end==CHA_CURSOR_RIGHT || ora_end==CHA_CURSOR_BELOW || ora_end==CHA_CURSOR_SAME)) {
		return CHA_FFC_IS_OVERLAPPED_FULL;
	}

	ChaCursorOrder ora_es = cha_cursorm_wo_order(priv->a_end_cursor, other_start);
	if ((ora_es == CHA_CURSOR_LEFT || ora_es==CHA_CURSOR_ABOVE) && (ora_end==CHA_CURSOR_RIGHT || ora_end==CHA_CURSOR_BELOW || ora_end==CHA_CURSOR_SAME)) {
		return CHA_FFC_RIGHT_OVERLAPPED;
	}


	ora_es = cha_cursorm_wo_order(priv->a_start_cursor, other_end);
	if ((ora_es == CHA_CURSOR_RIGHT || ora_es==CHA_CURSOR_BELOW) && (ora_start==CHA_CURSOR_ABOVE || ora_start==CHA_CURSOR_LEFT || ora_start==CHA_CURSOR_SAME)) {
		return CHA_FFC_LEFT_OVERLAPPED;
	}


	return CHA_FFC_NO_OVERLAP;
}


#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_field)) { \
			cat_log_error("Object is read only:%o", e_field); \
			return rval; \
		} \


void cha_form_field_wo_set_start_and_end(ChaFormFieldWo *e_field, ChaCursorMWo *start_cursor, ChaCursorMWo *end_cursor) {
	ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private(e_field);
	CHECK_IF_WRITABLE();
	if (priv->a_start_cursor!=start_cursor) {
		start_cursor = cha_cursorm_wo_clone(start_cursor, CAT_CLONE_DEPTH_AS_ANCHORED);
		cat_unref_ptr(priv->a_start_cursor);
		priv->a_start_cursor = start_cursor;
	}
	if (priv->a_end_cursor!=end_cursor) {
		end_cursor = cha_cursorm_wo_clone(end_cursor, CAT_CLONE_DEPTH_AS_ANCHORED);
		cat_unref_ptr(priv->a_end_cursor);
		priv->a_end_cursor = end_cursor;
	}
}

int cha_form_field_wo_hash(const ChaFormFieldWo *field) {
	const ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private((ChaFormFieldWo *) field);
	int hash = priv->editable ? 13 : 3;
	hash += priv->field_index*7;
	hash += cha_cursorm_wo_hash(priv->a_end_cursor)*367;
	hash += cha_cursorm_wo_hash(priv->a_start_cursor)*653;
	return hash;
}


static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_FORM_FIELD_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private((ChaFormFieldWo *) e_uninitialized);
	if (original) {
		ChaFormFieldWoPrivate *rpriv = cha_form_field_wo_get_instance_private((ChaFormFieldWo *) original);
		priv->a_start_cursor = cat_ref_ptr(rpriv->a_start_cursor);
		priv->a_end_cursor = cat_ref_ptr(rpriv->a_end_cursor);
		priv->editable = rpriv->editable;
		priv->field_index = rpriv->field_index;
		priv->marked_modified = rpriv->marked_modified;
	} else {
		priv->a_start_cursor = NULL;
		priv->a_end_cursor = NULL;
		priv->editable = FALSE;
		priv->field_index = 0;
		priv->marked_modified = FALSE;
	}
	return CAT_WO_CLASS(cha_form_field_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}



static void l_anchor_children(CatWo *wo, int version) {
	ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private((ChaFormFieldWo *) wo);
	priv->marked_modified = FALSE;
}

static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const ChaFormFieldWoPrivate *priv_a = cha_form_field_wo_get_instance_private((ChaFormFieldWo *) wo_a);
	const ChaFormFieldWoPrivate *priv_b = cha_form_field_wo_get_instance_private((ChaFormFieldWo *) wo_b);
	return (priv_a->editable==priv_b->editable)
			&& (priv_a->field_index==priv_b->field_index)
			&& (cha_cursorm_wo_equal(priv_a->a_start_cursor, priv_b->a_start_cursor))
			&& (cha_cursorm_wo_equal(priv_a->a_end_cursor, priv_b->a_end_cursor));
}


static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_FORM_FIELD_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private(CHA_FORM_FIELD_WO(e_uninitialized));
	if (wo_source) {
		ChaFormFieldWoPrivate *priv_src = cha_form_field_wo_get_instance_private(CHA_FORM_FIELD_WO(wo_source));
		priv->a_start_cursor =cha_cursorm_wo_clone(priv_src->a_start_cursor, CAT_CLONE_DEPTH_NONE);
		priv->a_end_cursor = cha_cursorm_wo_clone(priv_src->a_end_cursor, CAT_CLONE_DEPTH_NONE);
		priv->editable = priv_src->editable;
		priv->field_index = priv_src->field_index;
		priv->marked_modified = priv_src->marked_modified;
	} else {
		priv->a_start_cursor = NULL;
		priv->a_end_cursor = NULL;
		priv->editable = FALSE;
		priv->field_index = 0;
		priv->marked_modified = FALSE;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cha_form_field_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}


CAT_WO_BASE_C(ChaFormFieldWo,cha_form_field_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaFormFieldWo *instance = CHA_FORM_FIELD_WO(self);
	ChaFormFieldWoPrivate *priv = cha_form_field_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p(%d): %s, field_index=%d, start=%o, end=%o, %s]", iname, self, cat_wo_get_version((CatWo *) self), cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable", priv->field_index, priv->a_start_cursor, priv->a_end_cursor, priv->marked_modified ? "marked" : "not-marked");
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
