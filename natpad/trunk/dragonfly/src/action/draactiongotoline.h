/*
   File:    draactiongotoline.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Sep 1, 2015
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

#ifndef ACTION_DRAACTIONGOTOLINE_H_
#define ACTION_DRAACTIONGOTOLINE_H_

#include "../draeditorpanel.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_GOTO_LINE              (dra_action_goto_line_get_type())
#define DRA_ACTION_GOTO_LINE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_goto_line_get_type(), DraActionGotoLine))
#define DRA_ACTION_GOTO_LINE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_GOTO_LINE, DraActionGotoLineClass))
#define DRA_IS_ACTION_GOTO_LINE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_GOTO_LINE))
#define DRA_IS_ACTION_GOTO_LINE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_GOTO_LINE))
#define DRA_ACTION_GOTO_LINE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_GOTO_LINE, DraActionGotoLineClass))


typedef struct _DraActionGotoLine               DraActionGotoLine;
typedef struct _DraActionGotoLinePrivate        DraActionGotoLinePrivate;
typedef struct _DraActionGotoLineClass          DraActionGotoLineClass;


struct _DraActionGotoLine {
	LeaAction parent;
};

struct _DraActionGotoLineClass {
	LeaActionClass parent_class;
};


GType dra_action_goto_line_get_type();

DraActionGotoLine *dra_action_goto_line_new();

void dra_action_goto_line_set_editor_panel(DraActionGotoLine *action, DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* ACTION_DRAACTIONGOTOLINE_H_ */
