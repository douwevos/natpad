/*
   File:    xanservice.h
   Project: xantus
   Author:  Douwe Vos
   Date:    Feb 9, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#ifndef XANSERVICE_H_
#define XANSERVICE_H_

#include <caterpillar.h>
#include <elk.h>
#include <worm.h>

G_BEGIN_DECLS

#define XAN_TYPE_SERVICE              (xan_service_get_type())
#define XAN_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), xan_service_get_type(), XanService))
#define XAN_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), XAN_TYPE_SERVICE, XanServiceClass))
#define XAN_IS_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XAN_TYPE_SERVICE))
#define XAN_IS_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), XAN_TYPE_SERVICE))
#define XAN_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), XAN_TYPE_SERVICE, XanServiceClass))


typedef struct _XanService               XanService;
typedef struct _XanServicePrivate        XanServicePrivate;
typedef struct _XanServiceClass          XanServiceClass;


struct _XanService {
	GObject parent;
};

struct _XanServiceClass {
	GObjectClass parent_class;
};


GType xan_service_get_type();

XanService *xan_service_new();

G_END_DECLS

#endif /* XANSERVICE_H_ */
