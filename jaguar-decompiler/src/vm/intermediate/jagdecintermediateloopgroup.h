/*
   File:    jagdecintermediateloopgroup.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 17, 2012
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


#ifndef JAGDECINTERMEDIATELOOPGROUP_H_
#define JAGDECINTERMEDIATELOOPGROUP_H_


#include "jagdecintermediategroup.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_LOOP_GROUP              (jag_dec_intermediate_loop_group_get_type())
#define JAG_DEC_INTERMEDIATE_LOOP_GROUP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_loop_group_get_type(), JagDecIntermediateLoopGroup))
#define JAG_DEC_INTERMEDIATE_LOOP_GROUP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_LOOP_GROUP, JagDecIntermediateLoopGroupClass))
#define JAG_DEC_IS_INTERMEDIATE_LOOP_GROUP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_LOOP_GROUP))
#define JAG_DEC_IS_INTERMEDIATE_LOOP_GROUP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_LOOP_GROUP))
#define JAG_DEC_INTERMEDIATE_LOOP_GROUP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_LOOP_GROUP, JagDecIntermediateLoopGroupClass))
#define JAG_DEC_INTERMEDIATE_LOOP_GROUP_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_LOOP_GROUP, JagDecIntermediateLoopGroupPrivate))

typedef struct _JagDecIntermediateLoopGroup               JagDecIntermediateLoopGroup;
typedef struct _JagDecIntermediateLoopGroupPrivate        JagDecIntermediateLoopGroupPrivate;
typedef struct _JagDecIntermediateLoopGroupClass          JagDecIntermediateLoopGroupClass;


struct _JagDecIntermediateLoopGroup {
	JagDecIntermediateGroup parent;
	JagDecIntermediateLoopGroupPrivate *priv;
};

struct _JagDecIntermediateLoopGroupClass {
	JagDecIntermediateGroupClass parent_class;
};


GType jag_dec_intermediate_loop_group_get_type();

JagDecIntermediateLoopGroup *jag_dec_intermediate_loop_group_new();

JagDecIntermediateLoopGroup *jag_dec_intermediate_loop_group_new_with_nodes(CatArrayWo *e_node_list);

G_END_DECLS


#endif /* JAGDECINTERMEDIATELOOPGROUP_H_ */
