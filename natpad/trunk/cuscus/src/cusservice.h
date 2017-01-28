/*
   File:    cusservice.h
   Project: cuscus
   Author:  Douwe Vos
   Date:    May 6, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef CUSSERVICE_H_
#define CUSSERVICE_H_

#include <caterpillar.h>
#include <elk.h>
#include <worm.h>

G_BEGIN_DECLS

#define CUS_TYPE_SERVICE              (cus_service_get_type())
#define CUS_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cus_service_get_type(), CusService))
#define CUS_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CUS_TYPE_SERVICE, CusServiceClass))
#define CUS_IS_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CUS_TYPE_SERVICE))
#define CUS_IS_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CUS_TYPE_SERVICE))
#define CUS_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CUS_TYPE_SERVICE, CusServiceClass))


typedef struct _CusService               CusService;
typedef struct _CusServicePrivate        CusServicePrivate;
typedef struct _CusServiceClass          CusServiceClass;


struct _CusService {
	GObject parent;
};

struct _CusServiceClass {
	GObjectClass parent_class;
};


GType cus_service_get_type();

CusService *cus_service_new(WorService *wor_service, ElkService *elk_service);

G_END_DECLS

#endif /* CUSSERVICE_H_ */
