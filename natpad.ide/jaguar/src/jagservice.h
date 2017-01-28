/*
   File:    jagservice.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 10, 2011
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2011 Douwe Vos.

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

#ifndef JAGSERVICE_H_
#define JAGSERVICE_H_

#include <caterpillar.h>
#include <worm.h>
#include <elk.h>
#include <moose.h>
//#include "module/jagijarmap.h"

G_BEGIN_DECLS

#define JAG_TYPE_SERVICE              (jag_service_get_type())
#define JAG_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_service_get_type(), JagService))
#define JAG_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_SERVICE, JagServiceClass))
#define JAG_IS_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_SERVICE))
#define JAG_IS_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_SERVICE))
#define JAG_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_SERVICE, JagServiceClass))
#define JAG_SERVICE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_SERVICE, JagServicePrivate))


typedef struct _JagService               JagService;
typedef struct _JagServicePrivate        JagServicePrivate;
typedef struct _JagServiceClass          JagServiceClass;


struct _JagService {
		GObject parent;
		JagServicePrivate *priv;
};

struct _JagServiceClass {
	GObjectClass parent_class;
};


GType jag_service_get_type();

JagService *jag_service_new(WorService *wor_service, ElkService *elk_service, MooService *moo_service);


//JagIJarMap *jag_service_get_jar_map(JagService *jag_service);

G_END_DECLS



#endif /* JAGSERVICE_H_ */
