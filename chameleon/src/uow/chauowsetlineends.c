/*
   File:    chauowsetlineends.c
   Project: chameleon
   Author:  Douwe Vos
   Date:    Apr 14, 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

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

#include "chauowsetlineends.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ChaUowSetLineEnds"
#include <logging/catlog.h>

struct _ChaUowSetLineEndsPrivate {
	ChaLineEnd line_end;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

G_DEFINE_TYPE_WITH_CODE(ChaUowSetLineEnds, cha_uow_set_line_ends, CHA_TYPE_UOW,  // @suppress("Unused static function")
	G_ADD_PRIVATE(ChaUowSetLineEnds)
	G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void cha_uow_set_line_ends_class_init(ChaUowSetLineEndsClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void cha_uow_set_line_ends_init(ChaUowSetLineEnds *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(cha_uow_set_line_ends_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(cha_uow_set_line_ends_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

ChaUowSetLineEnds *cha_uow_set_line_ends_new(ChaLineEnd line_end) {
	ChaUowSetLineEnds *result = g_object_new(CHA_TYPE_UOW_SET_LINE_ENDS, NULL);
	cat_ref_anounce(result);
	ChaUowSetLineEndsPrivate *priv = cha_uow_set_line_ends_get_instance_private(result);
	cha_uow_construct((ChaUow *) result, TRUE);
	priv->line_end = line_end;
	return result;
}


static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	ChaUowSetLineEndsPrivate *priv = cha_uow_set_line_ends_get_instance_private((ChaUowSetLineEnds *) self);
	ChaDocument *document = cha_document_view_get_document(document_view);
	gboolean is_editable = cha_document_is_editable(document);
	ChaRevisionWo *a_rev = cha_document_get_editable_revision(document);
	cha_revision_wo_set_line_ends(a_rev, priv->line_end, FALSE);

	if (!is_editable) {
		cha_document_anchor_document(document);
		cha_document_view_invalidate_lines(document_view);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	ChaUowSetLineEnds *instance = CHA_UOW_SET_LINE_ENDS(self);
	ChaUowSetLineEndsPrivate *priv = cha_uow_set_line_ends_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p line_end=%d]", iname, self, priv->line_end);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

