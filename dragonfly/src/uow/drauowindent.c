/*
   File:    drauowindent.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 14, 2015
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

#include "drauowindent.h"
#include "../context/dracontexteditor.h"
#include "../draeditor.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraUowIndent"
#include <logging/catlog.h>

struct _DraUowIndentPrivate {
	gboolean is_indent;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraUowIndent, dra_uow_indent, CHA_TYPE_UOW,
		G_ADD_PRIVATE(DraUowIndent)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view);

static void dra_uow_indent_class_init(DraUowIndentClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	ChaUowClass *uow_class = CHA_UOW_CLASS(clazz);
	uow_class->run = l_uow_run;
}

static void dra_uow_indent_init(DraUowIndent *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	DraUowIndent *instance = DRA_UOW_INDENT(object);
//	DraUowIndentPrivate *priv = dra_uow_indent_get_instance_private(instance);
	G_OBJECT_CLASS(dra_uow_indent_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_uow_indent_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraUowIndent *dra_uow_indent_new(gboolean is_indent) {
	DraUowIndent *result = g_object_new(DRA_TYPE_UOW_INDENT, NULL);
	cat_ref_anounce(result);
	DraUowIndentPrivate *priv = dra_uow_indent_get_instance_private(result);
	cha_uow_construct((ChaUow *) result, FALSE);
	priv->is_indent = is_indent;
	return result;
}

static void l_uow_run(ChaUow *self, struct _ChaEditor *editor, ChaDocumentView *document_view) {
	DraUowIndentPrivate *priv = dra_uow_indent_get_instance_private(DRA_UOW_INDENT(self));

	DraContextEditor *context_editor = dra_editor_get_context_editor((DraEditor *) editor);
	if (context_editor) {
		dra_context_editor_indent(context_editor, priv->is_indent);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraUowIndent *instance = DRA_UOW_INDENT(self);
	DraUowIndentPrivate *priv = dra_uow_indent_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p is_indent=%d]", iname, self, priv->is_indent);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/

