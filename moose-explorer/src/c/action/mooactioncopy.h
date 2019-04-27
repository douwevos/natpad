/*
   File:    mooactioncopy.h
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Dec 25, 2013
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

#ifndef MOOACTIONCOPY_H_
#define MOOACTIONCOPY_H_

#include <caterpillar.h>
#include <leafhopper.h>
#include "../clipboard/mooclipboard.h"

G_BEGIN_DECLS

#define MOO_TYPE_ACTION_COPY              (moo_action_copy_get_type())
#define MOO_ACTION_COPY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_action_copy_get_type(), MooActionCopy))
#define MOO_ACTION_COPY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_ACTION_COPY, MooActionCopyClass))
#define MOO_IS_ACTION_COPY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_ACTION_COPY))
#define MOO_IS_ACTION_COPY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_ACTION_COPY))
#define MOO_ACTION_COPY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_ACTION_COPY, MooActionCopyClass))

typedef struct _MooActionCopy               MooActionCopy;
typedef struct _MooActionCopyPrivate        MooActionCopyPrivate;
typedef struct _MooActionCopyClass          MooActionCopyClass;

struct _MooActionCopy {
	LeaAction parent;
};

struct _MooActionCopyClass {
	LeaActionClass parent_class;
};

GType moo_action_copy_get_type();

MooActionCopy *moo_action_copy_new(MooClipboard *moo_clipboard);

void moo_action_copy_set_has_copyable_selection(MooActionCopy *action_copy, gboolean has_copyable_selection, CatArrayWo *selection);

G_END_DECLS

#endif /* MOOACTIONCOPY_H_ */
