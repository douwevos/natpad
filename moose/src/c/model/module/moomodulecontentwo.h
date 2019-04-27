/*
   File:    moomodulecontentwo.h
   Project: moose
   Author:  Douwe Vos
   Date:    May 16, 2013
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

#ifndef MOOMODULECONTENT_H_
#define MOOMODULECONTENT_H_

#include "../../mooservice.h"
#include <caterpillar.h>
#include <shoveler.h>
#include <viper.h>

G_BEGIN_DECLS

#define MOO_TYPE_MODULE_CONTENT_WO              (moo_module_content_wo_get_type())
#define MOO_MODULE_CONTENT_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_module_content_wo_get_type(), MooModuleContentWo))
#define MOO_MODULE_CONTENT_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_MODULE_CONTENT_WO, MooModuleContentWoClass))
#define MOO_IS_MODULE_CONTENT_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_MODULE_CONTENT_WO))
#define MOO_IS_MODULE_CONTENT_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_MODULE_CONTENT_WO))
#define MOO_MODULE_CONTENT_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_MODULE_CONTENT_WO, MooModuleContentWoClass))

typedef struct _MooModuleContentWo               MooModuleContentWo;
typedef struct _MooModuleContentWoPrivate        MooModuleContentWoPrivate;
typedef struct _MooModuleContentWoClass          MooModuleContentWoClass;

struct _MooModuleContentWo {
	GObject parent;
};

struct _MooModuleContentWoClass {
	GObjectClass parent_class;
};


GType moo_module_content_wo_get_type();

MooModuleContentWo *moo_module_content_wo_new(CatReadableTreeNode *base_node);

MooModuleContentWo *moo_module_content_wo_ensure_editable(MooModuleContentWo *module_content);
MooModuleContentWo *moo_module_content_wo_anchor(MooModuleContentWo *module_content, int version);


gboolean moo_module_content_wo_should_refresh_viper_info(MooModuleContentWo *module_content);

void moo_module_content_wo_set_refresh_viper_info(MooModuleContentWo *e_module_content, gboolean refreshViperInfo);

CatReadableTreeNode *moo_module_content_wo_get_base_node(MooModuleContentWo *module_content);

void moo_module_content_wo_set_base_node(MooModuleContentWo *module_content, CatReadableTreeNode *base_node);

void moo_module_content_wo_set_settings_info(MooModuleContentWo *module_content, int settingsFileIndex, VipNode *settingsVipNode);


void moo_module_content_wo_transfer_settings(MooService *moo_service, MooModuleContentWo *e_module_content, MooModuleContentWo *transfer_from);

void moo_module_content_wo_settings_save(MooService *moo_service, MooModuleContentWo *module_content);

ShoModel *moo_module_content_wo_get_model(MooModuleContentWo *module_content);
ShoModel *moo_module_content_wo_get_writable_model(MooModuleContentWo *module_content);

CatStringWo *moo_module_content_wo_key();

G_END_DECLS

#endif /* MOOMODULECONTENT_H_ */
