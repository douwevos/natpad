/*
   File:    draconnectorkey.h
   Project: dragonfly
   Author:  Douwe Vos
   Date:    Dec 26, 2016
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

#ifndef DOCUMENT_DRACONNECTORKEY_H_
#define DOCUMENT_DRACONNECTORKEY_H_

#include <caterpillar.h>
#include <chameleon.h>

G_BEGIN_DECLS

#define DRA_TYPE_CONNECTOR_KEY              (dra_connector_key_get_type())
#define DRA_CONNECTOR_KEY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_connector_key_get_type(), DraConnectorKey))
#define DRA_CONNECTOR_KEY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_CONNECTOR_KEY, DraConnectorKeyClass))
#define DRA_IS_CONNECTOR_KEY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_CONNECTOR_KEY))
#define DRA_IS_CONNECTOR_KEY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_CONNECTOR_KEY))
#define DRA_CONNECTOR_KEY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_CONNECTOR_KEY, DraConnectorKeyClass))


typedef struct _DraConnectorKey               DraConnectorKey;
typedef struct _DraConnectorKeyPrivate        DraConnectorKeyPrivate;
typedef struct _DraConnectorKeyClass          DraConnectorKeyClass;


struct _DraConnectorKey {
	GObject parent;
};

struct _DraConnectorKeyClass {
	GObjectClass parent_class;
};


GType dra_connector_key_get_type();

DraConnectorKey *dra_connector_key_new(ChaDocument *document, CatStringWo *a_slot_key);


int dra_connector_key_hash(const DraConnectorKey *key);

gboolean dra_connector_key_equal(const DraConnectorKey *key_a, const DraConnectorKey *key_b);


G_END_DECLS

#endif /* DOCUMENT_DRACONNECTORKEY_H_ */
