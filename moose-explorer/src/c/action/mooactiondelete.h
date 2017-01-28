/*
   File:    mooactiondelete.h
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Apr 18, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef MOOACTIONDELETE_H_
#define MOOACTIONDELETE_H_

#include <leafhopper.h>
#include <caterpillar.h>
#include <moose.h>

G_BEGIN_DECLS

#define MOO_TYPE_ACTION_DELETE              (moo_action_delete_get_type())
#define MOO_ACTION_DELETE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_action_delete_get_type(), MooActionDelete))
#define MOO_ACTION_DELETE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_ACTION_DELETE, MooActionDeleteClass))
#define MOO_IS_ACTION_DELETE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_ACTION_DELETE))
#define MOO_IS_ACTION_DELETE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_ACTION_DELETE))
#define MOO_ACTION_DELETE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_ACTION_DELETE, MooActionDeleteClass))
#define MOO_ACTION_DELETE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_ACTION_DELETE, MooActionDeletePrivate))


typedef struct _MooActionDelete               MooActionDelete;
typedef struct _MooActionDeletePrivate        MooActionDeletePrivate;
typedef struct _MooActionDeleteClass          MooActionDeleteClass;


struct _MooActionDelete {
	LeaAction parent;
	MooActionDeletePrivate *priv;
};

struct _MooActionDeleteClass {
	LeaActionClass parent_class;
};


GType moo_action_delete_get_type();

MooActionDelete *moo_action_delete_new(MooService *moo_service, LeaFrame *frame);

void moo_action_delete_set_selection(MooActionDelete *action_delete, CatArrayWo *selection);

G_END_DECLS

#endif /* MOOACTIONDELETE_H_ */
