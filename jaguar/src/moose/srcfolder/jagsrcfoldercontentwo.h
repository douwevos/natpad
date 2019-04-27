/*
   File:    jagsrcfoldercontentwo.h
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

#ifndef JAGSRCFOLDERCONTENTWO_H_
#define JAGSRCFOLDERCONTENTWO_H_

#include <caterpillar.h>
#include <moose.h>

G_BEGIN_DECLS

#define JAG_TYPE_SRC_FOLDER_CONTENT_WO              (jag_src_folder_content_wo_get_type())
#define JAG_SRC_FOLDER_CONTENT_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_src_folder_content_wo_get_type(), JagSrcFolderContentWo))
#define JAG_SRC_FOLDER_CONTENT_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_SRC_FOLDER_CONTENT_WO, JagSrcFolderContentWoClass))
#define JAG_IS_SRC_FOLDER_CONTENT_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_SRC_FOLDER_CONTENT_WO))
#define JAG_IS_SRC_FOLDER_CONTENT_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_SRC_FOLDER_CONTENT_WO))
#define JAG_SRC_FOLDER_CONTENT_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_SRC_FOLDER_CONTENT_WO, JagSrcFolderContentWoClass))

typedef struct _JagSrcFolderContentWo               JagSrcFolderContentWo;
typedef struct _JagSrcFolderContentWoPrivate        JagSrcFolderContentWoPrivate;
typedef struct _JagSrcFolderContentWoClass          JagSrcFolderContentWoClass;

struct _JagSrcFolderContentWo {
	GObject parent;
};

struct _JagSrcFolderContentWoClass {
	GObjectClass parent_class;
};

GType jag_src_folder_content_wo_get_type();

JagSrcFolderContentWo *jag_src_folder_content_wo_new();

CatStringWo *jag_src_folder_content_wo_key();

JagSrcFolderContentWo *jag_src_folder_content_wo_ensure_editable(JagSrcFolderContentWo *content, MooNodeWo *moo_src_folder_node);
gboolean jag_src_folder_content_wo_is_fixed(JagSrcFolderContentWo *content);
int jag_src_folder_content_wo_get_version(JagSrcFolderContentWo *content);
JagSrcFolderContentWo *jag_src_folder_content_wo_anchor(JagSrcFolderContentWo *content, int version);

void jag_src_folder_content_wo_mark(JagSrcFolderContentWo *e_content);

G_END_DECLS

#endif /* JAGSRCFOLDERCONTENTWO_H_ */
