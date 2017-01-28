/*
   File:    jagdecintermediategroup.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 5, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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


#ifndef JAGDECINTERMEDIATEGROUP_H_
#define JAGDECINTERMEDIATEGROUP_H_

#include "jagdeciintermediatenode.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_GROUP              (jag_dec_intermediate_group_get_type())
#define JAG_DEC_INTERMEDIATE_GROUP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_group_get_type(), JagDecIntermediateGroup))
#define JAG_DEC_INTERMEDIATE_GROUP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_GROUP, JagDecIntermediateGroupClass))
#define JAG_DEC_IS_INTERMEDIATE_GROUP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_GROUP))
#define JAG_DEC_IS_INTERMEDIATE_GROUP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_GROUP))
#define JAG_DEC_INTERMEDIATE_GROUP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_GROUP, JagDecIntermediateGroupClass))
#define JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_GROUP, JagDecIntermediateGroupPrivate))

typedef struct _JagDecIntermediateGroup               JagDecIntermediateGroup;
typedef struct _JagDecIntermediateGroupPrivate        JagDecIntermediateGroupPrivate;
typedef struct _JagDecIntermediateGroupClass          JagDecIntermediateGroupClass;


struct _JagDecIntermediateGroup {
	GObject parent;
	JagDecIntermediateGroupPrivate *priv;
};

struct _JagDecIntermediateGroupClass {
	GObjectClass parent_class;
};


GType jag_dec_intermediate_group_get_type();

void jag_dec_intermediate_group_construct(JagDecIntermediateGroup *intermediate_group);
void jag_dec_intermediate_group_construct_with_nodes(JagDecIntermediateGroup *intermediate_group, CatArrayWo *e_intermediate_nodes);

JagDecIntermediateGroup *jag_dec_intermediate_group_new();
JagDecIntermediateGroup *jag_dec_intermediate_group_new_with_nodes(CatArrayWo *e_intermediate_nodes);


gboolean jag_dec_intermediate_group_add(JagDecIntermediateGroup *intermediate_group, JagDecIIntermediateNode *node);
void jag_dec_intermediate_group_add_at(JagDecIntermediateGroup *intermediate_group, int index, JagDecIIntermediateNode *node);
void jag_dec_intermediate_group_replace(JagDecIntermediateGroup *intermediate_group, int index, JagDecIIntermediateNode *node);

int jag_dec_intermediate_group_count(JagDecIntermediateGroup *intermediate_group);
JagDecIIntermediateNode *jag_dec_intermediate_group_get(JagDecIntermediateGroup *intermediate_group, int index);
JagDecIIntermediateNode *jag_dec_intermediate_group_get_last(JagDecIntermediateGroup *intermediate_group);
CatArrayWo *jag_dec_intermediate_group_get_all(JagDecIntermediateGroup *intermediate_group);


void jag_dec_intermediate_group_remove(JagDecIntermediateGroup *intermediate_group, JagDecIIntermediateNode *node_to_remove);
JagDecIIntermediateNode *jag_dec_intermediate_group_remove_at(JagDecIntermediateGroup *intermediate_group, int index);

int jag_dec_intermediate_group_find_index(JagDecIntermediateGroup *intermediate_group, JagDecIIntermediateNode *node);
JagDecIIntermediateNode *jag_dec_intermediate_group_find_node(JagDecIntermediateGroup *intermediate_group, int index);

void jag_dec_intermediate_group_dump(JagDecIntermediateGroup *intermediate_group, CatStringWo *a_prefix);


G_END_DECLS


#endif /* JAGDECINTERMEDIATEGROUP_H_ */
