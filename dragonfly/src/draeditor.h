/*
   File:    draeditor.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Mar 22, 2015
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

#ifndef DRAEDITOR_H_
#define DRAEDITOR_H_

#include "context/dracontexteditor.h"
#include "document/draconnectormap.h"
#include "document/draiconnectorrequestfactory.h"
#include <chameleon.h>
#include <caterpillar.h>
#include <worm.h>

G_BEGIN_DECLS

#define DRA_TYPE_EDITOR              (dra_editor_get_type())
#define DRA_EDITOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_editor_get_type(), DraEditor))
#define DRA_EDITOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_EDITOR, DraEditorClass))
#define DRA_IS_EDITOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_EDITOR))
#define DRA_IS_EDITOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_EDITOR))
#define DRA_EDITOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_EDITOR, DraEditorClass))


typedef struct _DraEditor               DraEditor;
typedef struct _DraEditorPrivate        DraEditorPrivate;
typedef struct _DraEditorClass          DraEditorClass;


struct _DraEditor {
	ChaEditor parent;
};

struct _DraEditorClass {
	ChaEditorClass parent_class;
	void (*showContextMenu)(DraEditor *editor, ChaCursorWo *cursor, int xpos, int ypos, DraLineTagWo *spell_tag, GdkEvent *event);
};

struct _DraEditorPanel;

GType dra_editor_get_type();

void dra_editor_construct(DraEditor *instance, ChaDocument *document, DraConnectorMap *connector_map, DraIConnectorRequestFactory *connector_factory, WorService *wor_service);

DraEditor *dra_editor_new(ChaDocument *document, DraConnectorMap *connector_map, DraIConnectorRequestFactory *connector_factory, WorService *wor_service);

struct _DraEditorPanel *dra_editor_get_panel(DraEditor *editor);

void dra_editor_set_preferences(DraEditor *editor, DraPreferencesWo *a_prefs);

void dra_editor_request_mark_occurrences(DraEditor *editor);

void dra_editor_set_occurrence_text(DraEditor *editor, CatStringWo *text);

void dra_editor_set_focus_active(DraEditor *editor, gboolean focus_active);

void dra_editor_goto_line(DraEditor *editor);

void dra_editor_set_context_editor(DraEditor *editor, DraContextEditor *context_editor);
DraContextEditor *dra_editor_get_context_editor(DraEditor *editor);

G_END_DECLS

#endif /* DRAEDITOR_H_ */
