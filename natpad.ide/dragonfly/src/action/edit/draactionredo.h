/*
   File:    draactionredo.h
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

#ifndef ACTION_EDIT_DRAACTIONREDO_H_
#define ACTION_EDIT_DRAACTIONREDO_H_

#include "../../draeditorpanel.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_REDO              (dra_action_redo_get_type())
#define DRA_ACTION_REDO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_redo_get_type(), DraActionRedo))
#define DRA_ACTION_REDO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_REDO, DraActionRedoClass))
#define DRA_IS_ACTION_REDO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_REDO))
#define DRA_IS_ACTION_REDO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_REDO))
#define DRA_ACTION_REDO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_REDO, DraActionRedoClass))


typedef struct _DraActionRedo               DraActionRedo;
typedef struct _DraActionRedoPrivate        DraActionRedoPrivate;
typedef struct _DraActionRedoClass          DraActionRedoClass;


struct _DraActionRedo {
	LeaAction parent;
};

struct _DraActionRedoClass {
	LeaActionClass parent_class;
};


GType dra_action_redo_get_type();

DraActionRedo *dra_action_redo_new();

void dra_action_redo_set_editor_panel(DraActionRedo *action, DraEditorPanel *editor_panel);
void dra_action_redo_set_has_redo(DraActionRedo *action, gboolean has_redo);

G_END_DECLS

#endif /* ACTION_EDIT_DRAACTIONREDO_H_ */
