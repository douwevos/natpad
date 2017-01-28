/*
   File:    elkiresourceeditorfactory.h
   Project: elk
   Author:  Douwe Vos
   Date:    Nov 24, 2013
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

#ifndef ELKIRESOURCEEDITORFACTORY_H_
#define ELKIRESOURCEEDITORFACTORY_H_

#include <caterpillar.h>
#include <moose.h>

#define ELK_TYPE_IRESOURCE_EDITOR_FACTORY                 (elk_iresource_editor_factory_get_type())
#define ELK_IRESOURCE_EDITOR_FACTORY(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), ELK_TYPE_IRESOURCE_EDITOR_FACTORY, ElkIResourceEditorFactory))
#define ELK_IS_IRESOURCE_EDITOR_FACTORY(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), ELK_TYPE_IRESOURCE_EDITOR_FACTORY))
#define ELK_IRESOURCE_EDITOR_FACTORY_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), ELK_TYPE_IRESOURCE_EDITOR_FACTORY, ElkIResourceEditorFactoryInterface))


typedef struct _ElkIResourceEditorFactory               ElkIResourceEditorFactory; /* dummy object */
typedef struct _ElkIResourceEditorFactoryInterface      ElkIResourceEditorFactoryInterface;


struct _MooNodeWo;
struct _ElkIResourceEditorFactoryMatcher;

struct _ElkIResourceEditorFactoryInterface {
	GTypeInterface parent_iface;

	GtkWidget *(*createEditor)(ElkIResourceEditorFactory *self, MooNodeWo *moo_node);
	GtkWidget *(*createOutline)(ElkIResourceEditorFactory *self, MooNodeWo *moo_node, GtkWidget *editor);
};

GType elk_iresource_editor_factory_get_type(void);

GtkWidget *elk_iresource_editor_factory_create_editor(ElkIResourceEditorFactory *self, MooNodeWo *moo_node);

#endif /* ELKIRESOURCEEDITORFACTORY_H_ */
