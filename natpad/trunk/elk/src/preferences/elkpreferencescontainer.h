/*
   File:    elkpreferencescontainer.h
   Project: elk
   Author:  Douwe Vos
   Date:    Sep 23, 2014
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

#ifndef ELKPREFERENCESCONTAINER_H_
#define ELKPREFERENCESCONTAINER_H_

#include "elkpreferenceswo.h"
#include <caterpillar.h>
#include <cow.h>
#include <shoveler.h>

G_BEGIN_DECLS

#define ELK_TYPE_PREFERENCES_CONTAINER              (elk_preferences_container_get_type())
#define ELK_PREFERENCES_CONTAINER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_preferences_container_get_type(), ElkPreferencesContainer))
#define ELK_PREFERENCES_CONTAINER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_PREFERENCES_CONTAINER, ElkPreferencesContainerClass))
#define ELK_IS_PREFERENCES_CONTAINER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_PREFERENCES_CONTAINER))
#define ELK_IS_PREFERENCES_CONTAINER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_PREFERENCES_CONTAINER))
#define ELK_PREFERENCES_CONTAINER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_PREFERENCES_CONTAINER, ElkPreferencesContainerClass))


typedef struct _ElkPreferencesContainer               ElkPreferencesContainer;
typedef struct _ElkPreferencesContainerPrivate        ElkPreferencesContainerPrivate;
typedef struct _ElkPreferencesContainerClass          ElkPreferencesContainerClass;


struct _ElkPreferencesContainer {
	CowContainer parent;
};

struct _ElkPreferencesContainerClass {
	CowContainerClass parent_class;
};


GType elk_preferences_container_get_type();

ElkPreferencesContainer *elk_preferences_container_new();

gboolean elk_preferences_container_set(ElkPreferencesContainer *container, ElkPreferencesWo *new_prefs);
ElkPreferencesWo *elk_preferences_container_get(ElkPreferencesContainer *container);

ElkPreferencesWo *elk_preferences_container_get_fixed(ElkPreferencesContainer *container);
ElkPreferencesWo *elk_preferences_container_get_editable(ElkPreferencesContainer *container);

ShoModel *elk_preferences_container_get_model(ElkPreferencesContainer *container);
void elk_preferences_container_set_model(ElkPreferencesContainer *container, ShoModel *model);


G_END_DECLS

#endif /* ELKPREFERENCESCONTAINER_H_ */
