/*
   File:    dralinetagwo.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jul 7, 2015
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

#include "dralinetagwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraLineTagWo"
#include <logging/catlog.h>

struct _DraLineTagWoPrivate {
	long long row;
	DraTagType tag_type;
	int start_index;
	int end_index;
	double red, green, blue;
	CatStringWo *text;
	GObject *extra_data;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraLineTagWo, dra_line_tag_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(DraLineTagWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);


static void dra_line_tag_wo_class_init(DraLineTagWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_line_tag_wo_init(DraLineTagWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraLineTagWo *instance = DRA_LINE_TAG_WO(object);
	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private(instance);
	cat_unref_ptr(priv->text);
	cat_unref_ptr(priv->extra_data);
	G_OBJECT_CLASS(dra_line_tag_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_line_tag_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraLineTagWo *dra_line_tag_wo_new(long long row, DraTagType tag_type) {
	DraLineTagWo *result = g_object_new(DRA_TYPE_LINE_TAG_WO, NULL);
	cat_ref_anounce(result);
	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->tag_type = tag_type;
	priv->row = row;
	priv->end_index = -1;
	priv->start_index = -1;
	return result;
}

long long dra_line_tag_wo_get_row(DraLineTagWo *line_tag) {
	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private(line_tag);
	return priv->row;
}

DraTagType dra_line_tag_wo_get_tag_type(DraLineTagWo *line_tag) {
	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private(line_tag);
	return priv->tag_type;
}

void dra_line_tag_wo_get_color(DraLineTagWo *line_tag, double *red, double *green, double *blue) {
	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private(line_tag);
	*red = priv->red;
	*green = priv->green;
	*blue = priv->blue;
}

void dra_line_tag_wo_get_start_and_end_index(DraLineTagWo *line_tag, int *start, int *end) {
	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private(line_tag);
	*start = priv->start_index;
	*end = priv->end_index;
}


CatStringWo *dra_line_tag_wo_get_text(DraLineTagWo *line_tag) {
	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private(line_tag);
	return priv->text;
}


GObject *dra_line_tag_wo_get_extra_data(DraLineTagWo *line_tag) {
	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private(line_tag);
	return priv->extra_data;
}

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_line_tag)) { \
			cat_log_error("Object is read only:%o", e_line_tag); \
			return rval; \
		} \

void dra_line_tag_wo_set_end_index(DraLineTagWo *e_line_tag, int right) {
	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private(e_line_tag);
	CHECK_IF_WRITABLE();
	priv->end_index = right;
}

void dra_line_tag_wo_set_start_and_end_index(DraLineTagWo *e_line_tag, int left, int right) {
	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private(e_line_tag);
	CHECK_IF_WRITABLE();
	priv->start_index = left;
	priv->end_index = right;
}

void dra_line_tag_wo_set_color(DraLineTagWo *e_line_tag, double red, double green, double blue) {
	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private(e_line_tag);
	CHECK_IF_WRITABLE();
	priv->red = red;
	priv->green = green;
	priv->blue = blue;
}

void dra_line_tag_wo_set_text(DraLineTagWo *e_line_tag, CatStringWo *text) {
	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private(e_line_tag);
	CHECK_IF_WRITABLE();
	cat_unref_ptr(priv->text);
	priv->text = cat_string_wo_clone(text, CAT_CLONE_DEPTH_AS_ANCHORED);
}

void dra_line_tag_wo_set_extra_data(DraLineTagWo *e_line_tag, GObject *extra_data) {
	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private(e_line_tag);
	CHECK_IF_WRITABLE();
	cat_ref_swap(priv->extra_data, extra_data);
}

static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(DRA_TYPE_LINE_TAG_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private((DraLineTagWo *) e_uninitialized);
	if (original) {
		DraLineTagWoPrivate *rpriv = dra_line_tag_wo_get_instance_private((DraLineTagWo *) original);
		priv->row = rpriv->row;
		priv->start_index = rpriv->start_index;
		priv->end_index = rpriv->end_index;
		priv->red = rpriv->red;
		priv->blue = rpriv->blue;
		priv->green = rpriv->green;
		priv->text = cat_ref_ptr(rpriv->text);
		priv->extra_data = cat_ref_ptr(rpriv->extra_data);
		priv->tag_type = rpriv->tag_type;
	} else {
		priv->row = -1;
		priv->start_index = -1;
		priv->end_index = -1;
		priv->red = 0;
		priv->blue = 0;
		priv->green = 0;
		priv->text = NULL;
		priv->extra_data = NULL;
		priv->tag_type = DRA_TAG_TYPE_PARSER_ERROR;
	}
	return CAT_WO_CLASS(dra_line_tag_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const DraLineTagWoPrivate *priv_a = dra_line_tag_wo_get_instance_private((DraLineTagWo *) wo_a);
	const DraLineTagWoPrivate *priv_b = dra_line_tag_wo_get_instance_private((DraLineTagWo *) wo_b);
	return (priv_a->row==priv_b->row)
				&& (priv_a->start_index==priv_b->start_index)
				&& (priv_a->end_index==priv_b->end_index)
				&& (priv_a->red==priv_b->red)
				&& (priv_a->green==priv_b->green)
				&& (priv_a->blue==priv_b->blue)
				&& (cat_string_wo_equal(priv_a->text, priv_b->text)
				&& (priv_a->tag_type==priv_b->tag_type)
				&& (priv_a->extra_data==priv_b->extra_data));
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(DRA_TYPE_LINE_TAG_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private(DRA_LINE_TAG_WO(e_uninitialized));
	if (wo_source) {
		DraLineTagWoPrivate *priv_src = dra_line_tag_wo_get_instance_private(DRA_LINE_TAG_WO(wo_source));
		priv->row = priv_src->row;
		priv->start_index = priv_src->start_index;
		priv->end_index = priv_src->end_index;
		priv->red = priv_src->red;
		priv->green = priv_src->green;
		priv->blue = priv_src->blue;
		priv->text = cat_string_wo_clone(priv_src->text, CAT_CLONE_DEPTH_NONE);
		priv->tag_type = priv_src->tag_type;
		if (CAT_IS_WO(priv_src->extra_data)) {
			priv->extra_data = (GObject *) cat_wo_clone((CatWo *) priv_src->extra_data, CAT_CLONE_DEPTH_NONE);
		} else {
			priv->extra_data = cat_ref_ptr(priv_src->extra_data);
		}
	} else {
		priv->row = -1;
		priv->start_index = -1;
		priv->end_index = -1;
		priv->red = 0;
		priv->blue = 0;
		priv->green = 0;
		priv->text = NULL;
		priv->extra_data = NULL;
		priv->tag_type = DRA_TAG_TYPE_PARSER_ERROR;
	}

	CatWoClass *wocls = CAT_WO_CLASS(dra_line_tag_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}


CAT_WO_BASE_C(DraLineTagWo,dra_line_tag_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraLineTagWo *instance = DRA_LINE_TAG_WO(self);
	DraLineTagWoPrivate *priv = dra_line_tag_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p(%d): %s, start/end=%d/%d, row=%d, text=%O]", iname, self, cat_wo_get_version((CatWo *) self), cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable", priv->start_index, priv->end_index, priv->row, priv->text);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
