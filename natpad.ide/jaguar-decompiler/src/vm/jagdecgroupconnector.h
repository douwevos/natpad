/*
   File:    jagdecgroupconnector.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 4, 2012
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


#ifndef JAGDECGROUPCONNECTOR_H_
#define JAGDECGROUPCONNECTOR_H_

#include "../type/jagdecimportrepository.h"
#include "intermediate/jagdecintermediategroup.h"
#include "intermediate/jagdecintermediateruncontext.h"
#include <jaguarbytecode.h>
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_GROUP_CONNECTOR              (jag_dec_group_connector_get_type())
#define JAG_DEC_GROUP_CONNECTOR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_group_connector_get_type(), JagDecGroupConnector))
#define JAG_DEC_GROUP_CONNECTOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_GROUP_CONNECTOR, JagDecGroupConnectorClass))
#define JAG_DEC_IS_GROUP_CONNECTOR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_GROUP_CONNECTOR))
#define JAG_DEC_IS_GROUP_CONNECTOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_GROUP_CONNECTOR))
#define JAG_DEC_GROUP_CONNECTOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_GROUP_CONNECTOR, JagDecGroupConnectorClass))
#define JAG_DEC_GROUP_CONNECTOR_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_GROUP_CONNECTOR, JagDecGroupConnectorPrivate))

typedef struct _JagDecGroupConnector               JagDecGroupConnector;
typedef struct _JagDecGroupConnectorPrivate        JagDecGroupConnectorPrivate;
typedef struct _JagDecGroupConnectorClass          JagDecGroupConnectorClass;


struct _JagDecGroupConnector {
	GObject parent;
	JagDecGroupConnectorPrivate *priv;
};

struct _JagDecGroupConnectorClass {
	GObjectClass parent_class;
};


GType jag_dec_group_connector_get_type();

JagDecGroupConnector *jag_dec_group_connector_new(JagDecImportRepository *import_repository, JagBytClassfile *classfile, JagBytMethod *method, JagBytMnemonicBlock *mnemonic_block, JagBytTryCatchList *try_catch_list);

JagDecIntermediateRunContext *jag_dec_group_connector_get_run_context(JagDecGroupConnector *group_connector);

JagDecIntermediateGroup *jag_dec_group_connector_get_model_container(JagDecGroupConnector *group_connector);

G_END_DECLS


#endif /* JAGDECGROUPCONNECTOR_H_ */
