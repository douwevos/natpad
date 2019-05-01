/*
   File:    dragroupmacro.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 21, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef ACTION_DRAGROUPMACRO_H_
#define ACTION_DRAGROUPMACRO_H_

#include "../draeditorpanel.h"
#include "../drapanelowner.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define DRA_TYPE_GROUP_MACRO              (dra_group_macro_get_type())
#define DRA_GROUP_MACRO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_group_macro_get_type(), DraGroupMacro))
#define DRA_GROUP_MACRO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_GROUP_MACRO, DraGroupMacroClass))
#define DRA_IS_GROUP_MACRO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_GROUP_MACRO))
#define DRA_IS_GROUP_MACRO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_GROUP_MACRO))
#define DRA_GROUP_MACRO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_GROUP_MACRO, DraGroupMacroClass))


typedef struct _DraGroupMacro               DraGroupMacro;
typedef struct _DraGroupMacroPrivate        DraGroupMacroPrivate;
typedef struct _DraGroupMacroClass          DraGroupMacroClass;


struct _DraGroupMacro {
	LeaActionGroup parent;
};

struct _DraGroupMacroClass {
	LeaActionGroupClass parent_class;
};


GType dra_group_macro_get_type();

DraGroupMacro *dra_group_macro_new(LeaKeyContext *key_context, DraPanelOwner *panel_owner);

void dra_group_macro_set_editor_panel(DraGroupMacro *group_macro, DraEditorPanel *editor_panel);

void dra_group_macro_on_macro_state(DraGroupMacro *group_macro, gboolean is_recording, gboolean macro_available);

G_END_DECLS

#endif /* ACTION_DRAGROUPMACRO_H_ */
