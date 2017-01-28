/*
   File:    jaglibrarycontentwo.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jul 20, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef JAGLIBRARYCONTENTWO_H_
#define JAGLIBRARYCONTENTWO_H_

#include "../../indexer/jar/jgijarentry.h"
#include "../jagmooseservice.h"
#include <caterpillar.h>


G_BEGIN_DECLS

#define JAG_TYPE_LIBRARY_CONTENT_WO              (jag_library_content_wo_get_type())
#define JAG_LIBRARY_CONTENT_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_library_content_wo_get_type(), JagLibraryContentWo))
#define JAG_LIBRARY_CONTENT_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_LIBRARY_CONTENT_WO, JagLibraryContentWoClass))
#define JAG_IS_LIBRARY_CONTENT_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_LIBRARY_CONTENT_WO))
#define JAG_IS_LIBRARY_CONTENT_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_LIBRARY_CONTENT_WO))
#define JAG_LIBRARY_CONTENT_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_LIBRARY_CONTENT_WO, JagLibraryContentWoClass))


typedef struct _JagLibraryContentWo               JagLibraryContentWo;
typedef struct _JagLibraryContentWoPrivate        JagLibraryContentWoPrivate;
typedef struct _JagLibraryContentWoClass          JagLibraryContentWoClass;


struct _JagLibraryContentWo {
	GObject parent;
};

struct _JagLibraryContentWoClass {
	GObjectClass parent_class;
};


GType jag_library_content_wo_get_type();

JagLibraryContentWo *jag_library_content_wo_new(JagMooseService *jag_moose_service, MooIdPath *module_id_path, JgiJarEntry *jar_entry);
JagLibraryContentWo *jag_library_content_wo_ensure_editable(JagLibraryContentWo *library_content);
gboolean jag_library_content_wo_is_fixed(JagLibraryContentWo *library_content);
int jag_library_content_wo_get_version(JagLibraryContentWo *library_content);

gboolean jag_library_content_wo_set_vip_node(JagLibraryContentWo *e_library_content, CatTreeNode *tree_node);
CatTreeNode *jag_library_content_wo_get_vip_node(JagLibraryContentWo *library_content);

CatStringWo *jag_library_content_wo_key();

gboolean jag_library_content_wo_set_new_jar_node_version(JagLibraryContentWo *library_content, MooNodeWo *real_jar_node);

void jag_library_content_wo_apply_jar_request(JagLibraryContentWo *library_content);

void jag_library_content_wo_set_node_id_path(JagLibraryContentWo *e_library_content, MooIdPath *node_id_path);


G_END_DECLS

#endif /* JAGLIBRARYCONTENTWO_H_ */
