/*
   File:    draactionmarkcut.h
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

#ifndef ACTION_SEARCH_DRAACTIONMARKCUT_H_
#define ACTION_SEARCH_DRAACTIONMARKCUT_H_

#include "../../draeditorpanel.h"
#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_MARK_CUT              (dra_action_mark_cut_get_type())
#define DRA_ACTION_MARK_CUT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_mark_cut_get_type(), DraActionMarkCut))
#define DRA_ACTION_MARK_CUT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_MARK_CUT, DraActionMarkCutClass))
#define DRA_IS_ACTION_MARK_CUT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_MARK_CUT))
#define DRA_IS_ACTION_MARK_CUT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_MARK_CUT))
#define DRA_ACTION_MARK_CUT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_MARK_CUT, DraActionMarkCutClass))


typedef struct _DraActionMarkCut               DraActionMarkCut;
typedef struct _DraActionMarkCutPrivate        DraActionMarkCutPrivate;
typedef struct _DraActionMarkCutClass          DraActionMarkCutClass;


struct _DraActionMarkCut {
	LeaAction parent;
};

struct _DraActionMarkCutClass {
	LeaActionClass parent_class;
};


GType dra_action_mark_cut_get_type();

DraActionMarkCut *dra_action_mark_cut_new();

void dra_action_mark_cut_set_editor(DraActionMarkCut *action, DraEditorPanel *editor_panel);

G_END_DECLS


#endif /* ACTION_SEARCH_DRAACTIONMARKCUT_H_ */
