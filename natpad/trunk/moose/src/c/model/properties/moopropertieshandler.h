/*
   File:    moopropertieshandler.h
   Project: moose
   Author:  Douwe Vos
   Date:    Mar 29, 2014
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

#ifndef MOOPROPERTIESHANDLER_H_
#define MOOPROPERTIESHANDLER_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_PROPERTIES_HANDLER              (moo_properties_handler_get_type())
#define MOO_PROPERTIES_HANDLER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_properties_handler_get_type(), MooPropertiesHandler))
#define MOO_PROPERTIES_HANDLER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_PROPERTIES_HANDLER, MooPropertiesHandlerClass))
#define MOO_IS_PROPERTIES_HANDLER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_PROPERTIES_HANDLER))
#define MOO_IS_PROPERTIES_HANDLER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_PROPERTIES_HANDLER))
#define MOO_PROPERTIES_HANDLER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_PROPERTIES_HANDLER, MooPropertiesHandlerClass))
#define MOO_PROPERTIES_HANDLER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_PROPERTIES_HANDLER, MooPropertiesHandlerPrivate))


typedef struct _MooPropertiesHandler               MooPropertiesHandler;
typedef struct _MooPropertiesHandlerPrivate        MooPropertiesHandlerPrivate;
typedef struct _MooPropertiesHandlerClass          MooPropertiesHandlerClass;

struct _MooService;

struct _MooPropertiesHandler {
	GObject parent;
	MooPropertiesHandlerPrivate *priv;
};

struct _MooPropertiesHandlerClass {
	GObjectClass parent_class;
};


GType moo_properties_handler_get_type();

MooPropertiesHandler *moo_properties_handler_new(struct _MooService *moo_service);

G_END_DECLS

#endif /* MOOPROPERTIESHANDLER_H_ */
