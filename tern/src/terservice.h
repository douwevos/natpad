/*
   File:    terservice.h
   Project: tern
   Author:  Douwe Vos
   Date:    Mar 14, 2012
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

#ifndef TERSERVICE_H_
#define TERSERVICE_H_

#include <cow.h>
#include <worm.h>
#include <elk.h>
#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define TER_TYPE_SERVICE              (ter_service_get_type())
#define TER_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), ter_service_get_type(), TerService))
#define TER_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), TER_TYPE_SERVICE, TerServiceClass))
#define TER_IS_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), TER_TYPE_SERVICE))
#define TER_IS_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), TER_TYPE_SERVICE))
#define TER_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), TER_TYPE_SERVICE, TerServiceClass))

typedef struct _TerService               TerService;
typedef struct _TerServicePrivate        TerServicePrivate;
typedef struct _TerServiceClass          TerServiceClass;

struct _TerService {
	GObject parent;
};

struct _TerServiceClass {
	GObjectClass parent_class;
};

GType ter_service_get_type();

TerService *ter_service_new(WorService *wor_service, ElkService *elk_service, VipService *vip_service);

G_END_DECLS

#endif /* TERSERVICE_H_ */
