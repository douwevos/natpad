/*
   File:    chaselection.c
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

#include "chaselectionprivate.h"
#include "../layout/chapangoattrholder.h"

CatS cha_selection_cd_plain_text = CAT_S_DEF("PLAINTEXT");
CatS cha_selection_cd_rich_text = CAT_S_DEF("RICHTEXT");
CatS cha_selection_cd_block_text = CAT_S_DEF("NATPADBLOCK");


#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaSelection"
#include <logging/catlog.h>

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaSelection, cha_selection, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaSelection)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_selection_class_init(ChaSelectionClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_selection_init(ChaSelection *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaSelection *instance = CHA_SELECTION(object);
	ChaSelectionPrivate *priv = cha_selection_get_instance_private(instance);
	cat_unref_ptr(priv->a_start);
	cat_unref_ptr(priv->a_end);
	cat_unref_ptr(priv->sequence);
	G_OBJECT_CLASS(cha_selection_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_selection_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void cha_selection_construct(ChaSelection *selection, CatAtomicInteger *sequence, ChaCursorWo *cursor) {
	ChaSelectionPrivate *priv = cha_selection_get_instance_private(selection);
	cursor = cha_cursor_wo_clone(cursor, CAT_CLONE_DEPTH_NONE);
	cursor = cha_cursor_wo_anchor(cursor, 0);
	priv->a_start = cursor;
	priv->a_end = cat_ref_ptr(cursor);
	priv->sequence = cat_ref_ptr(sequence);
	priv->modification_count = cat_atomic_integer_increment(sequence);
}

ChaSelectionPrivate *cha_selection_get_protected(ChaSelection *selection) {
	return cha_selection_get_instance_private(selection);
}


static void l_selection_order(ChaSelectionPrivate *priv) {
	ChaLineLocationWo *a_location = cha_cursor_wo_get_line_location(priv->a_start);
	int top_page_idx = cha_line_location_wo_get_page_index(a_location);
	int top_page_line_idx = cha_line_location_wo_get_page_line_index(a_location);
	int top_x_cursor_bytes = cha_cursor_wo_get_x_cursor_bytes(priv->a_start);

	a_location = cha_cursor_wo_get_line_location(priv->a_end);
	int bottom_page_idx = cha_line_location_wo_get_page_index(a_location);
	int bottom_page_line_idx = cha_line_location_wo_get_page_line_index(a_location);
	int bottom_x_cursor_bytes = cha_cursor_wo_get_x_cursor_bytes(priv->a_end);

	if ((bottom_page_idx<top_page_idx)
			|| ((bottom_page_idx==top_page_idx) && (bottom_page_line_idx<top_page_line_idx))
			|| ((bottom_page_idx==top_page_idx) && (bottom_page_line_idx==top_page_line_idx) && (bottom_x_cursor_bytes<top_x_cursor_bytes))
			) {
		priv->top_page_index = bottom_page_idx;
		priv->top_page_line_index = bottom_page_line_idx;
		priv->top_x_cursor_bytes = bottom_x_cursor_bytes;
		priv->bottom_page_index = top_page_idx;
		priv->bottom_page_line_index = top_page_line_idx;
		priv->bottom_x_cursor_bytes = top_x_cursor_bytes;
	} else {
		priv->top_page_index = top_page_idx;
		priv->top_page_line_index = top_page_line_idx;
		priv->top_x_cursor_bytes = top_x_cursor_bytes;
		priv->bottom_page_index = bottom_page_idx;
		priv->bottom_page_line_index = bottom_page_line_idx;
		priv->bottom_x_cursor_bytes = bottom_x_cursor_bytes;
	}

}

gboolean cha_selection_set_start(ChaSelection *selection, ChaCursorWo *cursor) {
	ChaSelectionPrivate *priv = cha_selection_get_instance_private(selection);
	if (cha_cursor_wo_equal(priv->a_start, cursor)) {
		return FALSE;
	}
	cursor = cha_cursor_wo_clone(cursor, CAT_CLONE_DEPTH_NONE);
	cursor = cha_cursor_wo_anchor(cursor, 0);
	cat_unref_ptr(priv->a_start);
	priv->a_start = cursor;

	l_selection_order(priv);

	priv->modification_count = cat_atomic_integer_increment(priv->sequence);
	return TRUE;
}


gboolean cha_selection_set_end(ChaSelection *selection, ChaCursorWo *cursor) {
	ChaSelectionPrivate *priv = cha_selection_get_instance_private(selection);
	if (cha_cursor_wo_equal(priv->a_end, cursor)) {
		return FALSE;
	}
	cursor = cha_cursor_wo_clone(cursor, CAT_CLONE_DEPTH_NONE);
	cursor = cha_cursor_wo_anchor(cursor, 0);
	cat_ref_swap(priv->a_end, cursor);
	cat_unref_ptr(cursor);

	l_selection_order(priv);

	priv->modification_count = cat_atomic_integer_increment(priv->sequence);
	return TRUE;
}



ChaCursorWo *cha_selection_get_start(ChaSelection *selection) {
	ChaSelectionPrivate *priv = cha_selection_get_instance_private(selection);
	return priv->a_start;
}

ChaCursorWo *cha_selection_get_end(ChaSelection *selection) {
	ChaSelectionPrivate *priv = cha_selection_get_instance_private(selection);
	return priv->a_end;
}

void cha_selection_get_top(ChaSelection *selection, int *page_index, int *page_line_index, int *cursor_x) {
	ChaSelectionPrivate *priv = cha_selection_get_instance_private(selection);
	if (page_index) {
		*page_index = priv->top_page_index;
	}
	if (page_line_index) {
		*page_line_index = priv->top_page_line_index;
	}
	if (cursor_x) {
		*cursor_x = priv->top_x_cursor_bytes;
	}
}

void cha_selection_get_bottom(ChaSelection *selection, int *page_index, int *page_line_index, int *cursor_x) {
	ChaSelectionPrivate *priv = cha_selection_get_instance_private(selection);
	if (page_index) {
		*page_index = priv->bottom_page_index;
	}
	if (page_line_index) {
		*page_line_index = priv->bottom_page_line_index;
	}
	if (cursor_x) {
		*cursor_x = priv->bottom_x_cursor_bytes;
	}
}


ChaCursorWo *cha_selection_delete(ChaSelection *selection, struct _ChaDocumentView *document_view, ChaRevisionWo *e_revision) {
	ChaSelectionClass *sel_class = CHA_SELECTION_GET_CLASS(selection);
	return sel_class->delete(selection, document_view, e_revision);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaSelection *instance = CHA_SELECTION(self);
	ChaSelectionPrivate *priv = cha_selection_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p version=%d, start=%o, end=%o]", iname, self, priv->modification_count, priv->a_start, priv->a_end);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
