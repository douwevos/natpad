/*
   File:    leadigitpanel.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 20, 2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2009 Douwe Vos.

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

#include "leadigitpanel.h"
#include "leadigitpanelactiongroup.h"
#include "leadigitpanelowner.h"
#include "../../src/menu/leamenuaction.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaDigitPanel"
#include <logging/catlog.h>

G_DEFINE_TYPE(LeaDigitPanel, lea_digit_panel, LEA_TYPE_PANEL) // @suppress("Unused static function")

static void l_finalize(GObject *object);

static void lea_digit_panel_class_init(LeaDigitPanelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->finalize = l_finalize;
}

static void lea_digit_panel_init(LeaDigitPanel *obj) {
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalizing: %p %s", object, g_type_name_from_instance((GTypeInstance *) object));
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_digit_panel_parent_class)->finalize(object);
	cat_log_detail("finalized: %p", object);
}

static void l_disbale_actions(GtkButton *button, gpointer user_data) {
}

LeaDigitPanel *lea_digit_panel_new(LeaIPanelOwner *panel_owner, LeaFrame *frame) {
	cat_log_debug("owner=%o, frame=%o", panel_owner, frame);
	LeaDigitPanel *result = g_object_new(LEA_TYPE_DIGIT_PANEL, NULL);
	cat_ref_anounce(result);

	GtkWidget *w_grid = gtk_grid_new();

	lea_panel_construct(LEA_PANEL(result), panel_owner, frame, cat_string_wo_new_with("Digit panel"), w_grid);

	GtkWidget *but = gtk_button_new_with_label("disable some actions");

	gtk_grid_attach(GTK_GRID(w_grid), but, 0,0, 1,1);
	gtk_widget_set_hexpand_set(but, TRUE);

	g_signal_connect(but, "clicked", G_CALLBACK(l_disbale_actions), panel_owner);


	but = gtk_button_new_with_label("One (1)");

	gtk_grid_attach(GTK_GRID(w_grid), but, 1,0, 1,1);
	gtk_widget_set_hexpand_set(but, TRUE);

	return result;
}
