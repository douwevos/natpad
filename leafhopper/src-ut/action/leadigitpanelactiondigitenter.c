/*
   File:    leaactiondigitenter.c
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

#include "leadigitpanelactiondigitenter.h"
#include <caterpillar.h>
#include <gdk/gdkkeysyms.h>
#include "../../src/leafhopper.h"
#include "../gtkwidgetdump.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaDigitPanelActionDigitEnter"
#include <logging/catlog.h>

static void _run(LeaAction *self);

G_DEFINE_TYPE(LeaDigitPanelActionDigitEnter, lea_digit_panel_action_digit_enter, LEA_TYPE_ACTION)

static void _dispose(GObject *object);

static void lea_digit_panel_action_digit_enter_class_init(LeaDigitPanelActionDigitEnterClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;

	LeaActionClass *action_class = LEA_ACTION_CLASS(clazz);
	action_class->action_run = _run;
}

static void lea_digit_panel_action_digit_enter_init(LeaDigitPanelActionDigitEnter *instance) {
}

static void _dispose(GObject *object) {
//	cat_log_detail("dispose:%p", object);
//	LeaDigitPanelActionDigitEnter *instance = LEA_DIGIT_PANEL_ACTION_DIGIT_ENTER(object);
//	cat_log_detail("end-dispose:%p", object);
}

struct _DigitText {
	char *short_text;
	char *long_text;
};

typedef struct _DigitText DigitText;

static DigitText digit_text[] = {
	{ "0", "Zero" },
	{ "1", "One" },
	{ "2", "Two" },
	{ "3", "Three" },
	{ "4", "Four" },
	{ "5", "Five" },
	{ "6", "Six" },
	{ "7", "Seven" },
	{ "8", "Eight" },
	{ "9", "Nine" },
};


LeaDigitPanelActionDigitEnter *lea_digit_panel_action_digit_enter_new(LeaFrame *frame, int digit) {
	LeaDigitPanelActionDigitEnter *result = g_object_new(LEA_TYPE_DIGIT_PANEL_ACTION_DIGIT_ENTER, NULL);
	result->digit = digit;
	result->frame = frame;
	DigitText *dtext = &digit_text[digit];
	CatStringWo *stext = cat_string_wo_new_with(dtext->short_text);
	CatStringWo *ltext = cat_string_wo_new_with(dtext->long_text);
	lea_action_construct(LEA_ACTION(result), ltext, stext, cat_string_wo_new_with("gtk-copy"));
	CatStringWo *e_seq = cat_string_wo_new_with(NULL);
	cat_string_wo_append_unichar(e_seq, GDK_KEY_0+digit);
	lea_action_set_default_key_sequence(LEA_ACTION(result), lea_key_sequence_from_string(e_seq));
	return result;
}



static void _run(LeaAction *self) {
	LeaDigitPanelActionDigitEnter *this = LEA_DIGIT_PANEL_ACTION_DIGIT_ENTER(self);
	cat_log_debug("should run digit");
	gtk_widget_dump(GTK_WIDGET(this->frame));
}

