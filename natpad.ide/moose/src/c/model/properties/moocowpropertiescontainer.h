/*
   File:    moocowpropertiescontainer.h
   Project: moose
   Author:  Douwe Vos
   Date:    Aug 30, 2014
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

#ifndef MOOCOWPROPERTIESCONTAINER_H_
#define MOOCOWPROPERTIESCONTAINER_H_

#include "../moonodewo.h"
#include "mooipropertieshandler.h"
#include <cow.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_COW_PROPERTIES_CONTAINER              (moo_cow_properties_container_get_type())
#define MOO_COW_PROPERTIES_CONTAINER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_cow_properties_container_get_type(), MooCowPropertiesContainer))
#define MOO_COW_PROPERTIES_CONTAINER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_COW_PROPERTIES_CONTAINER, MooCowPropertiesContainerClass))
#define MOO_IS_COW_PROPERTIES_CONTAINER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_COW_PROPERTIES_CONTAINER))
#define MOO_IS_COW_PROPERTIES_CONTAINER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_COW_PROPERTIES_CONTAINER))
#define MOO_COW_PROPERTIES_CONTAINER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_COW_PROPERTIES_CONTAINER, MooCowPropertiesContainerClass))


typedef struct _MooCowPropertiesContainer               MooCowPropertiesContainer;
typedef struct _MooCowPropertiesContainerPrivate        MooCowPropertiesContainerPrivate;
typedef struct _MooCowPropertiesContainerClass          MooCowPropertiesContainerClass;

struct _MooService;

struct _MooCowPropertiesContainer {
	CowContainer parent;
};

struct _MooCowPropertiesContainerClass {
	CowContainerClass parent_class;
};


GType moo_cow_properties_container_get_type();

MooCowPropertiesContainer *moo_cow_properties_container_new(struct _MooService *moo_service);

void moo_cow_properties_container_set_node(MooCowPropertiesContainer *container, MooNodeWo *node);

MooNodeWo *moo_cow_properties_container_get_node(MooCowPropertiesContainer *container);


void moo_cow_properties_container_add_handler(MooCowPropertiesContainer *container, MooIPropertiesHandler *handler);

void moo_cow_properties_container_commit(MooCowPropertiesContainer *container);


G_END_DECLS

#endif /* MOOCOWPROPERTIESCONTAINER_H_ */
