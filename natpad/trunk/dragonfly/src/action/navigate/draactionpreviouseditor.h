/*
   File:    draactionpreviouseditor.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 12, 2015
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

#ifndef ACTION_SEARCH_DRAACTIONPREVIOUSEDITOR_H_
#define ACTION_SEARCH_DRAACTIONPREVIOUSEDITOR_H_

#include "../../drapanelowner.h"
#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_PREVIOUS_EDITOR              (dra_action_previous_editor_get_type())
#define DRA_ACTION_PREVIOUS_EDITOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_previous_editor_get_type(), DraActionPreviousEditor))
#define DRA_ACTION_PREVIOUS_EDITOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_PREVIOUS_EDITOR, DraActionPreviousEditorClass))
#define DRA_IS_ACTION_PREVIOUS_EDITOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_PREVIOUS_EDITOR))
#define DRA_IS_ACTION_PREVIOUS_EDITOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_PREVIOUS_EDITOR))
#define DRA_ACTION_PREVIOUS_EDITOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_PREVIOUS_EDITOR, DraActionPreviousEditorClass))


typedef struct _DraActionPreviousEditor               DraActionPreviousEditor;
typedef struct _DraActionPreviousEditorPrivate        DraActionPreviousEditorPrivate;
typedef struct _DraActionPreviousEditorClass          DraActionPreviousEditorClass;


struct _DraActionPreviousEditor {
	LeaAction parent;
};

struct _DraActionPreviousEditorClass {
	LeaActionClass parent_class;
};


GType dra_action_previous_editor_get_type();

DraActionPreviousEditor *dra_action_previous_editor_new(DraPanelOwner *panel_owner);

G_END_DECLS

#endif /* ACTION_SEARCH_DRAACTIONPREVIOUSEDITOR_H_ */
