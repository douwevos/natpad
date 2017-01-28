/*
   File:    jagsrcfolderupdater.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 20, 2013
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

#ifndef JAGSRCFOLDERUPDATER_H_
#define JAGSRCFOLDERUPDATER_H_

#include <caterpillar.h>
#include <viper.h>
#include <moose.h>

G_BEGIN_DECLS

#define JAG_TYPE_SRC_FOLDER_UPDATER              (jag_src_folder_updater_get_type())
#define JAG_SRC_FOLDER_UPDATER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_src_folder_updater_get_type(), JagSrcFolderUpdater))
#define JAG_SRC_FOLDER_UPDATER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_SRC_FOLDER_UPDATER, JagSrcFolderUpdaterClass))
#define JAG_IS_SRC_FOLDER_UPDATER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_SRC_FOLDER_UPDATER))
#define JAG_IS_SRC_FOLDER_UPDATER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_SRC_FOLDER_UPDATER))
#define JAG_SRC_FOLDER_UPDATER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_SRC_FOLDER_UPDATER, JagSrcFolderUpdaterClass))
#define JAG_SRC_FOLDER_UPDATER_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_SRC_FOLDER_UPDATER, JagSrcFolderUpdaterPrivate))


typedef struct _JagSrcFolderUpdater               JagSrcFolderUpdater;
typedef struct _JagSrcFolderUpdaterPrivate        JagSrcFolderUpdaterPrivate;
typedef struct _JagSrcFolderUpdaterClass          JagSrcFolderUpdaterClass;


struct _JagSrcFolderUpdater {
	GObject parent;
	JagSrcFolderUpdaterPrivate *priv;
};

struct _JagSrcFolderUpdaterClass {
	GObjectClass parent_class;
};


GType jag_src_folder_updater_get_type();

JagSrcFolderUpdater *jag_src_folder_updater_new(VipISequence *moo_id_sequence, MooNodeWo *editable_root, MooNodeWo *editable_src_folder,
		MooIdPath *src_folder_path);

void jag_src_folder_updater_run(JagSrcFolderUpdater *updater);


G_END_DECLS

#endif /* JAGSRCFOLDERUPDATER_H_ */
