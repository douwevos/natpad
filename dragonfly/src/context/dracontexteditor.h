/*
   File:    dracontexteditor.h
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

#ifndef CONTEXT_DRACONTEXTEDITOR_H_
#define CONTEXT_DRACONTEXTEDITOR_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_CONTEXT_EDITOR              (dra_context_editor_get_type())
#define DRA_CONTEXT_EDITOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_context_editor_get_type(), DraContextEditor))
#define DRA_CONTEXT_EDITOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_CONTEXT_EDITOR, DraContextEditorClass))
#define DRA_IS_CONTEXT_EDITOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_CONTEXT_EDITOR))
#define DRA_IS_CONTEXT_EDITOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_CONTEXT_EDITOR))
#define DRA_CONTEXT_EDITOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_CONTEXT_EDITOR, DraContextEditorClass))


typedef struct _DraContextEditor               DraContextEditor;
typedef struct _DraContextEditorPrivate        DraContextEditorPrivate;
typedef struct _DraContextEditorClass          DraContextEditorClass;


struct _DraContextEditor {
	GObject parent;
};

struct _DraContextEditorClass {
	GObjectClass parent_class;
	void (*format)(DraContextEditor *context_editor);
};


GType dra_context_editor_get_type();

struct _DraEditorPanel;

void dra_context_editor_construct(DraContextEditor *context_editor, struct _DraEditorPanel *editor_panel);

DraContextEditor *dra_context_editor_new(struct _DraEditorPanel *editor_panel);

struct _DraEditorPanel *dra_context_editor_get_panel(DraContextEditor *context_editor);

gboolean dra_context_editor_indent(DraContextEditor *context_editor, gboolean increase_indent);

void dra_context_editor_toggle_line_comment(DraContextEditor *context_editor);

G_END_DECLS

#endif /* CONTEXT_DRACONTEXTEDITOR_H_ */
