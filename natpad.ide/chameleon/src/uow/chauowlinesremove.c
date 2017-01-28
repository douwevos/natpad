/*
   File:    chauowlinesremove.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Aug 30, 2015
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

#include "chauowlinesremove.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaUowLinesRemove"
#include <logging/catlog.h>

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaUowLinesRemove, cha_uow_lines_remove, CHA_TYPE_UOW,
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

static void cha_uow_lines_remove_class_init(ChaUowLinesRemoveClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void cha_uow_lines_remove_init(ChaUowLinesRemove *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ChaUowLinesRemove *instance = CHA_UOW_LINES_REMOVE(object);
	G_OBJECT_CLASS(cha_uow_lines_remove_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_uow_lines_remove_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaUowLinesRemove *cha_uow_lines_remove_new() {
	ChaUowLinesRemove *result = g_object_new(CHA_TYPE_UOW_LINES_REMOVE, NULL);
	cat_ref_anounce(result);
	cha_uow_construct((ChaUow *) result, FALSE);
	return result;
}

static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	ChaLineLocationWo *ll_first = NULL;
	ChaLineLocationWo *ll_last = NULL;
	int valid_end_column;
//	l_get_first_and_last_row(document_view, &ll_first, &ll_last, &valid_end_column);
	cha_uow_get_first_and_last_row(document_view, &ll_first, &ll_last, &valid_end_column);

	cat_log_debug("ll_first=%o, ll_last=%o", ll_first, ll_last);
	if (ll_first==NULL || ll_last==NULL) {
		cat_unref_ptr(ll_first);
		cat_unref_ptr(ll_last);
		return;
	}

	ChaDocument *document = cha_document_view_get_document(document_view);
	gboolean is_editable = cha_document_is_editable(document);
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);

	long long first_row = cha_revision_wo_calculate_row(e_revision, ll_first);
	long long last_row = cha_revision_wo_calculate_row(e_revision, ll_last);

	cha_revision_wo_lines_remove(e_revision, first_row, last_row);

	cha_document_view_clear_selection(document_view, NULL);

	if (!is_editable) {
		cat_log_debug("anchoring");
		cha_document_anchor_document(document);
	}
	cat_log_debug("done");
	cat_unref_ptr(ll_first);
	cat_unref_ptr(ll_last);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
