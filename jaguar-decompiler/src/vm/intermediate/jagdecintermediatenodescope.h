/*
   File:    jagdecintermediatenodescope.h
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


#ifndef JAGDECINTERMEDIATENODESCOPE_H_
#define JAGDECINTERMEDIATENODESCOPE_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_DEC_TYPE_INTERMEDIATE_NODE_SCOPE              (jag_dec_intermediate_node_scope_get_type())
#define JAG_DEC_INTERMEDIATE_NODE_SCOPE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_intermediate_node_scope_get_type(), JagDecIntermediateNodeScope))
#define JAG_DEC_INTERMEDIATE_NODE_SCOPE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_INTERMEDIATE_NODE_SCOPE, JagDecIntermediateNodeScopeClass))
#define JAG_DEC_IS_INTERMEDIATE_NODE_SCOPE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_INTERMEDIATE_NODE_SCOPE))
#define JAG_DEC_IS_INTERMEDIATE_NODE_SCOPE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_INTERMEDIATE_NODE_SCOPE))
#define JAG_DEC_INTERMEDIATE_NODE_SCOPE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_INTERMEDIATE_NODE_SCOPE, JagDecIntermediateNodeScopeClass))
#define JAG_DEC_INTERMEDIATE_NODE_SCOPE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_INTERMEDIATE_NODE_SCOPE, JagDecIntermediateNodeScopePrivate))

typedef struct _JagDecIntermediateNodeScope               JagDecIntermediateNodeScope;
typedef struct _JagDecIntermediateNodeScopePrivate        JagDecIntermediateNodeScopePrivate;
typedef struct _JagDecIntermediateNodeScopeClass          JagDecIntermediateNodeScopeClass;


struct _JagDecIntermediateNodeScope {
	GObject parent;
	JagDecIntermediateNodeScopePrivate *priv;
};

struct _JagDecIntermediateNodeScopeClass {
	GObjectClass parent_class;
};


#define JAG_DEC_SCOPE_INDEX_NONE  -1
#define JAG_DEC_SCOPE_INDEX_EXIT  -2


GType jag_dec_intermediate_node_scope_get_type();

JagDecIntermediateNodeScope *jag_dec_intermediate_node_scope_new(int exit_index, int jump_index);

int jag_dec_intermediate_node_scope_get_exit_index(JagDecIntermediateNodeScope *node_scope);
void jag_dec_intermediate_node_scope_set_exit_index(JagDecIntermediateNodeScope *node_scope, int new_exit_index);

int jag_dec_intermediate_node_scope_get_jump_index(JagDecIntermediateNodeScope *node_scope);
void jag_dec_intermediate_node_scope_set_jump_index(JagDecIntermediateNodeScope *node_scope, int new_jump_index);

void jag_dec_intermediate_node_scope_unset(JagDecIntermediateNodeScope *node_scope);
void jag_dec_intermediate_node_scope_copy_from(JagDecIntermediateNodeScope *node_scope, JagDecIntermediateNodeScope *to_copy_from);

int jag_dec_intermediate_node_scope_get_out_index(JagDecIntermediateNodeScope *node_scope);

CatStringWo *jag_dec_intermediate_node_scope_as_text(JagDecIntermediateNodeScope *node_scope, CatStringWo *e_dump_buffer);

G_END_DECLS

#endif /* JAGDECINTERMEDIATENODESCOPE_H_ */
