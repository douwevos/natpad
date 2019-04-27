/*
   File:    elkcompletionpopup.h
   Project: astarte
   Author:  Douwe Vos
   Date:    Sep 17, 2010
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

#ifndef ELKCOMPLETIONPOPUP_H_
#define ELKCOMPLETIONPOPUP_H_

#include "elkcompletionpanel.h"
#include <gdk/gdk.h>
#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ELK_TYPE_COMPLETION_POPUP            (elk_completion_popup_get_type())
#define ELK_COMPLETION_POPUP(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_completion_popup_get_type(), ElkCompletionPopup))
#define ELK_COMPLETION_POPUP_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_COMPLETION_POPUP, ElkCompletionPopupClass))
#define ELK_IS_COMPLETION_POPUP(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_COMPLETION_POPUP))
#define ELK_IS_COMPLETION_POPUP_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_COMPLETION_POPUP))
#define ELK_COMPLETION_POPUP_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_COMPLETION_POPUP, ElkCompletionPopupClass))

typedef struct _ElkCompletionPopup       ElkCompletionPopup;
typedef struct _ElkCompletionPopupClass  ElkCompletionPopupClass;


struct _ElkCompletionPopup {
	GObject parent;
	GtkWindow *popup_window;
	GtkWidget *widget;
	GtkWidget *scrolled_window;
	ElkCompletionPanel *completion_panel;

	int count;
	int xpos, ypos;

	gboolean has_grab;
};


struct _ElkCompletionPopupClass {
	GObjectClass parent_class;
};


GType elk_completion_popup_get_type(void);

ElkCompletionPopup *elk_completion_popup_new(GtkWidget *widget, ElkCompletionPanel *completion_panel);

void elk_completion_popup_show(ElkCompletionPopup *popup);

G_END_DECLS

#endif /* ELKCOMPLETIONPOPUP_H_ */
