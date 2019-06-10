/*
   File:    dradocumentconnector.h
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

#ifndef DOCUMENT_DRADOCUMENTCONNECTOR_H_
#define DOCUMENT_DRADOCUMENTCONNECTOR_H_

#include "draiconnectorrequestfactory.h"
#include "../spell/draspellhelper.h"
#include <caterpillar.h>
#include <chameleon.h>
#include <worm.h>

G_BEGIN_DECLS

#define DRA_TYPE_DOCUMENT_CONNECTOR              (dra_document_connector_get_type())
#define DRA_DOCUMENT_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), dra_document_connector_get_type(), DraDocumentConnector))
#define DRA_DOCUMENT_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DRA_TYPE_DOCUMENT_CONNECTOR, DraDocumentConnectorClass))
#define DRA_IS_DOCUMENT_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), DRA_TYPE_DOCUMENT_CONNECTOR))
#define DRA_IS_DOCUMENT_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DRA_TYPE_DOCUMENT_CONNECTOR))
#define DRA_DOCUMENT_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DRA_TYPE_DOCUMENT_CONNECTOR, DraDocumentConnectorClass))


typedef struct _DraDocumentConnector               DraDocumentConnector;
typedef struct _DraDocumentConnectorPrivate        DraDocumentConnectorPrivate;
typedef struct _DraDocumentConnectorClass          DraDocumentConnectorClass;


struct _DraDocumentConnector {
	GObject parent;
};

struct _DraDocumentConnectorClass {
	GObjectClass parent_class;
};


GType dra_document_connector_get_type();

DraDocumentConnector *dra_document_connector_new(WorService *wor_service, DraSpellHelper *spell_helper, ChaDocument *document, CatStringWo *a_slot_key, DraIConnectorRequestFactory *factory);

void dra_document_connector_set_spell_helper(DraDocumentConnector *connector, DraSpellHelper *new_spell_helper);

void dra_document_connector_increase_usage(DraDocumentConnector *connector);
gboolean dra_document_connector_decrease_usage(DraDocumentConnector *connector);

void dra_document_connector_post_augment_request(DraDocumentConnector *connector);

G_END_DECLS

#endif /* DOCUMENT_DRADOCUMENTCONNECTOR_H_ */
