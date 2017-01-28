/*
   File:    draactionindent.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 13, 2015
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

#ifndef ACTION_EDIT_DRAACTIONINDENT_H_
#define ACTION_EDIT_DRAACTIONINDENT_H_

#include "../../draeditorpanel.h"
#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_INDENT              (dra_action_indent_get_type())
#define DRA_ACTION_INDENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_indent_get_type(), DraActionIndent))
#define DRA_ACTION_INDENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_INDENT, DraActionIndentClass))
#define DRA_IS_ACTION_INDENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_INDENT))
#define DRA_IS_ACTION_INDENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_INDENT))
#define DRA_ACTION_INDENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_INDENT, DraActionIndentClass))


typedef struct _DraActionIndent               DraActionIndent;
typedef struct _DraActionIndentPrivate        DraActionIndentPrivate;
typedef struct _DraActionIndentClass          DraActionIndentClass;


struct _DraActionIndent {
	LeaAction parent;
};

struct _DraActionIndentClass {
	LeaActionClass parent_class;
};


GType dra_action_indent_get_type();

DraActionIndent *dra_action_indent_new();

void dra_action_indent_set_editor_panel(DraActionIndent *action, DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* ACTION_EDIT_DRAACTIONINDENT_H_ */
