/*
   File:    mulservice.h
   Project: mule
   Author:  Douwe Vos
   Date:    Mar 21, 2012
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

#ifndef MULSERVICE_H_
#define MULSERVICE_H_

#include <worm.h>
#include <elk.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define MUL_TYPE_SERVICE              (mul_service_get_type())
#define MUL_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), mul_service_get_type(), MulService))
#define MUL_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MUL_TYPE_SERVICE, MulServiceClass))
#define MUL_IS_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MUL_TYPE_SERVICE))
#define MUL_IS_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MUL_TYPE_SERVICE))
#define MUL_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MUL_TYPE_SERVICE, MulServiceClass))
#define MUL_SERVICE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MUL_TYPE_SERVICE, MulServicePrivate))

typedef struct _MulService               MulService;
typedef struct _MulServicePrivate        MulServicePrivate;
typedef struct _MulServiceClass          MulServiceClass;



struct _MulService {
	GObject parent;
	MulServicePrivate *priv;
};

struct _MulServiceClass {
	GObjectClass parent_class;
};


GType mul_service_get_type();

MulService *mul_service_new(WorService *wor_service, ElkService *elk_service);

G_END_DECLS

#endif /* MULSERVICE_H_ */
