/*
   File:    mooactionrefresh.h
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Mar 14, 2014
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

#ifndef MOOACTIONREFRESH_H_
#define MOOACTIONREFRESH_H_

#include <caterpillar.h>
#include <leafhopper.h>
#include <moose.h>

G_BEGIN_DECLS

#define MOO_TYPE_ACTION_REFRESH              (moo_action_refresh_get_type())
#define MOO_ACTION_REFRESH(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_action_refresh_get_type(), MooActionRefresh))
#define MOO_ACTION_REFRESH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_ACTION_REFRESH, MooActionRefreshClass))
#define MOO_IS_ACTION_REFRESH(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_ACTION_REFRESH))
#define MOO_IS_ACTION_REFRESH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_ACTION_REFRESH))
#define MOO_ACTION_REFRESH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_ACTION_REFRESH, MooActionRefreshClass))
#define MOO_ACTION_REFRESH_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_ACTION_REFRESH, MooActionRefreshPrivate))


typedef struct _MooActionRefresh               MooActionRefresh;
typedef struct _MooActionRefreshPrivate        MooActionRefreshPrivate;
typedef struct _MooActionRefreshClass          MooActionRefreshClass;


struct _MooActionRefresh {
	LeaAction parent;
	MooActionRefreshPrivate *priv;
};

struct _MooActionRefreshClass {
	LeaActionClass parent_class;
};


GType moo_action_refresh_get_type();

MooActionRefresh *moo_action_refresh_new(MooService *moo_service);

void moo_action_refresh_set_selection(MooActionRefresh *action_refresh, CatArrayWo *selection);

G_END_DECLS

#endif /* MOOACTIONREFRESH_H_ */
