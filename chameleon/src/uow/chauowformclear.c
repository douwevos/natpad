/*
   File:    chauowformclear.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Oct 16, 2015
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

#include "chauowformclear.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaUowFormClear"
#include <logging/catlog.h>

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaUowFormClear, cha_uow_form_clear, CHA_TYPE_UOW,
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

static void cha_uow_form_clear_class_init(ChaUowFormClearClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void cha_uow_form_clear_init(ChaUowFormClear *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ChaUowFormClear *instance = CHA_UOW_FORM_CLEAR(object);
	G_OBJECT_CLASS(cha_uow_form_clear_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_uow_form_clear_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaUowFormClear *cha_uow_form_clear_new() {
	ChaUowFormClear *result = g_object_new(CHA_TYPE_UOW_FORM_CLEAR, NULL);
	cat_ref_anounce(result);
	cha_uow_construct((ChaUow *) result, FALSE);
	return result;
}


gboolean cha_uow_form_clear_run(ChaUowFormClear *form_clear, ChaDocumentView *document_view) {
	gboolean result = FALSE;
	ChaDocument *document = cha_document_view_get_document(document_view);
	gboolean is_editable = cha_document_is_editable(document);
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);
	if (cha_revision_wo_get_form(e_revision)!=NULL) {
		cha_revision_wo_set_form(e_revision, NULL);
		result = TRUE;
	}
	if (!is_editable) {
		cha_document_anchor_document(document);
	}
	return result;
}

static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	cha_uow_form_clear_run(CHA_UOW_FORM_CLEAR(self), document_view);
}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
//	ChaUowFormClear *instance = CHA_UOW_FORM_CLEAR(self);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
