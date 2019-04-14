/*
   File:    dragroupedit.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jun 20, 2015
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

#ifndef ACTION_DRAGROUPEDIT_H_
#define ACTION_DRAGROUPEDIT_H_

#include "../draeditorpanel.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define DRA_TYPE_GROUP_EDIT              (dra_group_edit_get_type())
#define DRA_GROUP_EDIT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_group_edit_get_type(), DraGroupEdit))
#define DRA_GROUP_EDIT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_GROUP_EDIT, DraGroupEditClass))
#define DRA_IS_GROUP_EDIT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_GROUP_EDIT))
#define DRA_IS_GROUP_EDIT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_GROUP_EDIT))
#define DRA_GROUP_EDIT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_GROUP_EDIT, DraGroupEditClass))


typedef struct _DraGroupEdit               DraGroupEdit;
typedef struct _DraGroupEditPrivate        DraGroupEditPrivate;
typedef struct _DraGroupEditClass          DraGroupEditClass;


struct _DraGroupEdit {
	LeaActionGroup parent;
};

struct _DraGroupEditClass {
	LeaActionGroupClass parent_class;
};


GType dra_group_edit_get_type();

DraGroupEdit *dra_group_edit_new(LeaKeyContext *key_context, gpointer clipboard_handler);

void dra_group_edit_set_editor_panel(DraGroupEdit *group_edit, DraEditorPanel *editor_panel);

void dra_group_edit_refresh_clipboard_has_text(DraGroupEdit *group_edit);

void dra_group_edit_set_has_selection(DraGroupEdit *group_edit, gboolean has_selection);
void dra_group_edit_set_has_clipboard(DraGroupEdit *group_edit, gboolean has_clipboard);

void dra_group_edit_set_history_info(DraGroupEdit *group_edit, int history_index, int history_length);
void dra_group_edit_set_line_ends(DraGroupEdit *group_edit, ChaLineEnd line_ends, gboolean line_ends_are_mixed);

G_END_DECLS

#endif /* ACTION_DRAGROUPEDIT_H_ */
