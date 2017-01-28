/*
   File:    jagcontexteditor.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Oct 19, 2015
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

#ifndef SRC_EDITOR_JAGCONTEXTEDITOR_H_
#define SRC_EDITOR_JAGCONTEXTEDITOR_H_

#include "../indexer/jagindexer.h"
#include <dragonfly.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_CONTEXT_EDITOR              (jag_context_editor_get_type())
#define JAG_CONTEXT_EDITOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_context_editor_get_type(), JagContextEditor))
#define JAG_CONTEXT_EDITOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_CONTEXT_EDITOR, JagContextEditorClass))
#define JAG_IS_CONTEXT_EDITOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_CONTEXT_EDITOR))
#define JAG_IS_CONTEXT_EDITOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_CONTEXT_EDITOR))
#define JAG_CONTEXT_EDITOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_CONTEXT_EDITOR, JagContextEditorClass))


typedef struct _JagContextEditor               JagContextEditor;
typedef struct _JagContextEditorPrivate        JagContextEditorPrivate;
typedef struct _JagContextEditorClass          JagContextEditorClass;


struct _JagContextEditor {
	DraContextEditor parent;
};

struct _JagContextEditorClass {
	DraContextEditorClass parent_class;
};


GType jag_context_editor_get_type();

JagContextEditor *jag_context_editor_new(DraEditorPanel *editor_panel, JagIndexer *indexer);

G_END_DECLS

#endif /* SRC_EDITOR_JAGCONTEXTEDITOR_H_ */
