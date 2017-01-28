/*
   File:    draactionundo.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Aug 30, 2015
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

#ifndef ACTION_EDIT_DRAACTIONUNDO_H_
#define ACTION_EDIT_DRAACTIONUNDO_H_

#include "../../draeditorpanel.h"
#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_UNDO              (dra_action_undo_get_type())
#define DRA_ACTION_UNDO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_undo_get_type(), DraActionUndo))
#define DRA_ACTION_UNDO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_UNDO, DraActionUndoClass))
#define DRA_IS_ACTION_UNDO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_UNDO))
#define DRA_IS_ACTION_UNDO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_UNDO))
#define DRA_ACTION_UNDO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_UNDO, DraActionUndoClass))


typedef struct _DraActionUndo               DraActionUndo;
typedef struct _DraActionUndoPrivate        DraActionUndoPrivate;
typedef struct _DraActionUndoClass          DraActionUndoClass;


struct _DraActionUndo {
	LeaAction parent;
};

struct _DraActionUndoClass {
	LeaActionClass parent_class;
};


GType dra_action_undo_get_type();

DraActionUndo *dra_action_undo_new();

void dra_action_undo_set_editor_panel(DraActionUndo *action, DraEditorPanel *editor_panel);

void dra_action_undo_set_has_undo(DraActionUndo *action, gboolean has_undo);

G_END_DECLS

#endif /* ACTION_EDIT_DRAACTIONUNDO_H_ */
