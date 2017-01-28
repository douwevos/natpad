/*
   File:    leadigitpanelowner.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Mar 18, 2010
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

#include "leadigitpanelowner.h"
#include "leadigitpanelactiongroup.h"
#include "leadigitpanel.h"
#include "../../src/menu/leamenuaction.h"
#include "../../src/key/leaikeybinding.h"
#include "../../src/layout/leaipanelowner.h"
#include "action/leaactionscientific.h"
#include <caterpillar.h>

#include "logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaDigitPanelOwner"
#include "logging/catlog.h"

static void l_ipanel_owner_iface_init(LeaIPanelOwnerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaDigitPanelOwner, lea_digit_panel_owner, G_TYPE_OBJECT, {
	G_IMPLEMENT_INTERFACE(LEA_TYPE_IPANEL_OWNER, l_ipanel_owner_iface_init)
});

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void lea_digit_panel_owner_class_init(LeaDigitPanelOwnerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void lea_digit_panel_owner_init(LeaDigitPanelOwner *node) {
}

static void l_dispose(GObject *object) {
	LeaDigitPanelOwner *instance = LEA_DIGIT_PANEL_OWNER(object);
	if (instance->frame && instance->digit_action_group) {
//		lea_frame_unmerge_action_group(instance->frame, instance->digit_action_group);
	}

	cat_unref_ptr(instance->key_context);
	cat_unref_ptr(instance->e_panel_list);
	cat_unref_ptr(instance->digit_action_group);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_digit_panel_owner_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

LeaDigitPanelOwner *lea_digit_panel_owner_new(LeaFrame *frame) {
	LeaDigitPanelOwner *result = g_object_new(LEA_TYPE_DIGIT_PANEL_OWNER, NULL);
	cat_ref_anounce(result);
	result->frame = frame;
	result->e_panel_list = cat_array_wo_new();
	result->key_context = lea_key_context_new(cat_string_wo_new_with("Digit panel owner"));

	LeaActionGroup *digit_action_group = lea_action_group_new(cat_string_wo_new_with("digit.group"), cat_string_wo_new_with("_Digits"));
	result->digit_action_group = digit_action_group;
	LeaAction *act_scf = (LeaAction *) lea_action_scientific_new();
	lea_action_group_add(result->digit_action_group, act_scf);
	cat_unref_ptr(act_scf);


//	lea_frame_merge_action_group(frame, digit_action_group);

	return result;
}



LeaActionGroup *lea_digit_panel_owner_get_action_group(LeaDigitPanelOwner *panel_owner) {
	return panel_owner->digit_action_group;
}


static gboolean _ipanel_owner_panel_closing(LeaIPanelOwner *self, LeaPanel *panel) {
	LeaDigitPanelOwner *this = LEA_DIGIT_PANEL_OWNER(self);
	int index = cat_array_wo_find_index(this->e_panel_list, panel, -1);
	if (index>=0) {
		cat_array_wo_remove(this->e_panel_list, index, NULL);
	}
	return TRUE;
}

static void l_panel_owner_add_panel(LeaIPanelOwner *self, struct _LeaPanel *panel) {

}

static void l_panel_owner_remove_panel(LeaIPanelOwner *self, struct _LeaPanel *panel) {
}

static LeaKeyContext *l_panel_owner_get_key_context(LeaIPanelOwner *self) {
	LeaDigitPanelOwner *this = LEA_DIGIT_PANEL_OWNER(self);
	return this->key_context;
}

static void l_ipanel_owner_iface_init(LeaIPanelOwnerInterface *iface) {
	iface->panel_closing = _ipanel_owner_panel_closing;
	iface->addPanel = l_panel_owner_add_panel;
	iface->removePanel = l_panel_owner_remove_panel;
	iface->getKeyContext = l_panel_owner_get_key_context;
}
