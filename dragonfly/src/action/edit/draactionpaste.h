/*
   File:    draactionpaste.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jun 21, 2015
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

#ifndef ACTION_EDIT_DRAACTIONPASTE_H_
#define ACTION_EDIT_DRAACTIONPASTE_H_

#include "../../draeditorpanel.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_PASTE              (dra_action_paste_get_type())
#define DRA_ACTION_PASTE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_paste_get_type(), DraActionPaste))
#define DRA_ACTION_PASTE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_PASTE, DraActionPasteClass))
#define DRA_IS_ACTION_PASTE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_PASTE))
#define DRA_IS_ACTION_PASTE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_PASTE))
#define DRA_ACTION_PASTE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_PASTE, DraActionPasteClass))


typedef struct _DraActionPaste               DraActionPaste;
typedef struct _DraActionPastePrivate        DraActionPastePrivate;
typedef struct _DraActionPasteClass          DraActionPasteClass;


struct _DraActionPaste {
	LeaAction parent;
};

struct _DraActionPasteClass {
	LeaActionClass parent_class;
};


GType dra_action_paste_get_type();

DraActionPaste *dra_action_paste_new();

void dra_action_paste_set_editor_panel(DraActionPaste *action, DraEditorPanel *editor_panel);

void dra_action_paste_set_has_clipboard(DraActionPaste *action, gboolean clipboard_has_text);

G_END_DECLS

#endif /* ACTION_EDIT_DRAACTIONPASTE_H_ */
