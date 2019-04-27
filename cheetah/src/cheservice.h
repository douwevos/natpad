/*
   File:    cheservice.h
   Project: cheetah
   Author:  Douwe Vos
   Date:    Feb 8, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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

#ifndef CHESERVICE_H_
#define CHESERVICE_H_

#include <caterpillar.h>
#include <worm.h>
#include <elk.h>

G_BEGIN_DECLS

#define CHE_TYPE_SERVICE              (che_service_get_type())
#define CHE_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), che_service_get_type(), CheService))
#define CHE_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CHE_TYPE_SERVICE, CheServiceClass))
#define CHE_IS_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CHE_TYPE_SERVICE))
#define CHE_IS_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CHE_TYPE_SERVICE))
#define CHE_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CHE_TYPE_SERVICE, CheServiceClass))

typedef struct _CheService               CheService;
typedef struct _CheServicePrivate        CheServicePrivate;
typedef struct _CheServiceClass          CheServiceClass;

struct _CheService {
	GObject parent;
};

struct _CheServiceClass {
	GObjectClass parent_class;
};

GType che_service_get_type();

CheService *che_service_new(WorService *wor_service, ElkService *elk_service);

G_END_DECLS

#endif /* CHESERVICE_H_ */
