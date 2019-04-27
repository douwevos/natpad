/*
   File:    jagsrcfilecontentwo.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Nov 23, 2013
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

#ifndef JAGSRCFILECONTENTWO_H_
#define JAGSRCFILECONTENTWO_H_

#include <caterpillar.h>
#include <moose.h>
#include "../../indexer/moose/jagindexsourcecontent.h"

G_BEGIN_DECLS

#define JAG_TYPE_SRCFILE_CONTENT_WO              (jag_srcfile_content_wo_get_type())
#define JAG_SRCFILE_CONTENT_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_srcfile_content_wo_get_type(), JagSrcfileContentWo))
#define JAG_SRCFILE_CONTENT_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_SRCFILE_CONTENT_WO, JagSrcfileContentWoClass))
#define JAG_IS_SRCFILE_CONTENT_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_SRCFILE_CONTENT_WO))
#define JAG_IS_SRCFILE_CONTENT_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_SRCFILE_CONTENT_WO))
#define JAG_SRCFILE_CONTENT_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_SRCFILE_CONTENT_WO, JagSrcfileContentWoClass))

typedef struct _JagSrcfileContentWo               JagSrcfileContentWo;
typedef struct _JagSrcfileContentWoPrivate        JagSrcfileContentWoPrivate;
typedef struct _JagSrcfileContentWoClass          JagSrcfileContentWoClass;

struct _JagSrcfileContentWo {
	GObject parent;
};

struct _JagSrcfileContentWoClass {
	GObjectClass parent_class;
};

GType jag_srcfile_content_wo_get_type();

JagSrcfileContentWo *jag_srcfile_content_wo_new(MooIdPath *classfilePath, VipIFile *linked_file);

JagSrcfileContentWo *jag_srcfile_content_wo_ensure_editable(JagSrcfileContentWo *content);
gboolean jag_srcfile_content_wo_is_fixed(JagSrcfileContentWo *content);
int jag_srcfile_content_wo_get_version(JagSrcfileContentWo *content);
JagSrcfileContentWo *jag_srcfile_content_wo_anchor(JagSrcfileContentWo *content, int version);

void jag_srcfile_content_wo_mark(JagSrcfileContentWo *e_content);


MooIdPath *jag_srcfile_content_wo_get_file_path(JagSrcfileContentWo *content);

VipIFile *jag_srcfile_content_wo_get_file(JagSrcfileContentWo *content);
void jag_srcfile_content_wo_set_file(JagSrcfileContentWo *e_content, VipIFile *file);

JagIndexSourceContent *jag_srcfile_content_wo_get_index_context(JagSrcfileContentWo *content);

CatStringWo *jag_srcfile_content_wo_key();

G_END_DECLS

#endif /* JAGSRCFILECONTENTWO_H_ */
