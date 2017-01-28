/*
   File:    elkactionrevert.h
   Project: elk
   Author:  Douwe Vos
   Date:    Jan 12, 2013
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#ifndef ELKACTIONREVERT_H_
#define ELKACTIONREVERT_H_

#include <dragonfly.h>
#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define ELK_TYPE_ACTION_REVERT              (elk_action_revert_get_type())
#define ELK_ACTION_REVERT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_action_revert_get_type(), ElkActionRevert))
#define ELK_ACTION_REVERT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_ACTION_REVERT, ElkActionRevertClass))
#define ELK_IS_ACTION_REVERT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_ACTION_REVERT))
#define ELK_IS_ACTION_REVERT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_ACTION_REVERT))
#define ELK_ACTION_REVERT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_ACTION_REVERT, ElkActionRevertClass))
#define ELK_ACTION_REVERT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), ELK_TYPE_ACTION_REVERT, ElkActionRevertPrivate))

typedef struct _ElkActionRevert               ElkActionRevert;
typedef struct _ElkActionRevertPrivate        ElkActionRevertPrivate;
typedef struct _ElkActionRevertClass          ElkActionRevertClass;


struct _ElkActionRevert {
	LeaAction parent;
	ElkActionRevertPrivate *priv;
};

struct _ElkActionRevertClass {
	LeaActionClass parent_class;
};


GType elk_action_revert_get_type();

ElkActionRevert *elk_action_revert_new();

void elk_action_revert_set_editor_panel(ElkActionRevert *action, DraEditorPanel *editor_panel);

G_END_DECLS

#endif /* ELKACTIONREVERT_H_ */
