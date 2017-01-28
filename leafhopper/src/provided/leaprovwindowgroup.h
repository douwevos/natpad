/*
   File:    leaprovwindowgroup.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Dec 7, 2013
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

#ifndef LEAPROVWINDOWGROUP_H_
#define LEAPROVWINDOWGROUP_H_

#include "../action/leaactiongroup.h"
#include "leaprovshowviewgroup.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_PROV_WINDOW_GROUP              (lea_prov_window_group_get_type())
#define LEA_PROV_WINDOW_GROUP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_prov_window_group_get_type(), LeaProvWindowGroup))
#define LEA_PROV_WINDOW_GROUP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_PROV_WINDOW_GROUP, LeaProvWindowGroupClass))
#define LEA_IS_PROV_WINDOW_GROUP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_PROV_WINDOW_GROUP))
#define LEA_IS_PROV_WINDOW_GROUP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_PROV_WINDOW_GROUP))
#define LEA_PROV_WINDOW_GROUP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_PROV_WINDOW_GROUP, LeaProvWindowGroupClass))
#define LEA_PROV_WINDOW_GROUP_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), LEA_TYPE_PROV_WINDOW_GROUP, LeaProvWindowGroupPrivate))


typedef struct _LeaProvWindowGroup               LeaProvWindowGroup;
typedef struct _LeaProvWindowGroupPrivate        LeaProvWindowGroupPrivate;
typedef struct _LeaProvWindowGroupClass          LeaProvWindowGroupClass;


struct _LeaProvWindowGroup {
	LeaActionGroup parent;
	LeaProvWindowGroupPrivate *priv;
};

struct _LeaProvWindowGroupClass {
	LeaActionGroupClass parent_class;
};


GType lea_prov_window_group_get_type();

LeaProvWindowGroup *lea_prov_window_group_new();

G_END_DECLS

#endif /* LEAPROVWINDOWGROUP_H_ */
