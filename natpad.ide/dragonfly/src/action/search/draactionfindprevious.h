/*
   File:    draactionfindprevious.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 16, 2015
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

#ifndef ACTION_SEARCH_DRAACTIONFINDPREVIOUS_H_
#define ACTION_SEARCH_DRAACTIONFINDPREVIOUS_H_

#include "../../draeditorpanel.h"
#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_FIND_PREVIOUS              (dra_action_find_previous_get_type())
#define DRA_ACTION_FIND_PREVIOUS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_find_previous_get_type(), DraActionFindPrevious))
#define DRA_ACTION_FIND_PREVIOUS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_FIND_PREVIOUS, DraActionFindPreviousClass))
#define DRA_IS_ACTION_FIND_PREVIOUS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_FIND_PREVIOUS))
#define DRA_IS_ACTION_FIND_PREVIOUS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_FIND_PREVIOUS))
#define DRA_ACTION_FIND_PREVIOUS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_FIND_PREVIOUS, DraActionFindPreviousClass))


typedef struct _DraActionFindPrevious               DraActionFindPrevious;
typedef struct _DraActionFindPreviousPrivate        DraActionFindPreviousPrivate;
typedef struct _DraActionFindPreviousClass          DraActionFindPreviousClass;


struct _DraActionFindPrevious {
	LeaAction parent;
};

struct _DraActionFindPreviousClass {
	LeaActionClass parent_class;
};


GType dra_action_find_previous_get_type();

DraActionFindPrevious *dra_action_find_previous_new();

void dra_action_find_previous_set_editor_panel(DraActionFindPrevious *action, DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* ACTION_SEARCH_DRAACTIONFINDPREVIOUS_H_ */
