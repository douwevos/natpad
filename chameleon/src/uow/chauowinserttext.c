/*
   File:    chauowinserttext.c
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

#include "chauowinserttext.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaUowInsertText"
#include <logging/catlog.h>

struct _ChaUowInsertTextPrivate {
	CatStringWo *a_text;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaUowInsertText, cha_uow_insert_text, CHA_TYPE_UOW,
		G_ADD_PRIVATE(ChaUowInsertText)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);


static void cha_uow_insert_text_class_init(ChaUowInsertTextClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void cha_uow_insert_text_init(ChaUowInsertText *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	ChaUowInsertText *instance = CHA_UOW_INSERT_TEXT(object);
	ChaUowInsertTextPrivate *priv = cha_uow_insert_text_get_instance_private(instance);
	cat_unref_ptr(priv->a_text);
	G_OBJECT_CLASS(cha_uow_insert_text_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_uow_insert_text_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaUowInsertText *cha_uow_insert_text_new(CatStringWo *ca_text) {
	ChaUowInsertText *result = g_object_new(CHA_TYPE_UOW_INSERT_TEXT, NULL);
	cat_ref_anounce(result);
	ChaUowInsertTextPrivate *priv = cha_uow_insert_text_get_instance_private(result);
	cha_uow_construct((ChaUow *) result, FALSE);
	priv->a_text = ca_text;
	return result;
}


static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	ChaUowInsertTextPrivate *priv = cha_uow_insert_text_get_instance_private(CHA_UOW_INSERT_TEXT(self));
	ChaDocument *document = cha_document_view_get_document(document_view);
	gboolean is_editable = cha_document_is_editable(document);
	ChaRevisionWo *e_revision = cha_document_get_editable_revision(document);

	cha_document_view_remove_selection(document_view);

	CatArrayWo *line_list_out = cha_document_create_line_list(document, priv->a_text);
	cha_revision_wo_insert(e_revision, line_list_out);

	ChaCursorWo *e_cursor = cha_revision_wo_get_editable_cursor(e_revision);

//	cha_revision_wo_apply_form_changes(e_revision);

	cha_document_view_mark_layout_x_cursor(document_view,e_revision, e_cursor);

	if (!is_editable) {
		cha_document_anchor_document(document);
		cha_document_view_move_view_to_focus(document_view, FALSE);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaUowInsertText *instance = CHA_UOW_INSERT_TEXT(self);
	ChaUowInsertTextPrivate *priv = cha_uow_insert_text_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p text=%o]", iname, self, priv->a_text);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

