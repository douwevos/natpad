/*
   File:    chauow.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jan 28, 2015
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

#include "chauow.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaUow"
#include <logging/catlog.h>

struct _ChaUowPrivate {
	gboolean is_reading_only;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaUow, cha_uow, G_TYPE_OBJECT,
		G_ADD_PRIVATE(ChaUow)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_uow_class_init(ChaUowClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void cha_uow_init(ChaUow *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ChaUow *instance = CHA_UOW(object);
//	ChaUowPrivate *priv = cha_uow_get_instance_private(instance);
	G_OBJECT_CLASS(cha_uow_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_uow_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void cha_uow_construct(ChaUow *uow, gboolean is_reading_only) {
	ChaUowPrivate *priv = cha_uow_get_instance_private(uow);
	priv->is_reading_only = is_reading_only;
}


gboolean cha_uow_is_reading_only(const ChaUow *uow) {
	ChaUowPrivate *priv = cha_uow_get_instance_private((ChaUow *)uow);
	return priv->is_reading_only;
}

void cha_uow_run(ChaUow *uow, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	ChaDocument *document = cha_document_view_get_document(document_view);
	if (cha_document_is_read_only(document)) {
		if (!cha_uow_is_reading_only(uow)) {
			/* document is read only but UOW is not reading only */
			return;
		}
	}
	CHA_UOW_GET_CLASS(uow)->run(uow, editor, document_view);
}

void cha_uow_get_first_and_last_row(ChaDocumentView *document_view, ChaLineLocationWo **first_ll, ChaLineLocationWo **last_ll, int *valid_end_column) {
	ChaDocument *document = cha_document_view_get_document(document_view);
	ChaRevisionWo *revision = cha_document_get_current_revision_ref(document);
	ChaSelection *selection = cha_document_view_get_selection(document_view);
	if (selection!=NULL) {
		ChaCursorWo *start_cursor = cha_selection_get_start(selection);
		ChaCursorWo *end_cursor = cha_selection_get_end(selection);
		ChaLineLocationWo *start_ll = cha_cursor_wo_get_line_location(start_cursor);
		ChaLineLocationWo *end_ll = cha_cursor_wo_get_line_location(end_cursor);
		int ll_comp = cha_line_location_wo_compare(start_ll, end_ll);


		if (ll_comp==-1) {
			*valid_end_column = -1;
			*first_ll = cat_ref_ptr(start_ll);
			if (cha_cursor_wo_get_x_cursor_bytes(end_cursor)==0 /*&& end_cursor->sub_column==0 */) { // TODO sub column
				ChaLineLocationWo *t = cha_line_location_wo_clone(end_ll, CAT_CLONE_DEPTH_MAIN);
				cha_line_location_wo_set_page_line_index(t, cha_line_location_wo_get_page_line_index(t)-1);
				if (cha_line_location_wo_bind_to_revision(t, revision)) {
					*last_ll = cha_line_location_wo_anchor(t, 0);
				} else {
					cat_unref_ptr(t);
					*last_ll = NULL;
				}
			} else {
				*last_ll = cat_ref_ptr(end_ll);
			}
		} else if (ll_comp==1) {
			*valid_end_column = -1;
			*first_ll = cat_ref_ptr(end_ll);

			if (cha_cursor_wo_get_x_cursor_bytes(start_cursor)==0 /*&& end_cursor->sub_column==0 */) { // TODO sub column
				ChaLineLocationWo *t = cha_line_location_wo_clone(start_ll, CAT_CLONE_DEPTH_MAIN);
				cha_line_location_wo_set_page_line_index(t, cha_line_location_wo_get_page_line_index(t)-1);
				if (cha_line_location_wo_bind_to_revision(t, revision)) {
					*last_ll = cha_line_location_wo_anchor(t, 0);
				} else {
					cat_unref_ptr(t);
					*last_ll = NULL;
				}
			} else {
				*last_ll = cat_ref_ptr(start_ll);
			}
		} else {
			*first_ll = cat_ref_ptr(end_ll);
			*last_ll = cat_ref_ptr(start_ll);
			*valid_end_column = -1;

			int end_x_cur = cha_cursor_wo_get_x_cursor_bytes(end_cursor);
			int start_x_cur = cha_cursor_wo_get_x_cursor_bytes(start_cursor);
			if (start_x_cur > end_x_cur && end_x_cur==0) {
				*valid_end_column = start_x_cur;
			} else if (start_x_cur<end_x_cur && start_x_cur==0) {
				*valid_end_column = end_x_cur;
			}


		}
	} else {
		ChaCursorWo *cur_cursor = cha_revision_wo_get_cursor(revision);
		ChaLineLocationWo *cur_ll = cha_cursor_wo_get_line_location(cur_cursor);
		*first_ll = cat_ref_ptr(cur_ll);
		*last_ll = cat_ref_ptr(cur_ll);
		*valid_end_column = -1;
	}
	cat_unref_ptr(revision);
}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	ChaUow *instance = CHA_UOW(self);
//	ChaUowPrivate *priv = cha_uow_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
