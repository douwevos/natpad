/*
   File:    groedcontexteditor.h
   Project: gross-editor
   Author:  Douwe Vos
   Date:    Nov 20, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef EDITOR_GROEDCONTEXTEDITOR_H_
#define EDITOR_GROEDCONTEXTEDITOR_H_

#include <caterpillar.h>
#include <dragonfly.h>

G_BEGIN_DECLS

#define GROED_TYPE_CONTEXT_EDITOR              (groed_context_editor_get_type())
#define GROED_CONTEXT_EDITOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), groed_context_editor_get_type(), GroEdContextEditor))
#define GROED_CONTEXT_EDITOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GROED_TYPE_CONTEXT_EDITOR, GroEdContextEditorClass))
#define GROED_IS_CONTEXT_EDITOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GROED_TYPE_CONTEXT_EDITOR))
#define GROED_IS_CONTEXT_EDITOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GROED_TYPE_CONTEXT_EDITOR))
#define GROED_CONTEXT_EDITOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GROED_TYPE_CONTEXT_EDITOR, GroEdContextEditorClass))


typedef struct _GroEdContextEditor               GroEdContextEditor;
typedef struct _GroEdContextEditorPrivate        GroEdContextEditorPrivate;
typedef struct _GroEdContextEditorClass          GroEdContextEditorClass;


struct _GroEdContextEditor {
	DraContextEditor parent;
};

struct _GroEdContextEditorClass {
	DraContextEditorClass parent_class;
};

struct _DraEditorPanel;

GType groed_context_editor_get_type();

GroEdContextEditor *groed_context_editor_new(struct _DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* EDITOR_GROEDCONTEXTEDITOR_H_ */
