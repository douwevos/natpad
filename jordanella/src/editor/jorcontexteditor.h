/*
   File:    jorcontexteditor.h
   Project: jordanella
   Author:  Douwe Vos
   Date:    3 Jul 2019
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

#ifndef EDITOR_JORCONTEXTEDITOR_H_
#define EDITOR_JORCONTEXTEDITOR_H_

#include "joreditorconnector.h"
#include <dragonfly.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define JOR_TYPE_CONTEXT_EDITOR              (jor_context_editor_get_type())
#define JOR_CONTEXT_EDITOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jor_context_editor_get_type(), JorContextEditor))
#define JOR_CONTEXT_EDITOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JOR_TYPE_CONTEXT_EDITOR, JorContextEditorClass))
#define JOR_IS_CONTEXT_EDITOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JOR_TYPE_CONTEXT_EDITOR))
#define JOR_IS_CONTEXT_EDITOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JOR_TYPE_CONTEXT_EDITOR))
#define JOR_CONTEXT_EDITOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JOR_TYPE_CONTEXT_EDITOR, JorContextEditorClass))

struct _JorContextEditor {
	DraContextEditor parent;
};

struct _JorContextEditorClass {
	DraContextEditorClass parent_class;
};

typedef struct _JorContextEditor               JorContextEditor;
typedef struct _JorContextEditorPrivate        JorContextEditorPrivate;
typedef struct _JorContextEditorClass          JorContextEditorClass;


GType jor_context_editor_get_type();

JorContextEditor *jor_context_editor_new(DraEditorPanel *editor_panel, JorEditorConnector *connector);

G_END_DECLS

#endif /* EDITOR_JORCONTEXTEDITOR_H_ */
