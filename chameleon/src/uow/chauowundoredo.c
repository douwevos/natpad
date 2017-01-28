/*
   File:    chauowundoredo.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Jun 22, 2015
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

#include "chauowundoredo.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaUowUndoRedo"
#include <logging/catlog.h>

struct _ChaUowUndoRedoPrivate {
	gboolean redo;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(ChaUowUndoRedo, cha_uow_undo_redo, CHA_TYPE_UOW,
		G_ADD_PRIVATE(ChaUowUndoRedo)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

static void cha_uow_undo_redo_class_init(ChaUowUndoRedoClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void cha_uow_undo_redo_init(ChaUowUndoRedo *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(cha_uow_undo_redo_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_uow_undo_redo_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


ChaUowUndoRedo *cha_uow_undo_redo_new(gboolean redo) {
	ChaUowUndoRedo *result = g_object_new(CHA_TYPE_UOW_UNDO_REDO, NULL);
	cat_ref_anounce(result);
	ChaUowUndoRedoPrivate *priv = cha_uow_undo_redo_get_instance_private(result);
	cha_uow_construct((ChaUow *) result, FALSE);
	priv->redo = redo;
	return result;
}

static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	ChaUowUndoRedoPrivate *priv = cha_uow_undo_redo_get_instance_private(CHA_UOW_UNDO_REDO(self));
	ChaDocument *document = cha_document_view_get_document(document_view);

	if (priv->redo) {
		cha_document_redo(document);
	} else {
		cha_document_undo(document);
	}
	cha_document_view_move_view_to_focus(document_view, FALSE);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaUowUndoRedo *instance = CHA_UOW_UNDO_REDO(self);
	ChaUowUndoRedoPrivate *priv = cha_uow_undo_redo_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p redo=%d]", iname, self, priv->redo);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
