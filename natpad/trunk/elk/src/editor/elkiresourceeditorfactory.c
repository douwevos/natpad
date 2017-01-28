/*
   File:    elkiresourceeditorfactory.c
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

#include "elkiresourceeditorfactory.h"

G_DEFINE_INTERFACE(ElkIResourceEditorFactory, elk_iresource_editor_factory, G_TYPE_OBJECT);

static void elk_iresource_editor_factory_default_init (ElkIResourceEditorFactoryInterface *klass) {
	klass->createOutline = NULL;
}

GtkWidget *elk_iresource_editor_factory_create_editor(ElkIResourceEditorFactory *self, MooNodeWo *moo_node) {
	g_return_val_if_fail(ELK_IS_IRESOURCE_EDITOR_FACTORY(self), NULL);
	return ELK_IRESOURCE_EDITOR_FACTORY_GET_INTERFACE(self)->createEditor(self, moo_node);
}
