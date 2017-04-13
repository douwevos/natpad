/*
   File:    draactionopendeclaration.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Apr 8, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef ACTION_EDIT_DRAACTIONOPENDECLARATION_H_
#define ACTION_EDIT_DRAACTIONOPENDECLARATION_H_

#include "../../draeditorpanel.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_OPEN_DECLARATION              (dra_action_open_declaration_get_type())
#define DRA_ACTION_OPEN_DECLARATION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_open_declaration_get_type(), DraActionOpenDeclaration))
#define DRA_ACTION_OPEN_DECLARATION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_OPEN_DECLARATION, DraActionOpenDeclarationClass))
#define DRA_IS_ACTION_OPEN_DECLARATION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_OPEN_DECLARATION))
#define DRA_IS_ACTION_OPEN_DECLARATION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_OPEN_DECLARATION))
#define DRA_ACTION_OPEN_DECLARATION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_OPEN_DECLARATION, DraActionOpenDeclarationClass))


typedef struct _DraActionOpenDeclaration               DraActionOpenDeclaration;
typedef struct _DraActionOpenDeclarationPrivate        DraActionOpenDeclarationPrivate;
typedef struct _DraActionOpenDeclarationClass          DraActionOpenDeclarationClass;


struct _DraActionOpenDeclaration {
	LeaAction parent;
};

struct _DraActionOpenDeclarationClass {
	LeaActionClass parent_class;
};


GType dra_action_open_declaration_get_type();

DraActionOpenDeclaration *dra_action_open_declaration_new();

void dra_action_open_declaration_set_editor_panel(DraActionOpenDeclaration *action, DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* ACTION_EDIT_DRAACTIONOPENDECLARATION_H_ */
