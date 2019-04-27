/*
   File:    mooactionrename.h
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Apr 3, 2014
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

#ifndef MOOACTIONRENAME_H_
#define MOOACTIONRENAME_H_

#include <leafhopper.h>
#include <caterpillar.h>
#include <moose.h>

G_BEGIN_DECLS

#define MOO_TYPE_ACTION_RENAME              (moo_action_rename_get_type())
#define MOO_ACTION_RENAME(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_action_rename_get_type(), MooActionRename))
#define MOO_ACTION_RENAME_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_ACTION_RENAME, MooActionRenameClass))
#define MOO_IS_ACTION_RENAME(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_ACTION_RENAME))
#define MOO_IS_ACTION_RENAME_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_ACTION_RENAME))
#define MOO_ACTION_RENAME_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_ACTION_RENAME, MooActionRenameClass))

typedef struct _MooActionRename               MooActionRename;
typedef struct _MooActionRenamePrivate        MooActionRenamePrivate;
typedef struct _MooActionRenameClass          MooActionRenameClass;

struct _MooActionRename {
	LeaAction parent;
};

struct _MooActionRenameClass {
	LeaActionClass parent_class;
};

GType moo_action_rename_get_type();

MooActionRename *moo_action_rename_new(MooService *moo_service, LeaFrame *frame);

void moo_action_rename_set_selection(MooActionRename *action_rename, CatArrayWo *selection);

G_END_DECLS

#endif /* MOOACTIONRENAME_H_ */
