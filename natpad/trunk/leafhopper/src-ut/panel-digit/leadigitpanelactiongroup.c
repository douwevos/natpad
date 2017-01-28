/*
   File:    leadigitpanelactiongroup.c
   Project: leafhopper
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


#include "leadigitpanelactiongroup.h"
#include "../action/leadigitpanelactionopen.h"
#include "../action/leadigitpanelactiondigitenter.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaDigitPanelActionGroup"
#include <logging/catlog.h>

G_DEFINE_TYPE (LeaDigitPanelActionGroup, lea_digit_panel_action_group, LEA_TYPE_ACTION_GROUP)

static void _dispose(GObject *object);

static void lea_digit_panel_action_group_class_init(LeaDigitPanelActionGroupClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void lea_digit_panel_action_group_init(LeaDigitPanelActionGroup *instance) {
}

static void _dispose(GObject *object) {
}


LeaDigitPanelActionGroup *lea_digit_panel_action_group_new(LeaFrame *frame) {
	LeaDigitPanelActionGroup *result = g_object_new(LEA_TYPE_DIGIT_PANEL_ACTION_GROUP, NULL);
	cat_ref_anounce(result);
	LeaActionGroup *group = (LeaActionGroup *) result;
	LeaAction *action;
	result->e_action_digits = cat_array_wo_new();
	lea_action_group_construct(group, cat_string_wo_new_with("DigitPanelAG"), cat_string_wo_new_with("_Digital Panels"));

	int idx = 0;
	for(idx=0; idx<10; idx++) {
		action = (LeaAction *) lea_digit_panel_action_digit_enter_new(frame, idx);
		cat_array_wo_append(result->e_action_digits, (GObject *) action);
		lea_action_group_add(group, action);
		cat_unref_ptr(action);
	}


	action = (LeaAction *) lea_digit_panel_action_open_new();
	lea_action_group_add(group, action);
	cat_unref_ptr(action);
	return result;
}
