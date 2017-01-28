/*
   File:    mooresourcecontentwo.h
   Project: moose
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


#ifndef MOORESOURCECONTENTWO_H_
#define MOORESOURCECONTENTWO_H_

#include <caterpillar.h>
#include <viper.h>

G_BEGIN_DECLS

#define MOO_TYPE_RESOURCE_CONTENT_WO              (moo_resource_content_wo_get_type())
#define MOO_RESOURCE_CONTENT_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_resource_content_wo_get_type(), MooResourceContentWo))
#define MOO_RESOURCE_CONTENT_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_RESOURCE_CONTENT_WO, MooResourceContentWoClass))
#define MOO_IS_RESOURCE_CONTENT_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_RESOURCE_CONTENT_WO))
#define MOO_IS_RESOURCE_CONTENT_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_RESOURCE_CONTENT_WO))
#define MOO_RESOURCE_CONTENT_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_RESOURCE_CONTENT_WO, MooResourceContentWoClass))
#define MOO_RESOURCE_CONTENT_WO_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_RESOURCE_CONTENT_WO, MooResourceContentWoPrivate))


typedef struct _MooResourceContentWo               MooResourceContentWo;
typedef struct _MooResourceContentWoPrivate        MooResourceContentWoPrivate;
typedef struct _MooResourceContentWoClass          MooResourceContentWoClass;


struct _MooResourceContentWo {
	GObject parent;
	MooResourceContentWoPrivate *priv;
};

struct _MooResourceContentWoClass {
	GObjectClass parent_class;
};


GType moo_resource_content_wo_get_type();

MooResourceContentWo *moo_resource_content_wo_new(CatReadableTreeNode *tree_node, MooResourceContentWo *parent_resource);

MooResourceContentWo *moo_resource_content_wo_ensure_editable(MooResourceContentWo *resource_content, MooResourceContentWo *parent_resource);
gboolean moo_resource_content_wo_is_fixed(MooResourceContentWo *resource_content);


int moo_resource_content_wo_get_version(MooResourceContentWo *resource_content);

VipNode *moo_resource_content_wo_get_viper_node(MooResourceContentWo *resource_content);

void moo_resource_content_wo_set_node(MooResourceContentWo *e_resource_content, CatReadableTreeNode *node);
CatReadableTreeNode *moo_resource_content_wo_get_node(MooResourceContentWo *resource_content);

CatStringWo *moo_resource_content_wo_key();

G_END_DECLS

#endif /* MOORESOURCECONTENTWO_H_ */
