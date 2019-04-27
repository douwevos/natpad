/*
 * leapanelfocusevent.h
 *
 *  Created on: Mar 18, 2010
 */

#ifndef LEAPANELFOCUSEVENT_H_
#define LEAPANELFOCUSEVENT_H_

#include "../event/leaevent.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>

G_BEGIN_DECLS

#define LEA_TYPE_PANEL_FOCUS_EVENT            (lea_panel_focus_event_get_type())
#define LEA_PANEL_FOCUS_EVENT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_panel_focus_event_get_type(), LeaPanelFocusEvent))
#define LEA_PANEL_FOCUS_EVENT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_PANEL_FOCUS_EVENT, LeaPanelFocusEventClass))
#define LEA_IS_PANEL_FOCUS_EVENT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_PANEL_FOCUS_EVENT))
#define LEA_IS_PANEL_FOCUS_EVENT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_PANEL_FOCUS_EVENT))
#define LEA_PANEL_FOCUS_EVENT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_PANEL_FOCUS_EVENT, LeaPanelFocusEventClass))


typedef struct _LeaPanelFocusEvent             LeaPanelFocusEvent;

typedef struct _LeaPanelFocusEventClass        LeaPanelFocusEventClass;

struct _LeaPanelFocusEvent {
	LeaEvent parent;
	struct _LeaIPanelOwner *old_panel_owner;
	struct _LeaPanel *old_panel;
	struct _LeaIPanelOwner *new_panel_owner;
	struct _LeaPanel *new_panel;
	GtkWidget *focus_widget;
};

struct _LeaPanelFocusEventClass {
	LeaEventClass parent_class;
};


GType lea_panel_focus_event_get_type(void);

LeaPanelFocusEvent *lea_panel_focus_event_new(GtkWidget *focus_widget, struct _LeaIPanelOwner *old_panel_owner, struct _LeaPanel *old_panel, struct _LeaIPanelOwner *new_panel_owner, struct _LeaPanel *new_panel);

G_END_DECLS

#endif /* LEAPANELFOCUSEVENT_H_ */
