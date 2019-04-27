/*
   File:    moogroupmain.h
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Dec 23, 2013
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

#ifndef MOOGROUPMAIN_H_
#define MOOGROUPMAIN_H_

#include "../clipboard/mooclipboard.h"
#include <caterpillar.h>
#include <leafhopper.h>
#include <viper.h>
#include <moose.h>

G_BEGIN_DECLS

#define MOO_TYPE_GROUP_MAIN              (moo_group_main_get_type())
#define MOO_GROUP_MAIN(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_group_main_get_type(), MooGroupMain))
#define MOO_GROUP_MAIN_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_GROUP_MAIN, MooGroupMainClass))
#define MOO_IS_GROUP_MAIN(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_GROUP_MAIN))
#define MOO_IS_GROUP_MAIN_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_GROUP_MAIN))
#define MOO_GROUP_MAIN_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_GROUP_MAIN, MooGroupMainClass))

typedef struct _MooGroupMain               MooGroupMain;
typedef struct _MooGroupMainPrivate        MooGroupMainPrivate;
typedef struct _MooGroupMainClass          MooGroupMainClass;

struct _MooGroupMain {
	LeaActionGroup parent;
};

struct _MooGroupMainClass {
	LeaActionGroupClass parent_class;
};

GType moo_group_main_get_type();

MooGroupMain *moo_group_main_new(MooService *moo_service, LeaFrame *frame, MooClipboard *clipboard, LeaKeyContext *key_context);

gboolean moo_group_main_set_has_focus(MooGroupMain *group_main, gboolean owner_has_focus);

void moo_group_main_set_selection(MooGroupMain *group_main, VipSnapshot *vip_snapshot, CatArrayWo *selection);

G_END_DECLS

#endif /* MOOGROUPMAIN_H_ */
