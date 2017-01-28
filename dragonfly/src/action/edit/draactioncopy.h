/*
   File:    draactioncopy.h
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

#ifndef ACTION_EDIT_DRAACTIONCOPY_H_
#define ACTION_EDIT_DRAACTIONCOPY_H_

#include "../draactionclipboard.h"
#include "../../draeditorpanel.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS


#define DRA_TYPE_ACTION_COPY              (dra_action_copy_get_type())
#define DRA_ACTION_COPY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_copy_get_type(), DraActionCopy))
#define DRA_ACTION_COPY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_COPY, DraActionCopyClass))
#define DRA_IS_ACTION_COPY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_COPY))
#define DRA_IS_ACTION_COPY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_COPY))
#define DRA_ACTION_COPY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_COPY, DraActionCopyClass))


typedef struct _DraActionCopy               DraActionCopy;
typedef struct _DraActionCopyPrivate        DraActionCopyPrivate;
typedef struct _DraActionCopyClass          DraActionCopyClass;


struct _DraActionCopy {
	LeaAction parent;
};

struct _DraActionCopyClass {
	LeaActionClass parent_class;
};


GType dra_action_copy_get_type();

DraActionCopy *dra_action_copy_new(gpointer clipboard_handler);

void dra_action_copy_set_editor_panel(DraActionCopy *action, DraEditorPanel *editor_panel);

void dra_action_copy_set_has_selection(DraActionCopy *action, gboolean has_selection);

G_END_DECLS

#endif /* ACTION_EDIT_DRAACTIONCOPY_H_ */
