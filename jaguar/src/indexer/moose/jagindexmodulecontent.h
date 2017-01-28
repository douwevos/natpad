/*
   File:    jagindexmodulecontent.h
   Project: jaguar
   Author:  Douwe Vos
   Date:    May 7, 2014
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

#ifndef JAGINDEXMODULECONTENT_H_
#define JAGINDEXMODULECONTENT_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_INDEX_MODULE_CONTENT_WO              (jag_index_module_content_get_type())
#define JAG_INDEX_MODULE_CONTENT_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_index_module_content_get_type(), JagIndexModuleContent))
#define JAG_INDEX_MODULE_CONTENT_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_INDEX_MODULE_CONTENT_WO, JagIndexModuleContentClass))
#define JAG_IS_INDEX_MODULE_CONTENT_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_INDEX_MODULE_CONTENT_WO))
#define JAG_IS_INDEX_MODULE_CONTENT_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_INDEX_MODULE_CONTENT_WO))
#define JAG_INDEX_MODULE_CONTENT_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_INDEX_MODULE_CONTENT_WO, JagIndexModuleContentClass))
#define JAG_INDEX_MODULE_CONTENT_WO_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_INDEX_MODULE_CONTENT_WO, JagIndexModuleContentPrivate))


typedef struct _JagIndexModuleContent               JagIndexModuleContent;
typedef struct _JagIndexModuleContentPrivate        JagIndexModuleContentPrivate;
typedef struct _JagIndexModuleContentClass          JagIndexModuleContentClass;


struct _JagIndexModuleContent {
	GObject parent;
	JagIndexModuleContentPrivate *priv;
};

struct _JagIndexModuleContentClass {
	GObjectClass parent_class;
};


GType jag_index_module_content_get_type();

JagIndexModuleContent *jag_index_module_content_new();

gboolean jag_index_module_content_need_to_update(JagIndexModuleContent *index_module, int jag_module_content_version);


CatStringWo *jag_index_module_content_key();

G_END_DECLS

#endif /* JAGINDEXMODULECONTENT_H_ */
