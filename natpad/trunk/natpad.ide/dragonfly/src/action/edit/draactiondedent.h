/*
   File:    draactiondedent.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 13, 2015
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

#ifndef ACTION_EDIT_DRAACTIONDEDENT_H_
#define ACTION_EDIT_DRAACTIONDEDENT_H_

#include "../../draeditorpanel.h"
#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_ACTION_DEDENT              (dra_action_dedent_get_type())
#define DRA_ACTION_DEDENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_dedent_get_type(), DraActionDedent))
#define DRA_ACTION_DEDENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_ACTION_DEDENT, DraActionDedentClass))
#define DRA_IS_ACTION_DEDENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_ACTION_DEDENT))
#define DRA_IS_ACTION_DEDENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_ACTION_DEDENT))
#define DRA_ACTION_DEDENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_ACTION_DEDENT, DraActionDedentClass))


typedef struct _DraActionDedent               DraActionDedent;
typedef struct _DraActionDedentPrivate        DraActionDedentPrivate;
typedef struct _DraActionDedentClass          DraActionDedentClass;


struct _DraActionDedent {
	LeaAction parent;
};

struct _DraActionDedentClass {
	LeaActionClass parent_class;
};


GType dra_action_dedent_get_type();

DraActionDedent *dra_action_dedent_new();

void dra_action_dedent_set_editor_panel(DraActionDedent *action, DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* ACTION_EDIT_DRAACTIONDEDENT_H_ */
