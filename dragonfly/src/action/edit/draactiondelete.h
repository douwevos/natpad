/*
   File:    draactiondelete.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Aug 30, 2015
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

#ifndef ACTION_EDIT_DRAACTIONDELETE_H_
#define ACTION_EDIT_DRAACTIONDELETE_H_

#include "../../draeditorpanel.h"
#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_ACTION_DELETE              (dra_action_delete_get_type())
#define LEA_ACTION_DELETE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_action_delete_get_type(), DraActionDelete))
#define LEA_ACTION_DELETE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_ACTION_DELETE, DraActionDeleteClass))
#define LEA_IS_ACTION_DELETE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_ACTION_DELETE))
#define LEA_IS_ACTION_DELETE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_ACTION_DELETE))
#define LEA_ACTION_DELETE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_ACTION_DELETE, DraActionDeleteClass))


typedef struct _DraActionDelete               DraActionDelete;
typedef struct _DraActionDeletePrivate        DraActionDeletePrivate;
typedef struct _DraActionDeleteClass          DraActionDeleteClass;


struct _DraActionDelete {
	LeaAction parent;
};

struct _DraActionDeleteClass {
	LeaActionClass parent_class;
};


GType dra_action_delete_get_type();

DraActionDelete *dra_action_delete_new(LeaKeyContext *key_context);

void dra_action_delete_set_editor_panel(DraActionDelete *action, DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* ACTION_EDIT_DRAACTIONDELETE_H_ */
