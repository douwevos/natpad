/*
   File:    worservice.h
   Project: worm
   Author:  Douwe Vos
   Date:    Dec 12, 2011
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


#ifndef WORSERVICE_H_
#define WORSERVICE_H_

#include <caterpillar.h>
#include "worrequest.h"

G_BEGIN_DECLS

#define WOR_TYPE_SERVICE              (wor_service_get_type())
#define WOR_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), wor_service_get_type(), WorService))
#define WOR_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), WOR_TYPE_SERVICE, WorServiceClass))
#define WOR_IS_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), WOR_TYPE_SERVICE))
#define WOR_IS_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), WOR_TYPE_SERVICE))
#define WOR_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), WOR_TYPE_SERVICE, WorServiceClass))
#define WOR_SERVICE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), WOR_TYPE_SERVICE, WorServicePrivate))


typedef struct _WorService               WorService;
typedef struct _WorServicePrivate        WorServicePrivate;
typedef struct _WorServiceClass          WorServiceClass;


struct _WorService {
		GObject parent;
		WorServicePrivate *priv;
};

struct _WorServiceClass {
		GObjectClass parent_class;
};


GType wor_service_get_type();

WorService *wor_service_new();

WorService *wor_service_new_ext(int nr_of_workers, int load);


void wor_service_post_request(WorService *service, WorRequest *request);
void wor_service_request_shutdown(WorService *service);

G_END_DECLS

#endif /* WORSERVICE_H_ */
