/*
   File:    groedservice.h
   Project: gross-editor
   Author:  Douwe Vos
   Date:    Oct 12, 2016
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

#ifndef GROEDSERVICE_H_
#define GROEDSERVICE_H_

#include <caterpillar.h>
#include <elk.h>
#include <worm.h>

G_BEGIN_DECLS

#define GROED_TYPE_SERVICE              (groed_service_get_type())
#define GROED_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), groed_service_get_type(), GroEdService))
#define GROED_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GROED_TYPE_SERVICE, GroEdServiceClass))
#define GROED_IS_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GROED_TYPE_SERVICE))
#define GROED_IS_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GROED_TYPE_SERVICE))
#define GROED_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GROED_TYPE_SERVICE, GroEdServiceClass))


typedef struct _GroEdService               GroEdService;
typedef struct _GroEdServicePrivate        GroEdServicePrivate;
typedef struct _GroEdServiceClass          GroEdServiceClass;


struct _GroEdService {
	GObject parent;
};

struct _GroEdServiceClass {
	GObjectClass parent_class;
};


GType groed_service_get_type();

GroEdService *groed_service_new(WorService *wor_service, ElkService *elk_service);

G_END_DECLS

#endif /* GROEDSERVICE_H_ */
