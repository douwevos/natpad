/*
   File:    chacursorwo.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Feb 20, 2015
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

#include "chacursorwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaCursorWo"
#include <logging/catlog.h>

struct _ChaCursorWoPrivate {
	int x_cursor_bytes;
	int x_sub;
	ChaLineLocationWo *line_location;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaCursorWo, cha_cursor_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(ChaCursorWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static void l_anchor_children(CatWo *wo, int version);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);

static void cha_cursor_wo_class_init(ChaCursorWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->anchorChildren = l_anchor_children;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_cursor_wo_init(ChaCursorWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaCursorWo *instance = CHA_CURSOR_WO(object);
	ChaCursorWoPrivate *priv = cha_cursor_wo_get_instance_private(instance);
	cat_unref_ptr(priv->line_location);
	G_OBJECT_CLASS(cha_cursor_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_cursor_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaCursorWo *cha_cursor_wo_new() {
	ChaCursorWo *result = g_object_new(CHA_TYPE_CURSOR_WO, NULL);
	cat_ref_anounce(result);
	ChaCursorWoPrivate *priv = cha_cursor_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->x_cursor_bytes = 0;
	priv->line_location = cha_line_location_wo_new();
	priv->x_sub = 0;
	return result;
}

ChaCursorWo *cha_cursor_wo_new_ll_offset(ChaLineLocationWo *line_location, int x_cursor) {
	ChaCursorWo *result = g_object_new(CHA_TYPE_CURSOR_WO, NULL);
	cat_ref_anounce(result);
	ChaCursorWoPrivate *priv = cha_cursor_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->x_cursor_bytes = x_cursor;
	priv->line_location = cat_ref_ptr(line_location);
	priv->x_sub = 0;
	return result;
}


int cha_cursor_wo_get_x_cursor_bytes(const ChaCursorWo *cursor) {
	const ChaCursorWoPrivate *priv = cha_cursor_wo_get_instance_private((ChaCursorWo *) cursor);
	return priv->x_cursor_bytes;
}


int cha_cursor_wo_get_x_sub(const ChaCursorWo *cursor) {
	const ChaCursorWoPrivate *priv = cha_cursor_wo_get_instance_private((ChaCursorWo *) cursor);
	return priv->x_sub;
}

ChaLineLocationWo *cha_cursor_wo_get_line_location(const ChaCursorWo *cursor) {
	const ChaCursorWoPrivate *priv = cha_cursor_wo_get_instance_private((ChaCursorWo *) cursor);
	return priv->line_location;
}

#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_cursor)) { \
			cat_log_error("Object is read only:%o", e_cursor); \
			return rval; \
		} \


ChaLineLocationWo *cha_cursor_wo_get_editable_line_location(ChaCursorWo *e_cursor) {
	ChaCursorWoPrivate *priv = cha_cursor_wo_get_instance_private(e_cursor);
	CHECK_IF_WRITABLE(NULL)
	if (cha_line_location_wo_is_anchored(priv->line_location)) {
		ChaLineLocationWo *nll = cha_line_location_wo_create_editable(priv->line_location);
		cat_unref_ptr(priv->line_location);
		priv->line_location = nll;
	}
	return priv->line_location;
}

void cha_cursor_wo_set_x_cursor_bytes(ChaCursorWo *e_cursor, int x_cursor_bytes) {
	ChaCursorWoPrivate *priv = cha_cursor_wo_get_instance_private(e_cursor);
	CHECK_IF_WRITABLE()
	priv->x_cursor_bytes = x_cursor_bytes;
}


void cha_cursor_wo_set_x_sub(ChaCursorWo *e_cursor, int x_sub) {
	ChaCursorWoPrivate *priv = cha_cursor_wo_get_instance_private(e_cursor);
	CHECK_IF_WRITABLE()
	priv->x_sub = x_sub;
}


void cha_cursor_wo_set_line_location(ChaCursorWo *e_cursor, ChaLineLocationWo *location) {
	ChaCursorWoPrivate *priv = cha_cursor_wo_get_instance_private(e_cursor);
	CHECK_IF_WRITABLE()
	cat_ref_swap(priv->line_location, location);
}


ChaCursorOrder cha_cursor_wo_compare(const ChaCursorWo *instance_a, const ChaCursorWo *instance_b) {
	if (instance_a==instance_b) {
		return CHA_CURSOR_SAME;
	} else if (instance_a==NULL || instance_b==NULL) {
		return CHA_CURSOR_SAME;
	}
	const ChaCursorWoPrivate *priv_a = cha_cursor_wo_get_instance_private((ChaCursorWo *) instance_a);
	const ChaCursorWoPrivate *priv_b = cha_cursor_wo_get_instance_private((ChaCursorWo *) instance_b);
	int result = cha_line_location_wo_compare(priv_a->line_location, priv_b->line_location);
	if (result<0) {
		return CHA_CURSOR_ABOVE;
	} else if (result>0) {
		return CHA_CURSOR_BELOW;
	}

	if (priv_a->x_cursor_bytes<priv_b->x_cursor_bytes) {
		return CHA_CURSOR_LEFT;
	} else if (priv_a->x_cursor_bytes>priv_b->x_cursor_bytes) {
		return CHA_CURSOR_RIGHT;
	}
	if (priv_a->x_sub<priv_b->x_sub) {
		return CHA_CURSOR_LEFT;
	} else if (priv_a->x_sub>priv_b->x_sub) {
		return CHA_CURSOR_RIGHT;
	}
	return CHA_CURSOR_SAME;
}


static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_CURSOR_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaCursorWoPrivate *priv = cha_cursor_wo_get_instance_private((ChaCursorWo *) e_uninitialized);
	if (original) {
		ChaCursorWoPrivate *rpriv = cha_cursor_wo_get_instance_private((ChaCursorWo *) original);
		priv->line_location = cat_ref_ptr(rpriv->line_location);
		priv->x_cursor_bytes = rpriv->x_cursor_bytes;
		priv->x_sub = rpriv->x_sub;
	} else {
		priv->line_location = NULL;
		priv->x_cursor_bytes = -1;
		priv->x_sub = 0;
	}
	return CAT_WO_CLASS(cha_cursor_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static void l_anchor_children(CatWo *wo, int version) {
	ChaCursorWoPrivate *priv = cha_cursor_wo_get_instance_private((ChaCursorWo *) wo);
	priv->line_location = cha_line_location_wo_anchor(priv->line_location, version);
}

static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const ChaCursorWoPrivate *priv_a = cha_cursor_wo_get_instance_private((ChaCursorWo *) wo_a);
	const ChaCursorWoPrivate *priv_b = cha_cursor_wo_get_instance_private((ChaCursorWo *) wo_b);
	return (priv_a->x_cursor_bytes == priv_b->x_cursor_bytes)
			&& (priv_a->x_sub == priv_b->x_sub)
			&& cha_line_location_wo_equal(priv_a->line_location, priv_b->line_location);
}


static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_CURSOR_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaCursorWoPrivate *priv = cha_cursor_wo_get_instance_private(CHA_CURSOR_WO(e_uninitialized));
	if (wo_source) {
		ChaCursorWoPrivate *priv_src = cha_cursor_wo_get_instance_private(CHA_CURSOR_WO(wo_source));
		priv->line_location = cha_line_location_wo_clone(priv_src->line_location, CAT_CLONE_DEPTH_NONE);
		priv->x_cursor_bytes = priv_src->x_cursor_bytes;
		priv->x_sub = priv_src->x_sub;
	} else {
		priv->line_location = NULL;
		priv->x_cursor_bytes = -1;
		priv->x_sub = 0;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cha_cursor_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}


CAT_WO_BASE_C(ChaCursorWo,cha_cursor_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaCursorWo *instance = CHA_CURSOR_WO(self);
	ChaCursorWoPrivate *priv = cha_cursor_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p: %s, x_cursor_bytes=%d, sub=%d, %o]", iname, self, cat_wo_is_anchored((CatWo *) instance) ? "anchored" : "editable", priv->x_cursor_bytes, priv->x_sub, priv->line_location);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
