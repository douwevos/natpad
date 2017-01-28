/*
   File:    draacentry.c
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 17, 2015
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

#include "draacentry.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraAcEntry"
#include <logging/catlog.h>

struct _DraAcEntryPrivate {
	CatStringWo *entry_text;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraAcEntry, dra_ac_entry, G_TYPE_OBJECT,
		G_ADD_PRIVATE(DraAcEntry)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void dra_ac_entry_class_init(DraAcEntryClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_ac_entry_init(DraAcEntry *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraAcEntry *instance = DRA_AC_ENTRY(object);
	DraAcEntryPrivate *priv = dra_ac_entry_get_instance_private(instance);
	cat_unref_ptr(priv->entry_text);
	G_OBJECT_CLASS(dra_ac_entry_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_ac_entry_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


void dra_ac_entry_construct(DraAcEntry *entry, CatStringWo *entry_text) {
	DraAcEntryPrivate *priv = dra_ac_entry_get_instance_private(entry);
	priv->entry_text = cat_string_wo_clone(entry_text, CAT_CLONE_DEPTH_NONE);
}

DraAcEntry *dra_ac_entry_new(CatStringWo *entry_text) {
	DraAcEntry *result = g_object_new(DRA_TYPE_AC_ENTRY, NULL);
	cat_ref_anounce(result);
	dra_ac_entry_construct(result, entry_text);
	return result;
}
CatStringWo *dra_ac_entry_get_filter_text(DraAcEntry *entry) {
	DraAcEntryPrivate *priv = dra_ac_entry_get_instance_private(entry);
	return priv->entry_text;
}

int dra_ac_entry_get_width(DraAcEntry *entry, PangoLayout *pango_layout) {
	DraAcEntryPrivate *priv = dra_ac_entry_get_instance_private(entry);
	const char *pango_text = cat_string_wo_getchars(priv->entry_text);
	int pango_text_len = cat_string_wo_length(priv->entry_text);
	pango_layout_set_text(pango_layout, pango_text, pango_text_len);
	PangoRectangle ink_rect;
	PangoRectangle logical_rect;
	pango_layout_get_pixel_extents(pango_layout, &ink_rect, &logical_rect);
	return logical_rect.width;
}

void dra_ac_entry_paint(DraAcEntry *entry, GtkWidget *widget, cairo_t *cairo, PangoLayout *pango_layout, int ypos, int entry_height, gboolean selected) {
	DraAcEntryPrivate *priv = dra_ac_entry_get_instance_private(entry);

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

	const char *pango_text = cat_string_wo_getchars(priv->entry_text);
	int pango_text_len = cat_string_wo_length(priv->entry_text);

	cairo_set_source_rgb(cairo, 0, 0, 0.2);
	pango_cairo_update_layout(cairo, pango_layout);
	pango_layout_set_text(pango_layout, pango_text, pango_text_len);
	cairo_move_to(cairo, 0, ypos);
	pango_cairo_show_layout(cairo, pango_layout);
}

void dra_ac_entry_run(DraAcEntry *entry, struct _DraAcContext *ac_context) {
	DraAcEntryClass *entry_class = DRA_AC_ENTRY_GET_CLASS(entry);
	entry_class->run(entry, ac_context);

}

/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraAcEntry *instance = DRA_AC_ENTRY(self);
	DraAcEntryPrivate *priv = dra_ac_entry_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);

	cat_string_wo_format(append_to, "%s[%p entry_text=%o]", iname, self, priv->entry_text);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
