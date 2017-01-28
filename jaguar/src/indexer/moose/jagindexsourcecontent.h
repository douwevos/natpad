/*
   File:    jagindexsourcecontent.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    Jun 5, 2014
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

#ifndef JAGINDEXSOURCECONTENT_H_
#define JAGINDEXSOURCECONTENT_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_INDEX_SOURCE_CONTENT              (jag_index_source_content_get_type())
#define JAG_INDEX_SOURCE_CONTENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_index_source_content_get_type(), JagIndexSourceContent))
#define JAG_INDEX_SOURCE_CONTENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_INDEX_SOURCE_CONTENT, JagIndexSourceContentClass))
#define JAG_IS_INDEX_SOURCE_CONTENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_INDEX_SOURCE_CONTENT))
#define JAG_IS_INDEX_SOURCE_CONTENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_INDEX_SOURCE_CONTENT))
#define JAG_INDEX_SOURCE_CONTENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_INDEX_SOURCE_CONTENT, JagIndexSourceContentClass))
#define JAG_INDEX_SOURCE_CONTENT_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_INDEX_SOURCE_CONTENT, JagIndexSourceContentPrivate))


typedef struct _JagIndexSourceContent               JagIndexSourceContent;
typedef struct _JagIndexSourceContentPrivate        JagIndexSourceContentPrivate;
typedef struct _JagIndexSourceContentClass          JagIndexSourceContentClass;


struct _JagIndexSourceContent {
	GObject parent;
	JagIndexSourceContentPrivate *priv;
};

struct _JagIndexSourceContentClass {
	GObjectClass parent_class;
};


GType jag_index_source_content_get_type();

JagIndexSourceContent *jag_index_source_content_new();

gboolean jag_index_source_content_need_to_update(JagIndexSourceContent *index_source, int jag_srcfile_content_version);


G_END_DECLS

#endif /* JAGINDEXSOURCECONTENT_H_ */
