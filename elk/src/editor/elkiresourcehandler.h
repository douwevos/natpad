/*
   File:    elkiresourcehandler.h
   Project: elk
   Author:  Douwe Vos
   Date:    Nov 25, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#ifndef ELKIRESOURCEHANDLER_H_
#define ELKIRESOURCEHANDLER_H_

#include <caterpillar.h>
#include <viper.h>
#include <moose.h>
#include <leafhopper.h>

G_BEGIN_DECLS

#define ELK_TYPE_IRESOURCE_HANDLER                 (elk_iresource_handler_get_type ())
#define ELK_IRESOURCE_HANDLER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), ELK_TYPE_IRESOURCE_HANDLER, ElkIResourceHandler))
#define ELK_IS_IRESOURCE_HANDLER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ELK_TYPE_IRESOURCE_HANDLER))
#define ELK_IRESOURCE_HANDLER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), ELK_TYPE_IRESOURCE_HANDLER, ElkIResourceHandlerInterface))


typedef struct _ElkIResourceHandler               ElkIResourceHandler;
typedef struct _ElkIResourceHandlerInterface      ElkIResourceHandlerInterface;

struct _ElkIResourceHandlerInterface {
  GTypeInterface parent_iface;

  void (*matchEditorFactories)(ElkIResourceHandler *self, CatArrayWo *e_enlist_to, MooNodeWo *node);
  void (*emptyEditorFactories)(ElkIResourceHandler *self, CatHashMapWo *e_enlist_to);
};

GType elk_iresource_handler_get_type (void);

G_END_DECLS

#endif /* ELKIRESOURCEHANDLER_H_ */
