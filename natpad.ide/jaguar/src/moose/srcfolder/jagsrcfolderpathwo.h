/*
   File:    jagsrcfolderpathwo.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    May 31, 2014
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

#ifndef JAGSRCFOLDERPATHWO_H_
#define JAGSRCFOLDERPATHWO_H_

#include <caterpillar.h>
#include <moose.h>

G_BEGIN_DECLS

#define JAG_TYPE_SRC_FOLDER_PATH_WO              (jag_src_folder_path_wo_get_type())
#define JAG_SRC_FOLDER_PATH_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_src_folder_path_wo_get_type(), JagSrcFolderPathWo))
#define JAG_SRC_FOLDER_PATH_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_SRC_FOLDER_PATH_WO, JagSrcFolderPathWoClass))
#define JAG_IS_SRC_FOLDER_PATH_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_SRC_FOLDER_PATH_WO))
#define JAG_IS_SRC_FOLDER_PATH_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_SRC_FOLDER_PATH_WO))
#define JAG_SRC_FOLDER_PATH_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_SRC_FOLDER_PATH_WO, JagSrcFolderPathWoClass))
#define JAG_SRC_FOLDER_PATH_WO_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_SRC_FOLDER_PATH_WO, JagSrcFolderPathWoPrivate))


typedef struct _JagSrcFolderPathWo               JagSrcFolderPathWo;
typedef struct _JagSrcFolderPathWoPrivate        JagSrcFolderPathWoPrivate;
typedef struct _JagSrcFolderPathWoClass          JagSrcFolderPathWoClass;


struct _JagSrcFolderPathWo {
	GObject parent;
	JagSrcFolderPathWoPrivate *priv;
};

struct _JagSrcFolderPathWoClass {
	GObjectClass parent_class;
};


GType jag_src_folder_path_wo_get_type();

JagSrcFolderPathWo *jag_src_folder_path_wo_new(MooNamePath *name_path);

JagSrcFolderPathWo *jag_src_folder_path_wo_ensure_editable(JagSrcFolderPathWo *node);
JagSrcFolderPathWo *jag_src_folder_path_wo_get_original(JagSrcFolderPathWo *node);
gboolean jag_src_folder_path_wo_is_fixed(JagSrcFolderPathWo *node);
JagSrcFolderPathWo *jag_src_folder_path_wo_anchor(JagSrcFolderPathWo *node, int version);


MooNamePath *jag_src_folder_path_wo_get_name_path(JagSrcFolderPathWo *path);

gboolean jag_src_folder_path_wo_update_resource_node_version(JagSrcFolderPathWo *e_path, int new_res_version);
gboolean jag_src_folder_path_wo_test_resource_node_version(JagSrcFolderPathWo *e_path, int new_res_version);


int jag_src_folder_path_wo_get_jaguar_node_offset(JagSrcFolderPathWo *path);
void jag_src_folder_path_wo_set_jaguar_node_offset(JagSrcFolderPathWo *e_path, int new_offset);


G_END_DECLS

#endif /* JAGSRCFOLDERPATHWO_H_ */
