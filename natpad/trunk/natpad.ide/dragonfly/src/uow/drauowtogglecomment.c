/*
   File:    drauowtogglecomment.c
   Project: dragonfly
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

#include "drauowtogglecomment.h"
#include "../draeditor.h"
#include "../context/dracontexteditor.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraUowToggleComment"
#include <logging/catlog.h>

struct _DraUowToggleCommentPrivate {
	gboolean line_comment;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraUowToggleComment, dra_uow_toggle_comment, CHA_TYPE_UOW,
		G_ADD_PRIVATE(DraUowToggleComment)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

static void dra_uow_toggle_comment_class_init(DraUowToggleCommentClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void dra_uow_toggle_comment_init(DraUowToggleComment *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraUowToggleComment *instance = DRA_UOW_TOGGLE_COMMENT(object);
//	DraUowToggleCommentPrivate *priv = dra_uow_toggle_comment_get_instance_private(instance);
	G_OBJECT_CLASS(dra_uow_toggle_comment_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_uow_toggle_comment_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraUowToggleComment *dra_uow_toggle_comment_new() {
	DraUowToggleComment *result = g_object_new(DRA_TYPE_UOW_TOGGLE_COMMENT, NULL);
	cat_ref_anounce(result);
	DraUowToggleCommentPrivate *priv = dra_uow_toggle_comment_get_instance_private(result);
	priv->line_comment = TRUE;
	cha_uow_construct((ChaUow *) result, FALSE);
	return result;
}

static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	DraContextEditor *context_editor = dra_editor_get_context_editor(DRA_EDITOR(editor));
	if (context_editor) {
		dra_context_editor_toggle_line_comment(context_editor);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraUowToggleComment *instance = DRA_UOW_TOGGLE_COMMENT(self);
	DraUowToggleCommentPrivate *priv = dra_uow_toggle_comment_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p line_comment=%d]", iname, self, priv->line_comment);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
