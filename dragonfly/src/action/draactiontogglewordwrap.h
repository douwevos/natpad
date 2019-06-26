/*
   File:    draactiontogglewordwrap.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Jun 24, 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

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

#ifndef ACTION_DRAACTIONTOGGLEWORDWRAP_H_
#define ACTION_DRAACTIONTOGGLEWORDWRAP_H_

#include "../draeditorpanel.h"
#include "../preferences/drapreferenceswo.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_TOGGLE_WORD_WRAP              (dra_action_toggle_word_wrap_get_type())
#define DRA_ACTION_TOGGLE_WORD_WRAP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_toggle_word_wrap_get_type(), DraActionToggleWordWrap))
#define DRA_ACTION_TOGGLE_WORD_WRAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_TOGGLE_WORD_WRAP, DraActionToggleWordWrapClass))
#define DRA_IS_ACTION_TOGGLE_WORD_WRAP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_TOGGLE_WORD_WRAP))
#define DRA_IS_ACTION_TOGGLE_WORD_WRAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_TOGGLE_WORD_WRAP))
#define DRA_ACTION_TOGGLE_WORD_WRAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_TOGGLE_WORD_WRAP, DraActionToggleWordWrapClass))

struct _DraActionToggleWordWrap {
	LeaAction parent;
};

struct _DraActionToggleWordWrapClass {
	LeaActionClass parent_class;
};

typedef struct _DraActionToggleWordWrap               DraActionToggleWordWrap;
typedef struct _DraActionToggleWordWrapPrivate        DraActionToggleWordWrapPrivate;
typedef struct _DraActionToggleWordWrapClass          DraActionToggleWordWrapClass;


GType dra_action_toggle_word_wrap_get_type();

DraActionToggleWordWrap *dra_action_toggle_word_wrap_new();

void dra_action_toggle_word_wrap_set_editor_panel(DraActionToggleWordWrap *action, DraEditorPanel *editor_panel);

void dra_action_toggle_word_wrap_reconfigured(DraActionToggleWordWrap *action, DraPreferencesWo *a_prefs);

G_END_DECLS

#endif /* ACTION_DRAACTIONTOGGLEWORDWRAP_H_ */
