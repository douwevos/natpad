/*
   File:    mooactionproperties.h
   Project: moose-explorer
   Author:  Douwe Vos
   Date:    Mar 22, 2014
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

#ifndef MOOACTIONPROPERTIES_H_
#define MOOACTIONPROPERTIES_H_

#include <leafhopper.h>
#include <caterpillar.h>
#include <moose.h>

G_BEGIN_DECLS

#define MOO_TYPE_ACTION_PROPERTIES              (moo_action_properties_get_type())
#define MOO_ACTION_PROPERTIES(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_action_properties_get_type(), MooActionProperties))
#define MOO_ACTION_PROPERTIES_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_ACTION_PROPERTIES, MooActionPropertiesClass))
#define MOO_IS_ACTION_PROPERTIES(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_ACTION_PROPERTIES))
#define MOO_IS_ACTION_PROPERTIES_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_ACTION_PROPERTIES))
#define MOO_ACTION_PROPERTIES_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_ACTION_PROPERTIES, MooActionPropertiesClass))

typedef struct _MooActionProperties               MooActionProperties;
typedef struct _MooActionPropertiesPrivate        MooActionPropertiesPrivate;
typedef struct _MooActionPropertiesClass          MooActionPropertiesClass;

struct _MooActionProperties {
	LeaAction parent;
};

struct _MooActionPropertiesClass {
	LeaActionClass parent_class;
};

GType moo_action_properties_get_type();

MooActionProperties *moo_action_properties_new(MooService *moo_service);

void moo_action_properties_set_selection(MooActionProperties *action_properties, CatArrayWo *selection);

G_END_DECLS

#endif /* MOOACTIONPROPERTIES_H_ */
