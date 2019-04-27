/*
   File:    mooactionaddproject.h
   Project: moose
   Author:  Douwe Vos
   Date:    Nov 24, 2013
   Web:     http://www.natpad.net/
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

#ifndef MOOACTIONADDPROJECT_H_
#define MOOACTIONADDPROJECT_H_

#include <caterpillar.h>
#include <leafhopper.h>
#include <moose.h>

G_BEGIN_DECLS

#define MOO_TYPE_ACTION_ADD_PROJECT              (moo_action_add_project_get_type())
#define MOO_ACTION_ADD_PROJECT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_action_add_project_get_type(), MooActionAddProject))
#define MOO_ACTION_ADD_PROJECT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_ACTION_ADD_PROJECT, MooActionAddProjectClass))
#define MOO_IS_ACTION_ADD_PROJECT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_ACTION_ADD_PROJECT))
#define MOO_IS_ACTION_ADD_PROJECT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_ACTION_ADD_PROJECT))
#define MOO_ACTION_ADD_PROJECT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_ACTION_ADD_PROJECT, MooActionAddProjectClass))

typedef struct _MooActionAddProject               MooActionAddProject;
typedef struct _MooActionAddProjectPrivate        MooActionAddProjectPrivate;
typedef struct _MooActionAddProjectClass          MooActionAddProjectClass;

extern CatS moo_s_action_add_project;

struct _MooActionAddProject {
	LeaAction parent;
};

struct _MooActionAddProjectClass {
	LeaActionClass parent_class;
};

GType moo_action_add_project_get_type();

MooActionAddProject *moo_action_add_project_new(MooService *moo_service);

G_END_DECLS

#endif /* MOOACTIONADDPROJECT_H_ */
