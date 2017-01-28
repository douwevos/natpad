/*
   File:    jagmodulepropertieshandler.h
   Project: jaguar
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

#ifndef JAGMODULEPROPERTIESHANDLER_H_
#define JAGMODULEPROPERTIESHANDLER_H_

#include <moose.h>
#include <caterpillar.h>
#include "../jagmooseservice.h"

G_BEGIN_DECLS

#define JAG_TYPE_MODULE_PROPERTIES_HANDLER              (jag_module_properties_handler_get_type())
#define JAG_MODULE_PROPERTIES_HANDLER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_module_properties_handler_get_type(), JagModulePropertiesHandler))
#define JAG_MODULE_PROPERTIES_HANDLER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_MODULE_PROPERTIES_HANDLER, JagModulePropertiesHandlerClass))
#define JAG_IS_MODULE_PROPERTIES_HANDLER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_MODULE_PROPERTIES_HANDLER))
#define JAG_IS_MODULE_PROPERTIES_HANDLER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_MODULE_PROPERTIES_HANDLER))
#define JAG_MODULE_PROPERTIES_HANDLER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_MODULE_PROPERTIES_HANDLER, JagModulePropertiesHandlerClass))
#define JAG_MODULE_PROPERTIES_HANDLER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_MODULE_PROPERTIES_HANDLER, JagModulePropertiesHandlerPrivate))


typedef struct _JagModulePropertiesHandler               JagModulePropertiesHandler;
typedef struct _JagModulePropertiesHandlerPrivate        JagModulePropertiesHandlerPrivate;
typedef struct _JagModulePropertiesHandlerClass          JagModulePropertiesHandlerClass;


struct _JagModulePropertiesHandler {
	GObject parent;
	JagModulePropertiesHandlerPrivate *priv;
};

struct _JagModulePropertiesHandlerClass {
	GObjectClass parent_class;
};


GType jag_module_properties_handler_get_type();

JagModulePropertiesHandler *jag_module_properties_handler_new(JagMooseService *jag_moose_service);

G_END_DECLS

#endif /* JAGMODULEPROPERTIESHANDLER_H_ */
