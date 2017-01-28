/*
   File:    dragroupnavigate.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Oct 12, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#ifndef ACTION_DRAGROUPNAVIGATE_H_
#define ACTION_DRAGROUPNAVIGATE_H_

#include <leafhopper.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define DRA_TYPE_GROUP_NAVIGATE              (dra_group_navigate_get_type())
#define DRA_GROUP_NAVIGATE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_group_navigate_get_type(), DraGroupNavigate))
#define DRA_GROUP_NAVIGATE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_GROUP_NAVIGATE, DraGroupNavigateClass))
#define DRA_IS_GROUP_NAVIGATE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_GROUP_NAVIGATE))
#define DRA_IS_GROUP_NAVIGATE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_GROUP_NAVIGATE))
#define DRA_GROUP_NAVIGATE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_GROUP_NAVIGATE, DraGroupNavigateClass))


typedef struct _DraGroupNavigate               DraGroupNavigate;
typedef struct _DraGroupNavigatePrivate        DraGroupNavigatePrivate;
typedef struct _DraGroupNavigateClass          DraGroupNavigateClass;


struct _DraGroupNavigate {
	LeaActionGroup parent;
};

struct _DraGroupNavigateClass {
	LeaActionGroupClass parent_class;
};


GType dra_group_navigate_get_type();

DraGroupNavigate *dra_group_navigate_new();

G_END_DECLS

#endif /* ACTION_DRAGROUPNAVIGATE_H_ */
