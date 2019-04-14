/*
   File:    draactionselectlineending.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Apr 13, 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

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

#ifndef ACTION_EDIT_DRAACTIONSELECTLINEENDING_H_
#define ACTION_EDIT_DRAACTIONSELECTLINEENDING_H_

#include "../../draeditorpanel.h"
#include <caterpillar.h>
#include <leafhopper.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_SELECT_LINE_ENDING              (dra_action_select_line_ending_get_type())
#define DRA_ACTION_SELECT_LINE_ENDING(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_select_line_ending_get_type(), DraActionSelectLineEnding))
#define DRA_ACTION_SELECT_LINE_ENDING_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_SELECT_LINE_ENDING, DraActionSelectLineEndingClass))
#define DRA_IS_ACTION_SELECT_LINE_ENDING(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_SELECT_LINE_ENDING))
#define DRA_IS_ACTION_SELECT_LINE_ENDING_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_SELECT_LINE_ENDING))
#define DRA_ACTION_SELECT_LINE_ENDING_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_SELECT_LINE_ENDING, DraActionSelectLineEndingClass))

struct _DraActionSelectLineEnding {
	LeaAction parent;
};

struct _DraActionSelectLineEndingClass {
	LeaActionClass parent_class;
};

typedef struct _DraActionSelectLineEnding               DraActionSelectLineEnding;
typedef struct _DraActionSelectLineEndingPrivate        DraActionSelectLineEndingPrivate;
typedef struct _DraActionSelectLineEndingClass          DraActionSelectLineEndingClass;


GType dra_action_select_line_ending_get_type();

DraActionSelectLineEnding *dra_action_select_line_ending_new();

void dra_action_select_line_ending_set_editor_panel(DraActionSelectLineEnding *action, DraEditorPanel *editor_panel);

void dra_action_select_line_ending_set_line_ends(DraActionSelectLineEnding *action, ChaLineEnd line_ends, gboolean line_ends_are_mixed);

G_END_DECLS

#endif /* ACTION_EDIT_DRAACTIONSELECTLINEENDING_H_ */
