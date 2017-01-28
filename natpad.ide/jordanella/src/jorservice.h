/*
   File:    jorservice.h
   Project: jordanella
   Author:  Douwe Vos
   Date:    Feb 5, 2016
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

#ifndef JORSERVICE_H_
#define JORSERVICE_H_

#include <caterpillar.h>
#include <elk.h>
#include <worm.h>

G_BEGIN_DECLS

#define JOR_TYPE_SERVICE              (jor_service_get_type())
#define JOR_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jor_service_get_type(), JorService))
#define JOR_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JOR_TYPE_SERVICE, JorServiceClass))
#define JOR_IS_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JOR_TYPE_SERVICE))
#define JOR_IS_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JOR_TYPE_SERVICE))
#define JOR_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JOR_TYPE_SERVICE, JorServiceClass))


typedef struct _JorService               JorService;
typedef struct _JorServicePrivate        JorServicePrivate;
typedef struct _JorServiceClass          JorServiceClass;


struct _JorService {
	GObject parent;
};

struct _JorServiceClass {
	GObjectClass parent_class;
};


GType jor_service_get_type();

JorService *jor_service_new(WorService *wor_service, ElkService *elk_service);

G_END_DECLS

#endif /* JORSERVICE_H_ */
