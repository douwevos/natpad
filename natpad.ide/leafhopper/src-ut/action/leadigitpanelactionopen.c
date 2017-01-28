/*
   File:    leaactionopen.c
   Project: natpad
   Author:  Douwe Vos
   Date:    Dec 24, 2009
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

#include "leadigitpanelactionopen.h"
#include <caterpillar.h>
#include <gdk/gdkkeysyms.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaDigitPanelActionOpen"
#include <logging/catlog.h>

G_DEFINE_TYPE (LeaDigitPanelActionOpen, lea_digit_panel_action_open, LEA_TYPE_ACTION)

static void _run(LeaAction *self);

static void _dispose(GObject *object);

static void lea_digit_panel_action_open_class_init(LeaDigitPanelActionOpenClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = _run;
}

static void lea_digit_panel_action_open_init(LeaDigitPanelActionOpen *instance) {
}

static void _dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
//	LeaDigitPanelActionOpen *instance = LEA_DIGIT_PANEL_ACTION_OPEN(object);
//	cat_log_detail("end-dispose:%p", object);
}




LeaDigitPanelActionOpen *lea_digit_panel_action_open_new() {
	LeaDigitPanelActionOpen *result = g_object_new(LEA_TYPE_DIGIT_PANEL_ACTION_OPEN, NULL);
	lea_action_construct(LEA_ACTION(result), cat_string_wo_new_with("Open"), cat_string_wo_new_with("Open"), cat_string_wo_new_with("gtk-open"));
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(cat_string_wo_new_with("Ctrl+O")));
	return result;
}



static void _run(LeaAction *self) {
	cat_log_debug("should run open");
}

