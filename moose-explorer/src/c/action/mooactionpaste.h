/*
   File:    mooactionpaste.h
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Jan 11, 2014
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

#ifndef MOOACTIONPASTE_H_
#define MOOACTIONPASTE_H_

#include <caterpillar.h>
#include <leafhopper.h>
#include "../clipboard/mooclipboard.h"

G_BEGIN_DECLS

#define MOO_TYPE_ACTION_PASTE              (moo_action_paste_get_type())
#define MOO_ACTION_PASTE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_action_paste_get_type(), MooActionPaste))
#define MOO_ACTION_PASTE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_ACTION_PASTE, MooActionPasteClass))
#define MOO_IS_ACTION_PASTE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_ACTION_PASTE))
#define MOO_IS_ACTION_PASTE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_ACTION_PASTE))
#define MOO_ACTION_PASTE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_ACTION_PASTE, MooActionPasteClass))
#define MOO_ACTION_PASTE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_ACTION_PASTE, MooActionPastePrivate))


typedef struct _MooActionPaste               MooActionPaste;
typedef struct _MooActionPastePrivate        MooActionPastePrivate;
typedef struct _MooActionPasteClass          MooActionPasteClass;


struct _MooActionPaste {
	LeaAction parent;
	MooActionPastePrivate *priv;
};

struct _MooActionPasteClass {
	LeaActionClass parent_class;
};


GType moo_action_paste_get_type();

MooActionPaste *moo_action_paste_new(MooClipboard *moo_clipboard);

void moo_action_paste_set_selection(MooActionPaste *action_paste, gboolean has_resource, CatArrayWo *selection);

G_END_DECLS

#endif /* MOOACTIONPASTE_H_ */
