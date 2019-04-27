/*
   File:    leaactiongroupref.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Jan 2, 2014
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

#ifndef LEAACTIONGROUPREF_H_
#define LEAACTIONGROUPREF_H_

#include "leaactiongroup.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_ACTION_GROUP_REF              (lea_action_group_ref_get_type())
#define LEA_ACTION_GROUP_REF(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_action_group_ref_get_type(), LeaActionGroupRef))
#define LEA_ACTION_GROUP_REF_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_ACTION_GROUP_REF, LeaActionGroupRefClass))
#define LEA_IS_ACTION_GROUP_REF(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_ACTION_GROUP_REF))
#define LEA_IS_ACTION_GROUP_REF_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_ACTION_GROUP_REF))
#define LEA_ACTION_GROUP_REF_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_ACTION_GROUP_REF, LeaActionGroupRefClass))

typedef struct _LeaActionGroupRef               LeaActionGroupRef;
typedef struct _LeaActionGroupRefPrivate        LeaActionGroupRefPrivate;
typedef struct _LeaActionGroupRefClass          LeaActionGroupRefClass;


struct _LeaActionGroupRef {
	LeaActionGroup parent;
};

struct _LeaActionGroupRefClass {
	LeaActionGroupClass parent_class;
};


GType lea_action_group_ref_get_type();

LeaActionGroupRef *lea_action_group_ref_new(LeaActionGroup *action_group);

void lea_action_group_ref_merge(LeaActionGroupRef *action_group, LeaActionGroup *merge_group);

void lea_action_group_ref_unmerge(LeaActionGroupRef *action_group, LeaActionGroup *merge_group);

G_END_DECLS

#endif /* LEAACTIONGROUPREF_H_ */
