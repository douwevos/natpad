/*
   File:    leaoutputpanel.c
   Project: natpad
   Author:  Douwe Vos
   Date:    Dec 26, 2009
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

#include "leaoutputpanel.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaOutputPanel"
#include <logging/catlog.h>

G_DEFINE_TYPE (LeaOutputPanel, lea_output_panel, LEA_TYPE_PANEL) // @suppress("Unused static function")

static void l_dispose(GObject *object);

static void lea_output_panel_class_init(LeaOutputPanelClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
}

static void lea_output_panel_init(LeaOutputPanel *instance) {
}

static void l_dispose(GObject *object) {
	G_OBJECT_CLASS(lea_output_panel_parent_class)->dispose(object);
//	cat_log_detail("dispose:%p", object);
//	LeaOutputPanel *instance = LEA_OUTPUT_PANEL(object);
//	cat_log_detail("end-dispose:%p", object);
}


LeaOutputPanel *lea_output_panel_new(LeaIPanelOwner *panel_owner, LeaFrame *frame) {
	LeaOutputPanel *result = g_object_new(LEA_TYPE_OUTPUT_PANEL, NULL);
	cat_ref_anounce(result);
	GtkWidget *widget = gtk_text_view_new();
	lea_panel_construct(LEA_PANEL(result), panel_owner, frame, cat_string_wo_new_with("Output"), widget);
	return result;
}
