/*
   File:    leaoutputpanelowner.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    May 13, 2010
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


#include <caterpillar.h>

#include "leaoutputpanelowner.h"
//#include "leadigitpanelactiongroup.h"
#include "leaoutputactionreset.h"
#include "leaoutputactionclear.h"
#include "leaoutputpanel.h"
#include "../../src/menu/leamenuaction.h"
#include "../../src/key/leaikeybinding.h"
#include "../../src/layout/leaipanelowner.h"
#include "../../src/action/leaactiongroup.h"


#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaOutputPanelOwner"
#include <logging/catlog.h>



static void _check_action_group(LeaOutputPanelOwner *digit_panel_owner, gboolean should_be_active);

static void l_ipanel_owner_iface_init(LeaIPanelOwnerInterface *iface);

G_DEFINE_TYPE_WITH_CODE(LeaOutputPanelOwner, lea_output_panel_owner, G_TYPE_OBJECT, {
			G_IMPLEMENT_INTERFACE(LEA_TYPE_IPANEL_OWNER, l_ipanel_owner_iface_init)
});

static void _dispose(GObject *object);

static void lea_output_panel_owner_class_init(LeaOutputPanelOwnerClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void lea_output_panel_owner_init(LeaOutputPanelOwner *instance) {
}

static void _dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaOutputPanelOwner *instance = LEA_OUTPUT_PANEL_OWNER(object);

	if (instance->frame && instance->action_group) {
		lea_frame_unmerge_action_group(instance->frame, instance->action_group);
	}

	cat_unref_ptr(instance->action_group);
	cat_unref_ptr(instance->e_panel_list);
	cat_unref_ptr(instance->frame);
	cat_log_detail("end-dispose:%p", object);
}


static LeaActionGroup *l_create_simple_action_group(LeaOutputPanelOwner *owner);

LeaOutputPanelOwner *lea_output_panel_owner_new(LeaFrame *frame) {
	LeaOutputPanelOwner *result = g_object_new(LEA_TYPE_OUTPUT_PANEL_OWNER, NULL);
	cat_ref_anounce(result);
	result->e_panel_list = cat_array_wo_new();
	result->frame = cat_ref_ptr(frame);
	result->action_group = l_create_simple_action_group(result);

	lea_frame_merge_action_group(frame, result->action_group);

	return result;
}

static LeaActionGroup *l_create_simple_action_group(LeaOutputPanelOwner *owner) {



	LeaActionGroup *lea_test_group = lea_action_group_new(NULL, NULL);
	LeaActionGroup *result = lea_action_group_new(cat_string_wo_new_with("_Window"), cat_string_wo_new_with("_Window"));

	lea_action_group_add(lea_test_group, (LeaAction *) result);

	LeaAction *reset_action = (LeaAction *) lea_output_action_reset_new((LeaIPanelOwner *) owner);
	lea_action_group_add(result, reset_action);
	cat_unref(reset_action);

	LeaAction *clear_action = (LeaAction *) lea_output_action_clear_new();
	lea_action_group_add(result, clear_action);
	cat_unref(clear_action);


	cat_unref(result);

//	lea_action_group_add(result, reset_action);
//
//
//
//	LeaMenuBar *menu_bar = lea_frame_get_menu_bar(frame);
//	LeaMenuAction *menu_action = lea_menu_action_new(reset_action, 1);
//	lea_menu_bar_add_menu_item(menu_bar, cat_string_wo_new_with("Project:0/File:0/"), (LeaIMenuItem *) menu_action);

	return lea_test_group;
}



static void _check_action_group(LeaOutputPanelOwner *output_panel_owner, gboolean should_be_active) {
	int panel_count = cat_array_wo_size(output_panel_owner->e_panel_list);
	if (should_be_active) {
		if (panel_count==0) {
//			LeaDigitPanelActionGroup *group = lea_output_panel_action_group_new();
//			LeaFrame *frame;
//			lea_register_action_group(lea, group);
//
//			LeaAction *action = lea_action_group_get(LEA_ACTION_GROUP(group), cat_string_wo_new_with("Open"));
//			LeaMenuAction *menu_action = lea_menu_action_new(action,0);
//			lea_menu_bar_add_menu_item(lea->menu_bar, cat_string_wo_new_with("Project:0/File:0/"), menu_action);
//
//			action = lea_action_group_get(LEA_ACTION_GROUP(group), cat_string_wo_new_with("Zero"));
//			cat_log_debug("action[0]=%lx", (gulong) action);
//			LeaIKeyBinding *key_binding = LEA_IKEY_BINDING(action);
//			lea_key_handler_add_binding(lea->key_handler, key_binding);
//
//			menu_action = lea_menu_action_new(action, 1);
//			lea_menu_bar_add_menu_item(lea->menu_bar, cat_string_wo_new_with("Project:1/"), menu_action);


		}
	} else {
		if (panel_count==0) {
//			lea_unregister_action_group(output_panel_owner->lea, cat_string_wo_new_with("DigitPanelAG"));
		}

	}
}












static gboolean l_panel_owner_panel_closing(LeaIPanelOwner *self, LeaPanel *panel) {
	LeaOutputPanelOwner *this = LEA_OUTPUT_PANEL_OWNER(self);
	int index = cat_array_wo_find_index(this->e_panel_list, panel, -1);
	if (index>=0) {
		cat_array_wo_remove(this->e_panel_list, index, NULL);
	}
	_check_action_group(this, FALSE);
	return TRUE;
}


static void l_panel_owner_add_panel(LeaIPanelOwner *self, struct _LeaPanel *panel) {
}

static void l_panel_owner_remove_panel(LeaIPanelOwner *self, struct _LeaPanel *panel) {
}

static LeaKeyContext *l_panel_owner_get_key_context(LeaIPanelOwner *self) {
	return NULL;
}


static void l_ipanel_owner_iface_init(LeaIPanelOwnerInterface *iface) {
	iface->panel_closing = l_panel_owner_panel_closing;
	iface->addPanel = l_panel_owner_add_panel;
	iface->removePanel = l_panel_owner_remove_panel;
	iface->getKeyContext = l_panel_owner_get_key_context;
}
