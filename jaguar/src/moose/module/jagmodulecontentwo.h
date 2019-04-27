/*
   File:    jagmodulecontentwo.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 6, 2013
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

#ifndef JAGMODULECONTENTWO_H_
#define JAGMODULECONTENTWO_H_

#include "jagmodulesettingswo.h"
#include "../srcfolder/jagsrcfolderpathwo.h"
#include "../library/jaglibrarycontentwo.h"
#include "../../indexer/moose/jagindexmodulecontent.h"
#include "../jagmooseservice.h"
#include "../../indexer/context/jagjrelink.h"
#include <moose.h>
#include <caterpillar.h>
#include <shoveler.h>

G_BEGIN_DECLS

#define JAG_TYPE_MODULE_CONTENT_WO              (jag_module_content_wo_get_type())
#define JAG_MODULE_CONTENT_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_module_content_wo_get_type(), JagModuleContentWo))
#define JAG_MODULE_CONTENT_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_MODULE_CONTENT_WO, JagModuleContentWoClass))
#define JAG_IS_MODULE_CONTENT_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_MODULE_CONTENT_WO))
#define JAG_IS_MODULE_CONTENT_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_MODULE_CONTENT_WO))
#define JAG_MODULE_CONTENT_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_MODULE_CONTENT_WO, JagModuleContentWoClass))

typedef struct _JagModuleContentWo               JagModuleContentWo;
typedef struct _JagModuleContentWoPrivate        JagModuleContentWoPrivate;
typedef struct _JagModuleContentWoClass          JagModuleContentWoClass;


struct _JagModuleContentWo {
	GObject parent;
};

struct _JagModuleContentWoClass {
	GObjectClass parent_class;
	MooResourceNameMatcher *res_name_matcher;
};


GType jag_module_content_wo_get_type();

JagModuleContentWo *jag_module_content_wo_new(JagMooseService *jag_moose_service, MooIdPath *module_id_path);
JagModuleContentWo *jag_module_content_wo_ensure_editable(JagModuleContentWo *module_content);
gboolean jag_module_content_wo_is_fixed(JagModuleContentWo *module_content);
int jag_module_content_wo_get_version(JagModuleContentWo *module_content);

void jag_module_content_wo_mark(JagModuleContentWo *e_module_content);

gboolean jag_module_content_wo_test_resource_content_version(JagModuleContentWo *module_content, int next_version);
void jag_module_content_wo_set_resource_content_version(JagModuleContentWo *module_content, int next_version);

void jag_module_content_wo_reconfigure(JagModuleContentWo *e_module_content, ShoModel *model);

JagModuleSettingsWo *jag_module_content_wo_get_module_settings(JagModuleContentWo *module_content);

gboolean jag_module_content_wo_set_module_settings(JagModuleContentWo *e_module_content, JagModuleSettingsWo *new_settings);

void jag_module_content_write_settings_2_model(JagModuleContentWo *module_content, ShoModel *model);

CatStringWo *jag_module_content_wo_key();


gboolean jag_module_content_wo_set_source_folders(JagModuleContentWo *e_module_content, CatArrayWo *a_source_folders);
CatArrayWo *jag_module_content_wo_get_source_folders(JagModuleContentWo *module_content);

int jag_module_content_wo_source_path_count(JagModuleContentWo *module_content);
JagSrcFolderPathWo *jag_module_content_wo_source_path_at(JagModuleContentWo *module_content, int index);

JagSrcFolderPathWo *jag_module_content_wo_editable_source_path_at(JagModuleContentWo *e_module_content, int index);


gboolean jag_module_content_wo_set_libraries(JagModuleContentWo *e_module_content, CatArrayWo *a_source_folders);
CatArrayWo *jag_module_content_wo_get_libraries(JagModuleContentWo *module_content);

int jag_module_content_wo_library_content_count(JagModuleContentWo *module_content);
JagLibraryContentWo *jag_module_content_wo_editable_library_content_at(JagModuleContentWo *e_module_content, int index);

JagIndexModuleContent *jag_module_content_wo_get_index_context(JagModuleContentWo *module_content);

gboolean jag_module_content_wo_set_jre_link(JagModuleContentWo *e_module_content, JagJreLink *jre_link);
JagJreLink *jag_module_content_wo_get_jre_link(JagModuleContentWo *module_content);

G_END_DECLS

#endif /* JAGMODULECONTENTWO_H_ */
