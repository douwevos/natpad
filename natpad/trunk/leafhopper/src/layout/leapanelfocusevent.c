/*
 * leapanelfocusevent.c
 *
 *  Created on: Mar 18, 2010
 */

#include "leapanelfocusevent.h"
#include "leaipanelowner.h"
#include "leapanel.h"
#include <caterpillar.h>

#include "logging/catlogdefs.h"
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaPanelFocusEvent"
#include "logging/catlog.h"


G_DEFINE_TYPE(LeaPanelFocusEvent, lea_panel_focus_event, LEA_TYPE_EVENT)

static void _dispose(GObject *object);

static void lea_panel_focus_event_class_init(LeaPanelFocusEventClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = _dispose;
}

static void lea_panel_focus_event_init(LeaPanelFocusEvent *node) {
}

static void _dispose(GObject *object) {
	LeaPanelFocusEvent *instance = LEA_PANEL_FOCUS_EVENT(object);
	cat_unref_ptr(instance->old_panel_owner);
	cat_unref_ptr(instance->old_panel);
	cat_unref_ptr(instance->new_panel_owner);
	cat_unref_ptr(instance->new_panel);
	cat_unref_ptr(instance->focus_widget);
}

LeaPanelFocusEvent *lea_panel_focus_event_new(GtkWidget *focus_widget, LeaIPanelOwner *old_panel_owner, LeaPanel *old_panel, LeaIPanelOwner *new_panel_owner, LeaPanel *new_panel) {
	LeaPanelFocusEvent *result = g_object_new(LEA_TYPE_PANEL_FOCUS_EVENT, NULL);
	cat_ref_anounce(result);
	lea_event_construct((LeaEvent *) result);
	result->old_panel_owner = cat_ref_ptr(old_panel_owner);
	result->old_panel = cat_ref_ptr(old_panel);
	result->new_panel_owner = cat_ref_ptr(new_panel_owner);
	result->new_panel = cat_ref_ptr(new_panel);
	result->focus_widget = cat_ref_ptr(focus_widget);
	return result;
}
