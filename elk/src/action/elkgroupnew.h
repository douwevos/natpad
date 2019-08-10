/*
   File:    elkgroupnew.h
   Project: elk
   Author:  Douwe Vos
   Date:    9 Aug 2019
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2019 Douwe Vos.

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

#ifndef ACTION_ELKGROUPNEW_H_
#define ACTION_ELKGROUPNEW_H_

#include "../elkiservice.h"
#include <caterpillar.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define ELK_TYPE_GROUP_NEW              (elk_group_new_get_type())
#define ELK_GROUP_NEW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_group_new_get_type(), ElkGroupNew))
#define ELK_GROUP_NEW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_GROUP_NEW, ElkGroupNewClass))
#define ELK_IS_GROUP_NEW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_GROUP_NEW))
#define ELK_IS_GROUP_NEW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_GROUP_NEW))
#define ELK_GROUP_NEW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_GROUP_NEW, ElkGroupNewClass))

struct _ElkGroupNew {
	LeaActionGroup parent;
};

struct _ElkGroupNewClass {
	LeaActionGroupClass parent_class;
};

typedef struct _ElkGroupNew               ElkGroupNew;
typedef struct _ElkGroupNewPrivate        ElkGroupNewPrivate;
typedef struct _ElkGroupNewClass          ElkGroupNewClass;


GType elk_group_new_get_type();

ElkGroupNew *elk_group_new_new(ElkIService *service);

void elk_group_new_resource_handlers_updated(ElkGroupNew *group_new);

G_END_DECLS

#endif /* ACTION_ELKGROUPNEW_H_ */
