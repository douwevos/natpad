/*
   File:    chauowchangecase.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Oct 13, 2015
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

#include "chauowchangecase.h"
#include "../selection/chaselection.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaUowChangeCase"
#include <logging/catlog.h>

struct _ChaUowChangeCasePrivate {
	ChaUowCase to_case;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaUowChangeCase, cha_uow_change_case, CHA_TYPE_UOW,
		G_ADD_PRIVATE(ChaUowChangeCase)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

static void cha_uow_change_case_class_init(ChaUowChangeCaseClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void cha_uow_change_case_init(ChaUowChangeCase *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	ChaUowChangeCase *instance = CHA_UOW_CHANGE_CASE(object);
//	ChaUowChangeCasePrivate *priv = cha_uow_change_case_get_instance_private(instance);
	G_OBJECT_CLASS(cha_uow_change_case_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_uow_change_case_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaUowChangeCase *cha_uow_change_case_new(ChaUowCase to_case) {
	ChaUowChangeCase *result = g_object_new(CHA_TYPE_UOW_CHANGE_CASE, NULL);
	cat_ref_anounce(result);
	ChaUowChangeCasePrivate *priv = cha_uow_change_case_get_instance_private(result);
	priv->to_case = to_case;
	cha_uow_construct((ChaUow *) result, FALSE);
	return result;
}


static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	ChaUowChangeCasePrivate *priv = cha_uow_change_case_get_instance_private(CHA_UOW_CHANGE_CASE(self));
	ChaSelection *selection = cha_document_view_get_selection(document_view);
	if (selection==NULL) {
		return;
	}
	ChaDocument *document = cha_document_view_get_document(document_view);
	gboolean is_editable = cha_document_is_editable(document);
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);

	ChaSelectionClass *sl_class = CHA_SELECTION_GET_CLASS(selection);
	sl_class->changeCase(selection, document_view, e_revision, priv->to_case == CHA_UOW_CASE_TO_UPPER);

	if (!is_editable) {
		cha_document_anchor_document(document);
		cha_document_view_move_view_to_focus(document_view, FALSE);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaUowChangeCase *instance = CHA_UOW_CHANGE_CASE(self);
	ChaUowChangeCasePrivate *priv = cha_uow_change_case_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p to_case=%d]", iname, self, priv->to_case);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
