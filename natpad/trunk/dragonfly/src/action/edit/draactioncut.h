/*
   File:    draactioncut.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jun 21, 2015
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

#ifndef ACTION_EDIT_DRAACTIONCUT_H_
#define ACTION_EDIT_DRAACTIONCUT_H_

#include "../../draeditorpanel.h"
#include <caterpillar.h>
#include "draactioncopy.h"

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_CUT              (dra_action_cut_get_type())
#define DRA_ACTION_CUT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_cut_get_type(), DraActionCut))
#define DRA_ACTION_CUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_CUT, DraActionCutClass))
#define DRA_IS_ACTION_CUT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_CUT))
#define DRA_IS_ACTION_CUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_CUT))
#define DRA_ACTION_CUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_CUT, DraActionCutClass))


typedef struct _DraActionCut               DraActionCut;
typedef struct _DraActionCutPrivate        DraActionCutPrivate;
typedef struct _DraActionCutClass          DraActionCutClass;


struct _DraActionCut {
	LeaAction parent;
};

struct _DraActionCutClass {
	LeaActionClass parent_class;
};


GType dra_action_cut_get_type();

DraActionCut *dra_action_cut_new(gpointer clipboard_handler);

void dra_action_cut_set_editor_panel(DraActionCut *action, DraEditorPanel *editor_panel);

void dra_action_cut_set_has_selection(DraActionCut *action, gboolean has_selection);

G_END_DECLS

#endif /* ACTION_EDIT_DRAACTIONCUT_H_ */
