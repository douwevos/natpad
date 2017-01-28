/*
   File:    draactionmarkcopy.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Sep 2, 2015
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

#ifndef ACTION_SEARCH_DRAACTIONMARKCOPY_H_
#define ACTION_SEARCH_DRAACTIONMARKCOPY_H_

#include "../../draeditorpanel.h"
#include <leafhopper.h>
#include <caterpillar.h>
#include "../edit/draactioncopy.h"

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_MARK_COPY              (dra_action_mark_copy_get_type())
#define DRA_ACTION_MARK_COPY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_mark_copy_get_type(), DraActionMarkCopy))
#define DRA_ACTION_MARK_COPY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_MARK_COPY, DraActionMarkCopyClass))
#define DRA_IS_ACTION_MARK_COPY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_MARK_COPY))
#define DRA_IS_ACTION_MARK_COPY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_MARK_COPY))
#define DRA_ACTION_MARK_COPY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_MARK_COPY, DraActionMarkCopyClass))


typedef struct _DraActionMarkCopy               DraActionMarkCopy;
typedef struct _DraActionMarkCopyPrivate        DraActionMarkCopyPrivate;
typedef struct _DraActionMarkCopyClass          DraActionMarkCopyClass;


struct _DraActionMarkCopy {
	LeaAction parent;
};

struct _DraActionMarkCopyClass {
	LeaActionClass parent_class;
};


GType dra_action_mark_copy_get_type();

DraActionMarkCopy *dra_action_mark_copy_new(gpointer clipboard_handler);

void dra_action_mark_copy_set_editor(DraActionMarkCopy *action, DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* ACTION_SEARCH_DRAACTIONMARKCOPY_H_ */
