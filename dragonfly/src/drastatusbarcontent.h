/*
   File:    drastatusbarcontent.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 15, 2015
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

#ifndef DRASTATUSBARCONTENT_H_
#define DRASTATUSBARCONTENT_H_

#include "draeditorpanel.h"
#include <leafhopper.h>
#include <gtk/gtk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_STATUS_BAR_CONTENT              (dra_status_bar_content_get_type())
#define DRA_STATUS_BAR_CONTENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_status_bar_content_get_type(), DraStatusBarContent))
#define DRA_STATUS_BAR_CONTENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_STATUS_BAR_CONTENT, DraStatusBarContentClass))
#define DRA_IS_STATUS_BAR_CONTENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_STATUS_BAR_CONTENT))
#define DRA_IS_STATUS_BAR_CONTENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_STATUS_BAR_CONTENT))
#define DRA_STATUS_BAR_CONTENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_STATUS_BAR_CONTENT, DraStatusBarContentClass))


typedef struct _DraStatusBarContent               DraStatusBarContent;
typedef struct _DraStatusBarContentPrivate        DraStatusBarContentPrivate;
typedef struct _DraStatusBarContentClass          DraStatusBarContentClass;


struct _DraStatusBarContent {
	GtkBox parent;
};

struct _DraStatusBarContentClass {
	GtkBoxClass parent_class;
};


GType dra_status_bar_content_get_type();

DraStatusBarContent *dra_status_bar_content_new(LeaAction *action_toggle_word_wrap, LeaAction *action_toggle_show_whitespaces
		, LeaAction *action_toggle_mark_occurrences, LeaAction *action_toggle_spelling);

void dra_status_bar_content_set_active_editor_panel(DraStatusBarContent *status_bar_content, DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* DRASTATUSBARCONTENT_H_ */
