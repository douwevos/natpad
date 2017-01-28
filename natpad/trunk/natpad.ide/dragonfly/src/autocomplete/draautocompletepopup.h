/*
   File:    draautocompletepopup.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 17, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef AUTOCOMPLETE_DRAAUTOCOMPLETEPOPUP_H_
#define AUTOCOMPLETE_DRAAUTOCOMPLETEPOPUP_H_

#include "draautocompletepanel.h"
#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_AUTO_COMPLETE_POPUP              (dra_auto_complete_popup_get_type())
#define DRA_AUTO_COMPLETE_POPUP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_auto_complete_popup_get_type(), DraAutoCompletePopup))
#define DRA_AUTO_COMPLETE_POPUP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_AUTO_COMPLETE_POPUP, DraAutoCompletePopupClass))
#define DRA_IS_AUTO_COMPLETE_POPUP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_AUTO_COMPLETE_POPUP))
#define DRA_IS_AUTO_COMPLETE_POPUP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_AUTO_COMPLETE_POPUP))
#define DRA_AUTO_COMPLETE_POPUP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_AUTO_COMPLETE_POPUP, DraAutoCompletePopupClass))


typedef struct _DraAutoCompletePopup               DraAutoCompletePopup;
typedef struct _DraAutoCompletePopupPrivate        DraAutoCompletePopupPrivate;
typedef struct _DraAutoCompletePopupClass          DraAutoCompletePopupClass;


struct _DraAutoCompletePopup {
	GObject parent;
};

struct _DraAutoCompletePopupClass {
	GObjectClass parent_class;
};


GType dra_auto_complete_popup_get_type();

DraAutoCompletePopup *dra_auto_complete_popup_new(DraEditor *editor, DraAutoCompletePanel *auto_complete_panel);

GtkWindow *dra_auto_complete_popup_get_window(DraAutoCompletePopup *auto_complete_popup);

void dra_auto_complete_popup_set_bounds(DraAutoCompletePopup *auto_complete_popup, GtkAllocation *alloc);

void dra_auto_complete_popup_forward_event(DraAutoCompletePopup *auto_complete_popup, GdkEvent *eev);

void dra_auto_complete_popup_show(DraAutoCompletePopup *auto_complete_popup);

G_END_DECLS

#endif /* AUTOCOMPLETE_DRAAUTOCOMPLETEPOPUP_H_ */
