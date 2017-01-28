/*
   File:    cowcontainer.h
   Project: cow
   Author:  Douwe Vos
   Date:    Aug 26, 2014
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

#ifndef COWCONTAINER_H_
#define COWCONTAINER_H_

#include "cowichangelistener.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define COW_TYPE_CONTAINER              (cow_container_get_type())
#define COW_CONTAINER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cow_container_get_type(), CowContainer))
#define COW_CONTAINER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), COW_TYPE_CONTAINER, CowContainerClass))
#define COW_IS_CONTAINER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), COW_TYPE_CONTAINER))
#define COW_IS_CONTAINER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), COW_TYPE_CONTAINER))
#define COW_CONTAINER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), COW_TYPE_CONTAINER, CowContainerClass))


typedef struct _CowContainer               CowContainer;
typedef struct _CowContainerPrivate        CowContainerPrivate;
typedef struct _CowContainerClass          CowContainerClass;


struct _CowContainer {
	GObject parent;
};

struct _CowContainerClass {
	GObjectClass parent_class;
	gboolean (*isSameConfig)(CowContainer *container, GObject *config_a, GObject *config_b);
	CowContainer *(*createEditor)(CowContainer *container);
	void (*setConfiguration)(CowContainer *container, gpointer new_config);
};


GType cow_container_get_type();

void cow_container_construct(CowContainer *container);
CowContainer *cow_container_new();
CowContainer *cow_container_create_editor(CowContainer *container);
void cow_container_copy_from(CowContainer *dest, CowContainer *source);

GObject *cow_container_get_configuration(CowContainer *container);
void cow_container_set_configuration(CowContainer *container, gpointer new_config);

void cow_container_dispatch_change_event(CowContainer *container);
void cow_container_add_change_listener(CowContainer *container, CowIChangeListener *listener);
void cow_container_remove_change_listener(CowContainer *container, CowIChangeListener *listener);

gboolean cow_container_is_same_config(CowContainer *container, GObject *config_a, GObject *config_b);

G_END_DECLS

#endif /* COWCONTAINER_H_ */
