/*
   File:    draactionmarkinvertall.h
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

#ifndef ACTION_SEARCH_DRAACTIONMARKINVERTALL_H_
#define ACTION_SEARCH_DRAACTIONMARKINVERTALL_H_

#include "../../draeditorpanel.h"
#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_MARK_INVERT_ALL              (dra_action_mark_invert_all_get_type())
#define DRA_ACTION_MARK_INVERT_ALL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_mark_invert_all_get_type(), DraActionMarkInvertAll))
#define DRA_ACTION_MARK_INVERT_ALL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_MARK_INVERT_ALL, DraActionMarkInvertAllClass))
#define DRA_IS_ACTION_MARK_INVERT_ALL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_MARK_INVERT_ALL))
#define DRA_IS_ACTION_MARK_INVERT_ALL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_MARK_INVERT_ALL))
#define DRA_ACTION_MARK_INVERT_ALL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_MARK_INVERT_ALL, DraActionMarkInvertAllClass))


typedef struct _DraActionMarkInvertAll               DraActionMarkInvertAll;
typedef struct _DraActionMarkInvertAllPrivate        DraActionMarkInvertAllPrivate;
typedef struct _DraActionMarkInvertAllClass          DraActionMarkInvertAllClass;


struct _DraActionMarkInvertAll {
	LeaAction parent;
};

struct _DraActionMarkInvertAllClass {
	LeaActionClass parent_class;
};


GType dra_action_mark_invert_all_get_type();

DraActionMarkInvertAll *dra_action_mark_invert_all_new();

void dra_action_mark_invert_all_set_editor(DraActionMarkInvertAll *action, DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* ACTION_SEARCH_DRAACTIONMARKINVERTALL_H_ */
