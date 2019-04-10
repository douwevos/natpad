/*
   File:    draprefscolormappanel.c
   Project: elk
   Author:  Douwe Vos
   Date:    Oct 19, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#include "draprefscolormappanel.h"
#include "../drapreferenceswo.h"
#include <chameleon.h>

#include "dragladeprefscolormap.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "DraPrefsColorMapPanel"
#include <logging/catlog.h>

struct _DraPrefsColorMapPanelPrivate {
	CowIEntryAccessor *entry_accessor;
	CatWo *e_prefs;
	ChaPreferencesWo *e_dra_prefs;
	GtkWidget **w_color_s;
	gulong *s_color_s;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(DraPrefsColorMapPanel, dra_prefs_color_map_panel, COW_TYPE_PANEL,
		G_ADD_PRIVATE(DraPrefsColorMapPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);
static void l_panel_reconfigure(CowPanel *self, GObject *config);

static void dra_prefs_color_map_panel_class_init(DraPrefsColorMapPanelClass *clazz) {
	CowPanelClass *panel_class = COW_PANEL_CLASS(clazz);
	panel_class->reconfigure = l_panel_reconfigure;

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void dra_prefs_color_map_panel_init(DraPrefsColorMapPanel *instance) {
}


static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	DraPrefsColorMapPanel *instance = DRA_PREFS_COLOR_MAP_PANEL(object);
	DraPrefsColorMapPanelPrivate *priv = dra_prefs_color_map_panel_get_instance_private(instance);
	cat_unref_ptr(priv->entry_accessor);
	G_OBJECT_CLASS(dra_prefs_color_map_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(dra_prefs_color_map_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


static gboolean l_notify_modification(DraPrefsColorMapPanel *panel) {
	DraPrefsColorMapPanelPrivate *priv = dra_prefs_color_map_panel_get_instance_private(panel);
	cow_panel_set_configuration((CowPanel *) panel, (GObject *) priv->e_prefs);
	return TRUE;
}


static void l_color_set(GtkColorButton *button, gpointer data) {
	DraPrefsColorMapPanel *instance = DRA_PREFS_COLOR_MAP_PANEL(data);
	DraPrefsColorMapPanelPrivate *priv = dra_prefs_color_map_panel_get_instance_private(instance);

	int found_idx = -1;
	int idx;
	for(idx=0; idx<18; idx++) {
		GtkColorButton *w = (GtkColorButton *) priv->w_color_s[idx];
		if (w==button) {
			found_idx = idx;
			break;
		}
	}

	GdkRGBA rgba;
	if (found_idx>=0) {
		gtk_color_chooser_get_rgba(GTK_COLOR_CHOOSER(button), &rgba);
		ChaPrefsColorMapWo *e_color_map = cha_preferences_wo_get_editable_color_map(priv->e_dra_prefs);
		cha_prefs_color_map_wo_set_color_at(e_color_map, rgba.red, rgba.green, rgba.blue, found_idx);

		l_notify_modification(instance);
	}
}

DraPrefsColorMapPanel *dra_prefs_color_map_panel_new(CowIEntryAccessor *entry_accessor) {
	DraPrefsColorMapPanel *result = g_object_new(DRA_TYPE_PREFS_COLOR_MAP_PANEL, NULL);
	cat_ref_anounce(result);
	DraPrefsColorMapPanelPrivate *priv = dra_prefs_color_map_panel_get_instance_private(result);
	priv->entry_accessor = cat_ref_ptr(entry_accessor);
	priv->e_prefs = NULL;
	priv->e_dra_prefs = NULL;
	priv->w_color_s = g_new(GtkWidget *, 18);
	priv->s_color_s = g_new(gulong, 18);
	cow_panel_construct((CowPanel *) result);


	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) dragladeprefscolormap_glade, (gsize) dragladeprefscolormap_glade_len, (GError **) (&gerror));
	GtkWidget *frame = (GtkWidget *) gtk_builder_get_object(builder, "main_frame");

	gtk_widget_set_vexpand_set(frame, TRUE);

	gtk_container_add((GtkContainer *) result, frame);

	priv->w_color_s[0] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton1");
	priv->w_color_s[1] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton2");
	priv->w_color_s[2] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton3");
	priv->w_color_s[3] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton4");
	priv->w_color_s[4] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton5");
	priv->w_color_s[5] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton6");
	priv->w_color_s[6] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton7");
	priv->w_color_s[7] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton8");
	priv->w_color_s[8] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton9");
	priv->w_color_s[9] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton10");
	priv->w_color_s[10] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton11");
	priv->w_color_s[11] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton12");
	priv->w_color_s[12] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton13");
	priv->w_color_s[13] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton14");
	priv->w_color_s[14] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton15");
	priv->w_color_s[15] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton16");
	priv->w_color_s[16] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton17");
	priv->w_color_s[17] = (GtkWidget *) gtk_builder_get_object(builder, "colorbutton18");

	int idx;
	for(idx=0; idx<18; idx++) {
		GtkWidget *w = priv->w_color_s[idx];
		priv->s_color_s[idx] = g_signal_connect(w, "color-set", G_CALLBACK(l_color_set), result);
	}

	return result;
}



static void l_refresh_form(DraPrefsColorMapPanel *panel) {
	DraPrefsColorMapPanelPrivate *priv = dra_prefs_color_map_panel_get_instance_private(panel);
	if (priv->e_dra_prefs) {
		ChaPrefsColorMapWo *color_map = cha_preferences_wo_get_color_map(priv->e_dra_prefs);
		if (color_map) {
			int idx;
			int count = cha_prefs_color_map_wo_count(color_map);
			GdkRGBA color;
			color.alpha = 1;
			for(idx=0; idx<18; idx++) {
				GtkWidget *w = priv->w_color_s[idx];
				g_signal_handler_block(w, priv->s_color_s[idx]);
				if (idx<count) {
					ChaColorEntry ce = cha_prefs_color_map_wo_get_color(color_map, idx);
					color.red = ce.red;
					color.green = ce.green;
					color.blue = ce.blue;
					gtk_color_chooser_set_rgba(GTK_COLOR_CHOOSER(w), &color);
					gtk_widget_set_visible(w, TRUE);
				} else {
					gtk_widget_set_visible(w, FALSE);
				}
				g_signal_handler_unblock(w, priv->s_color_s[idx]);
			}
		}
	}
}




static void l_panel_reconfigure(CowPanel *self, GObject *config) {
	DraPrefsColorMapPanel *instance = DRA_PREFS_COLOR_MAP_PANEL(self);
	DraPrefsColorMapPanelPrivate *priv = dra_prefs_color_map_panel_get_instance_private(instance);

	cat_log_debug("config=%o", config);
	CatWo *e_prefs = CAT_WO(config);
	if (e_prefs!=NULL) {
		e_prefs = cat_wo_clone(e_prefs, CAT_CLONE_DEPTH_MAIN);
	}
	cat_unref_ptr(priv->e_prefs);
	cat_unref_ptr(priv->e_dra_prefs);
	priv->e_prefs = e_prefs;

	if (e_prefs!=NULL) {

		cat_log_debug("e_prefs=%p", e_prefs);
		DraPreferencesWo *dra_prefs = (DraPreferencesWo *) cow_ientry_accessor_get(priv->entry_accessor, e_prefs);
		DraPreferencesWo *e_dra_prefs = NULL;
		if (dra_prefs == NULL) {
			e_dra_prefs = dra_preferences_wo_new();
		} else {
			e_dra_prefs = dra_preferences_wo_clone(dra_prefs, CAT_CLONE_DEPTH_MAIN);
		}
		cow_ientry_accessor_set(priv->entry_accessor, e_prefs, e_dra_prefs);
		cat_log_debug("e_dra_prefs=%p", e_dra_prefs);
		priv->e_dra_prefs = (ChaPreferencesWo *) e_dra_prefs;
	}

	l_refresh_form(instance);
}


/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *append_to) {
	DraPrefsColorMapPanel *instance = DRA_PREFS_COLOR_MAP_PANEL(self);
	DraPrefsColorMapPanelPrivate *priv = dra_prefs_color_map_panel_get_instance_private(instance);
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(append_to, "%s[%p e_ast_prefs=%p]", iname, self, priv->e_dra_prefs);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
