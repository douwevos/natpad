/*
   File:    terprefsmainpanel.c
   Project: tern
   Author:  Douwe Vos
   Date:    Oct 26, 2014
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

#include "terprefsmainpanel.h"
#include "../terpreferenceswo.h"
#include "../terprefsclazzwo.h"
#include "../terprefsmapper.h"
#include <leafhopper.h>
#include <elk.h>

#include "tergladeprefsmain.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerPrefsMainPanel"
#include <logging/catlog.h>

struct _TerPrefsMainPanelPrivate {
	GtkWidget *w_but_add;
};

static void l_stringable_iface_init(CatIStringableInterface *iface);

G_DEFINE_TYPE_WITH_CODE(TerPrefsMainPanel, ter_prefs_main_panel, COW_TYPE_PANEL,
		G_ADD_PRIVATE(TerPrefsMainPanel)
		G_IMPLEMENT_INTERFACE(CAT_TYPE_ISTRINGABLE, l_stringable_iface_init)
);

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void ter_prefs_main_panel_class_init(TerPrefsMainPanelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_prefs_main_panel_init(TerPrefsMainPanel *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(ter_prefs_main_panel_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(ter_prefs_main_panel_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

static void l_on_new_class_pressed(GtkButton *button, gpointer self);


TerPrefsMainPanel *ter_prefs_main_panel_new() {
	TerPrefsMainPanel *result = g_object_new(TER_TYPE_PREFS_MAIN_PANEL, NULL);
	cat_ref_anounce(result);
	TerPrefsMainPanelPrivate *priv = ter_prefs_main_panel_get_instance_private(result);
	cow_panel_construct((CowPanel *) result);

	GtkBuilder *builder = gtk_builder_new();
	GError *gerror = NULL;
	gtk_builder_add_from_string(builder, (const gchar *) tergladeprefsmain_glade, (gsize) tergladeprefsmain_glade_len, (GError **) (&gerror));
	GtkWidget *frame = (GtkWidget *) gtk_builder_get_object(builder, "main_frame");

	gtk_widget_set_vexpand_set(frame, TRUE);

	gtk_container_add((GtkContainer *) result, frame);

	priv->w_but_add = (GtkWidget *) gtk_builder_get_object(builder, "but_add");
	g_signal_connect(priv->w_but_add, "clicked", G_CALLBACK(l_on_new_class_pressed), result);

	return result;
}


static void l_on_new_class_pressed(GtkButton *button, gpointer self) {
	TerPrefsMainPanel *panel_main = TER_PREFS_MAIN_PANEL(self);

	LeaBasicInputDialog *dialog = lea_basic_input_dialog_new(GTK_WIDGET(panel_main), "Class _name", "Please type the name of the new document class below");
	int result = gtk_dialog_run(GTK_DIALOG(dialog));
	if (result == GTK_RESPONSE_OK) {
		const gchar *txt_new_class_name = lea_basic_input_dialog_get_entry_text(dialog);
		if (txt_new_class_name) {
			cat_log_debug("txt_new_class_name=%s", txt_new_class_name);
			CatStringWo *new_class_name = cat_string_wo_new_with(txt_new_class_name);

			ElkPreferencesWo *e_prefs = (ElkPreferencesWo *) cow_panel_get_configuration((CowPanel *) panel_main);
			ElkPreferencesWo *e_replacement = elk_preferences_wo_clone(e_prefs, CAT_CLONE_DEPTH_MAIN);

			TerPreferencesWo *e_ter_prefs = (TerPreferencesWo *) elk_preferences_wo_get_sub(e_replacement, CAT_S(ter_s_textpad));

			if (e_ter_prefs == NULL) {
				e_ter_prefs = ter_preferences_wo_new();
				elk_preferences_wo_set_sub(e_replacement, CAT_S(ter_s_textpad), (GObject *) e_ter_prefs);
				cat_unref(e_ter_prefs);
			} else if (ter_preferences_wo_is_anchored(e_ter_prefs)) {
				e_ter_prefs = ter_preferences_wo_create_editable(e_ter_prefs);
				elk_preferences_wo_set_sub(e_replacement, CAT_S(ter_s_textpad), (GObject *) e_ter_prefs);
				cat_unref(e_ter_prefs);
			}


			TerPrefsClazzWo *new_clazz = ter_preferences_wo_get_clazz(e_ter_prefs, new_class_name);
			if (new_clazz==NULL) {
				new_clazz = ter_prefs_clazz_wo_new(new_class_name);
				cat_log_error("new_clazz=%p", new_clazz);
				ter_preferences_wo_set_clazz(e_ter_prefs, new_clazz);
				cow_panel_set_configuration((CowPanel *) panel_main, (GObject *) e_replacement);
			}

//			cat_unref_ptr(e_replacement);
		} else {
			cat_log_warn("txt_new_class_name is NULL");
		}
	}
	gtk_widget_destroy(GTK_WIDGET(dialog));

}



/********************* start CatIStringable implementation *********************/

static void l_stringable_print(CatIStringable *self, struct _CatStringWo *e_append_to) {
	const char *iname = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_format(e_append_to, "%s[%p]", iname, self);
}

static void l_stringable_iface_init(CatIStringableInterface *iface) {
	iface->print = l_stringable_print;
}

/********************* end CatIStringable implementation *********************/
