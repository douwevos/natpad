/*
   File:    jagmooseservice.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 29, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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


#ifndef JAGMOOSESERVICE_H_
#define JAGMOOSESERVICE_H_

#include "../indexer/context/jagindexerjremap.h"
#include "../indexer/jar/jagindexerjarmap.h"
#include <caterpillar.h>
#include <worm.h>
#include <moose.h>

G_BEGIN_DECLS

#define JAG_TYPE_MOOSE_SERVICE              (jag_moose_service_get_type())
#define JAG_MOOSE_SERVICE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_moose_service_get_type(), JagMooseService))
#define JAG_MOOSE_SERVICE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_MOOSE_SERVICE, JagMooseServiceClass))
#define JAG_IS_MOOSE_SERVICE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_MOOSE_SERVICE))
#define JAG_IS_MOOSE_SERVICE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_MOOSE_SERVICE))
#define JAG_MOOSE_SERVICE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_MOOSE_SERVICE, JagMooseServiceClass))
#define JAG_MOOSE_SERVICE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_MOOSE_SERVICE, JagMooseServicePrivate))


typedef struct _JagMooseService               JagMooseService;
typedef struct _JagMooseServicePrivate        JagMooseServicePrivate;
typedef struct _JagMooseServiceClass          JagMooseServiceClass;


struct _JagMooseService {
	GObject parent;
	JagMooseServicePrivate *priv;
};

struct _JagMooseServiceClass {
	GObjectClass parent_class;
};


GType jag_moose_service_get_type();

JagMooseService *jag_moose_service_new(MooService *moo_service, JagIndexerJreMap *jre_map, JagIndexerJarMap *jar_map);

WorService *jag_moose_service_get_worm_service(JagMooseService *moose_service);
MooService *jag_moose_service_get_moo_service(JagMooseService *moose_service);
JagIndexerJreMap *jag_moose_service_get_jre_map(JagMooseService *moose_service);
JagIndexerJarMap *jag_moose_service_get_jar_map(JagMooseService *moose_service);

G_END_DECLS

#endif /* JAGMOOSESERVICE_H_ */
