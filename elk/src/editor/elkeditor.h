/*
   File:    elkeditor.h
   Project: elk
   Author:  Douwe Vos
   Date:    Jun 25, 2019
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

#ifndef EDITOR_ELKEDITOR_H_
#define EDITOR_ELKEDITOR_H_

#include <caterpillar.h>
#include <dragonfly.h>
#include <worm.h>

G_BEGIN_DECLS

#define ELK_TYPE_EDITOR              (elk_editor_get_type())
#define ELK_EDITOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), elk_editor_get_type(), ElkEditor))
#define ELK_EDITOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), ELK_TYPE_EDITOR, ElkEditorClass))
#define ELK_IS_EDITOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_EDITOR))
#define ELK_IS_EDITOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), ELK_TYPE_EDITOR))
#define ELK_EDITOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ELK_TYPE_EDITOR, ElkEditorClass))

struct _ElkEditor {
	DraEditor parent;
};

struct _ElkEditorClass {
	DraEditorClass parent_class;
};

typedef struct _ElkEditor               ElkEditor;
typedef struct _ElkEditorPrivate        ElkEditorPrivate;
typedef struct _ElkEditorClass          ElkEditorClass;


GType elk_editor_get_type();

ElkEditor *elk_editor_new(ChaDocument *document, DraConnectorMap *connector_map, DraIConnectorRequestFactory *connector_factory, WorService *wor_service);

G_END_DECLS

#endif /* EDITOR_ELKEDITOR_H_ */
