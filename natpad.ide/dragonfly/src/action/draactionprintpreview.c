/*
   File:    draactionprintpreview.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Sep 6, 2015
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

#include "draactionprintpreview.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraActionPrintPreview"
#include <logging/catlog.h>

struct _DraActionPrintPreviewPrivate {
	DraEditorPanel *editor_panel;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraActionPrintPreview, dra_action_print_preview, LEA_TYPE_ACTION,
		G_ADD_PRIVATE(DraActionPrintPreview)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_action_run(LeaAction *self);

static void dra_action_print_preview_class_init(DraActionPrintPreviewClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = l_action_run;
}

static void dra_action_print_preview_init(DraActionPrintPreview *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraActionPrintPreview *instance = DRA_ACTION_PRINT_PREVIEW(object);
	DraActionPrintPreviewPrivate *priv = dra_action_print_preview_get_instance_private(instance);
	cat_unref_ptr(priv->editor_panel);
	G_OBJECT_CLASS(dra_action_print_preview_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_action_print_preview_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


DraActionPrintPreview *dra_action_print_preview_new() {
	DraActionPrintPreview *result = g_object_new(DRA_TYPE_ACTION_PRINT_PREVIEW, NULL);
	cat_ref_anounce(result);
	DraActionPrintPreviewPrivate *priv = dra_action_print_preview_get_instance_private(result);
	lea_action_construct((LeaAction *) result, cat_string_wo_new_with("dragonfly.print.preview"), cat_string_wo_new_with("Print Pre_view"), cat_string_wo_new_with("document-print-preview"));
	lea_action_set_sensitive_self(LEA_ACTION(result), FALSE);
	priv->editor_panel = NULL;
	return result;
}


void dra_action_print_preview_set_editor_panel(DraActionPrintPreview *action, DraEditorPanel *editor_panel) {
	DraActionPrintPreviewPrivate *priv = dra_action_print_preview_get_instance_private(action);
	cat_ref_swap(priv->editor_panel, editor_panel);
	lea_action_set_sensitive_self(LEA_ACTION(action), editor_panel!=NULL);
}

static void l_action_run(LeaAction *self) {
	DraActionPrintPreviewPrivate *priv = dra_action_print_preview_get_instance_private(DRA_ACTION_PRINT_PREVIEW(self));
	if (priv->editor_panel) {
		DraEditor *editor = dra_editor_panel_get_editor(priv->editor_panel);
		ChaEditorPrinter *editor_printer = cha_editor_printer_new((ChaEditor *) editor, NULL);
		cha_editor_printer_do_print(editor_printer, TRUE);
	}
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraActionPrintPreview *instance = DRA_ACTION_PRINT_PREVIEW(self);
	DraActionPrintPreviewPrivate *priv = dra_action_print_preview_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p editor_panel=%p]", iname, self, priv->editor_panel);
	cat_string_wo_format(append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
