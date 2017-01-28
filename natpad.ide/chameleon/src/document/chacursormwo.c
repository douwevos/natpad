/*
   File:    chacursormwo.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Aug 10, 2015
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

#include "chacursormwo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaCursorMWo"
#include <logging/catlog.h>

struct _ChaCursorMWoPrivate {
	long long row;
	int column;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaCursorMWo, cha_cursorm_wo, CAT_TYPE_WO,
		G_ADD_PRIVATE(ChaCursorMWo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info);
static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b);
static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source);


static void cha_cursorm_wo_class_init(ChaCursorMWoClass *clazz) {
	CatWoClass *wo_class = CAT_WO_CLASS(clazz);
	wo_class->equal = l_equal;
	wo_class->constructEditable = l_construct_editable;
	wo_class->cloneContent = l_clone_content;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_cursorm_wo_init(ChaCursorMWo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(cha_cursorm_wo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_cursorm_wo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ChaCursorMWo *cha_cursorm_wo_new(long long row, int column) {
	ChaCursorMWo *result = g_object_new(CHA_TYPE_CURSORM_WO, NULL);
	cat_ref_anounce(result);
	ChaCursorMWoPrivate *priv = cha_cursorm_wo_get_instance_private(result);
	cat_wo_construct((CatWo *) result, TRUE);
	priv->row = row;
	priv->column = column;
	return result;
}

int cha_cursorm_wo_get_column(const ChaCursorMWo *cursor) {
	const ChaCursorMWoPrivate *priv = cha_cursorm_wo_get_instance_private((ChaCursorMWo *) cursor);
	return priv->column;
}

long long cha_cursorm_wo_get_row(const ChaCursorMWo *cursor) {
	const ChaCursorMWoPrivate *priv = cha_cursorm_wo_get_instance_private((ChaCursorMWo *) cursor);
	return priv->row;
}

ChaCursorOrder cha_cursorm_wo_order(const ChaCursorMWo *cursor, const ChaCursorMWo *other) {
	const ChaCursorMWoPrivate *priv = cha_cursorm_wo_get_instance_private((ChaCursorMWo *) cursor);
	const ChaCursorMWoPrivate *opriv = cha_cursorm_wo_get_instance_private((ChaCursorMWo *) other);
	if (opriv->row<priv->row) {
		return CHA_CURSOR_ABOVE;
	} else if (opriv->row>priv->row) {
		return CHA_CURSOR_BELOW;
	} else {
		if (opriv->column < priv->column) {
			return CHA_CURSOR_LEFT;
		} else if (opriv->column>priv->column) {
			return CHA_CURSOR_RIGHT;
		}
	}
	return CHA_CURSOR_SAME;
}

const char *cha_cursorm_wo_order_text(ChaCursorOrder order) {
	switch(order) {
	case CHA_CURSOR_ABOVE : return "CURSOR_ABOVE";
	case CHA_CURSOR_LEFT : return "CURSOR_LEFT";
	case CHA_CURSOR_SAME : return "CURSOR_SAME";
	case CHA_CURSOR_RIGHT : return "CURSOR_RIGHT";
	case CHA_CURSOR_BELOW : return "CURSOR_BELOW";
	}
	return "<INVALID>";
}



int cha_cursorm_wo_hash(const ChaCursorMWo *cursor) {
	int result = -1;
	if (cursor) {
		const ChaCursorMWoPrivate *priv = cha_cursorm_wo_get_instance_private((ChaCursorMWo *) cursor);
		result = ((int) priv->row) + ((int) (priv->row>>32));
		result += priv->column*997;
	}
	return result;
}


#define CHECK_IF_WRITABLE(rval) \
		if (cat_wo_is_anchored((CatWo *) e_cursor)) { \
			cat_log_error("Object is read only:%o", e_cursor); \
			return rval; \
		} \

void cha_cursorm_wo_set_column(ChaCursorMWo *e_cursor, int column) {
	ChaCursorMWoPrivate *priv = cha_cursorm_wo_get_instance_private(e_cursor);
	CHECK_IF_WRITABLE();
	priv->column = column;
}

void cha_cursorm_wo_set_row(ChaCursorMWo *e_cursor, long long row) {
	ChaCursorMWoPrivate *priv = cha_cursorm_wo_get_instance_private(e_cursor);
	CHECK_IF_WRITABLE();
	priv->row = row;
}

void cha_cursorm_wo_move(ChaCursorMWo *e_cursor, long long delta_row, int delta_column) {
	ChaCursorMWoPrivate *priv = cha_cursorm_wo_get_instance_private(e_cursor);
	CHECK_IF_WRITABLE();
	priv->row += delta_row;
	priv->column += delta_column;
}


static CatWo *l_construct_editable(CatWo *e_uninitialized, CatWo *original, struct CatWoInfo *info) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_CURSORM_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaCursorMWoPrivate *priv = cha_cursorm_wo_get_instance_private((ChaCursorMWo *) e_uninitialized);
	if (original) {
		ChaCursorMWoPrivate *rpriv = cha_cursorm_wo_get_instance_private((ChaCursorMWo *) original);
		priv->row = rpriv->row;
		priv->column = rpriv->column;
	} else {
		priv->row = 0;
		priv->column = 0;
	}
	return CAT_WO_CLASS(cha_cursorm_wo_parent_class)->constructEditable(e_uninitialized, original, info);
}


static gboolean l_equal(const CatWo *wo_a, const CatWo *wo_b) {
	if (wo_a==wo_b) {
		return TRUE;
	} else if (wo_a==NULL || wo_b==NULL) {
		return FALSE;
	}
	const ChaCursorMWoPrivate *priv_a = cha_cursorm_wo_get_instance_private((ChaCursorMWo *) wo_a);
	const ChaCursorMWoPrivate *priv_b = cha_cursorm_wo_get_instance_private((ChaCursorMWo *) wo_b);
	return (priv_a->row==priv_b->row) && (priv_a->column==priv_b->column);
}

static CatWo *l_clone_content(CatWo *e_uninitialized, const CatWo *wo_source) {
	if (e_uninitialized==NULL) {
		e_uninitialized = g_object_new(CHA_TYPE_CURSORM_WO, NULL);
		cat_ref_anounce(e_uninitialized);
	}

	ChaCursorMWoPrivate *priv = cha_cursorm_wo_get_instance_private(CHA_CURSORM_WO(e_uninitialized));
	if (wo_source) {
		ChaCursorMWoPrivate *priv_src = cha_cursorm_wo_get_instance_private(CHA_CURSORM_WO(wo_source));
		priv->column = priv_src->column;
		priv->row = priv_src->row;
	} else {
		priv->column = 0;
		priv->row = 0;
	}

	CatWoClass *wocls = CAT_WO_CLASS(cha_cursorm_wo_parent_class);
	if (wocls->cloneContent) {
		return wocls->cloneContent(e_uninitialized, wo_source);
	}
	return e_uninitialized;
}


CAT_WO_BASE_C(ChaCursorMWo,cha_cursorm_wo);

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaCursorMWo *instance = CHA_CURSORM_WO(self);
	ChaCursorMWoPrivate *priv = cha_cursorm_wo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p(%d - rc:%d): %s, row=%d, col=%d]", iname, self, cat_wo_get_version((CatWo *) self), ((GObject *) self)->ref_count, cat_wo_is_anchored((CatWo *) self) ? "anchored" : "editable", priv->row, priv->column);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
