/*
   File:    elkcompletionentry.c
   Project: elk
   Author:  Douwe Vos
   Date:    Sep 18, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#include "elkcompletionentry.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "ElkCompletionEntry"
#include <logging/catlog.h>

G_DEFINE_TYPE(ElkCompletionEntry, elk_completion_entry, G_TYPE_OBJECT) // @suppress("Unused static function")

static void l_dispose(GObject *object);

static void l_run_dummy(ElkCompletionEntry *entry, LeaPanel *panel) {
//	AstEditor *editor = AST_EDITOR(panel);
//	AstEditorUow *uow = (AstEditorUow *) ast_uow_insert_text_new(entry->a_text);
//	ast_editor_run(editor, uow);
//	cat_unref_ptr(uow);
}

static void elk_completion_entry_class_init(ElkCompletionEntryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	clazz->run = l_run_dummy;
}

static void elk_completion_entry_init(ElkCompletionEntry *obj) {
}

static void l_dispose(GObject *object) {
	ElkCompletionEntry *instance = ELK_COMPLETION_ENTRY(object);
	cat_unref_ptr(instance->a_text);
}


void elk_completion_entry_construct(ElkCompletionEntry *entry, CatStringWo *a_text) {
	entry->a_text = cat_ref_ptr(a_text);
}

ElkCompletionEntry *elk_completion_entry_new(CatStringWo *a_text) {
	ElkCompletionEntry *result = g_object_new(ELK_TYPE_COMPLETION_ENTRY, NULL);
	cat_ref_anounce(result);
	elk_completion_entry_construct(result, a_text);
	return result;
}


void elk_completion_entry_paint(GtkWidget *widget, ElkCompletionEntry *entry, cairo_t *cairo, PangoLayout *pango_layout, int ypos, int entry_height, gboolean selected) {

	int width = gtk_widget_get_allocated_width(widget);

	if (selected) {
//		cairo_set_source_rgb(cairo, 0, 0, 0.2);
//		cairo_rectangle(cairo, 0,ypos,width,entry_height);
//		cairo_stroke(cairo);

		GtkStyleContext *w_style_ctx = gtk_widget_get_style_context(widget);
		gtk_style_context_save(w_style_ctx);
		gtk_style_context_set_state(w_style_ctx, GTK_STATE_FLAG_SELECTED);
		gtk_render_background(w_style_ctx, cairo, 0, ypos, width, entry_height);
		gtk_style_context_restore(w_style_ctx);

	} else {
//		cairo_rectangle(cairo, 0,ypos,width,entry_height);
//		cairo_fill(cairo);
	}

	const char *pango_text = cat_string_wo_getchars(entry->a_text);
	int pango_text_len = cat_string_wo_length(entry->a_text);

	cairo_set_source_rgb(cairo, 0, 0, 0.2);
	pango_cairo_update_layout(cairo, pango_layout);
	pango_layout_set_text(pango_layout, pango_text, pango_text_len);
	cairo_move_to(cairo, 0, ypos);
	pango_cairo_show_layout(cairo, pango_layout);
}


int elk_completion_entry_get_width(ElkCompletionEntry *entry, PangoLayout *pango_layout) {
	const char *pango_text = cat_string_wo_getchars(entry->a_text);
	int pango_text_len = cat_string_wo_length(entry->a_text);
	pango_layout_set_text(pango_layout, pango_text, pango_text_len);
	PangoRectangle ink_rect;
	PangoRectangle logical_rect;
	pango_layout_get_pixel_extents(pango_layout, &ink_rect, &logical_rect);
	return logical_rect.width;
}


void elk_completion_entry_run(ElkCompletionEntry *entry, LeaPanel *editor_panel) {
	ElkCompletionEntryClass *entry_class = ELK_COMPLETION_ENTRY_GET_CLASS(entry);
	entry_class->run(entry, editor_panel);
}
