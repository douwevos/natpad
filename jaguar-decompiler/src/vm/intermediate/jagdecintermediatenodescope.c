/*
   File:    jagdecintermediatenodescope.c
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

#include "jagdecintermediatenodescope.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateNodeScope"
#include <logging/catlog.h>

struct _JagDecIntermediateNodeScopePrivate {
	int exit_index;
	int jump_index;
};

G_DEFINE_TYPE (JagDecIntermediateNodeScope, jag_dec_intermediate_node_scope, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_node_scope_class_init(JagDecIntermediateNodeScopeClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateNodeScopePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_node_scope_init(JagDecIntermediateNodeScope *instance) {
	JagDecIntermediateNodeScopePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_NODE_SCOPE, JagDecIntermediateNodeScopePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagDecIntermediateNodeScope *instance = JAG_DEC_INTERMEDIATE_NODE_SCOPE(object);
//	JagDecIntermediateNodeScopePrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateNodeScope *jag_dec_intermediate_node_scope_new(int exit_index, int jump_index) {
	JagDecIntermediateNodeScope *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_NODE_SCOPE, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateNodeScopePrivate *priv = result->priv;
	priv->exit_index = exit_index;
	priv->jump_index = jump_index;
	return result;
}


int jag_dec_intermediate_node_scope_get_exit_index(JagDecIntermediateNodeScope *node_scope) {
	return JAG_DEC_INTERMEDIATE_NODE_SCOPE_GET_PRIVATE(node_scope)->exit_index;
}

int jag_dec_intermediate_node_scope_get_jump_index(JagDecIntermediateNodeScope *node_scope) {
	return JAG_DEC_INTERMEDIATE_NODE_SCOPE_GET_PRIVATE(node_scope)->jump_index;
}

void jag_dec_intermediate_node_scope_set_exit_index(JagDecIntermediateNodeScope *node_scope, int new_exit_index) {
	JAG_DEC_INTERMEDIATE_NODE_SCOPE_GET_PRIVATE(node_scope)->exit_index = new_exit_index;
}

void jag_dec_intermediate_node_scope_set_jump_index(JagDecIntermediateNodeScope *node_scope, int new_jump_index) {
	JAG_DEC_INTERMEDIATE_NODE_SCOPE_GET_PRIVATE(node_scope)->jump_index = new_jump_index;
}

void jag_dec_intermediate_node_scope_unset(JagDecIntermediateNodeScope *node_scope) {
	JagDecIntermediateNodeScopePrivate *priv = JAG_DEC_INTERMEDIATE_NODE_SCOPE_GET_PRIVATE(node_scope);
	priv->exit_index = JAG_DEC_SCOPE_INDEX_NONE;
	priv->jump_index = JAG_DEC_SCOPE_INDEX_NONE;
}

void jag_dec_intermediate_node_scope_copy_from(JagDecIntermediateNodeScope *node_scope, JagDecIntermediateNodeScope *to_copy_from) {
	JagDecIntermediateNodeScopePrivate *priv = JAG_DEC_INTERMEDIATE_NODE_SCOPE_GET_PRIVATE(node_scope);
	JagDecIntermediateNodeScopePrivate *srcpriv = JAG_DEC_INTERMEDIATE_NODE_SCOPE_GET_PRIVATE(to_copy_from);
	priv->exit_index = srcpriv->exit_index;
	priv->jump_index = srcpriv->jump_index;
}



int jag_dec_intermediate_node_scope_get_out_index(JagDecIntermediateNodeScope *node_scope) {
	JagDecIntermediateNodeScopePrivate *priv = JAG_DEC_INTERMEDIATE_NODE_SCOPE_GET_PRIVATE(node_scope);
	int lcJumpIndex = priv->jump_index;
	int lcExitIndex = priv->exit_index;
	if (lcJumpIndex==JAG_DEC_SCOPE_INDEX_NONE && lcExitIndex!=JAG_DEC_SCOPE_INDEX_NONE) {
		return lcExitIndex;
	}
	if (lcJumpIndex!=JAG_DEC_SCOPE_INDEX_NONE && lcExitIndex==JAG_DEC_SCOPE_INDEX_NONE) {
		return lcJumpIndex;
	}

	return JAG_DEC_SCOPE_INDEX_NONE;
}

CatStringWo *jag_dec_intermediate_node_scope_as_text(JagDecIntermediateNodeScope *node_scope, CatStringWo *e_dump_buffer) {
	JagDecIntermediateNodeScopePrivate *priv = JAG_DEC_INTERMEDIATE_NODE_SCOPE_GET_PRIVATE(node_scope);
	if (e_dump_buffer == NULL) {
		e_dump_buffer = cat_string_wo_new();
	}
	cat_string_wo_append_chars(e_dump_buffer, "Scope[");
	switch(priv->exit_index) {
		case JAG_DEC_SCOPE_INDEX_EXIT : {
			cat_string_wo_append_chars(e_dump_buffer, "exit-index=exit, ");
		} break;
		case JAG_DEC_SCOPE_INDEX_NONE : {
		} break;
		default : {
			cat_string_wo_append_chars(e_dump_buffer, "exit-index=");
			cat_string_wo_append_decimal(e_dump_buffer, priv->exit_index);
			cat_string_wo_append_chars(e_dump_buffer, ", ");
		} break;
	}
	switch(priv->jump_index) {
		case JAG_DEC_SCOPE_INDEX_EXIT : {
			cat_string_wo_append_chars(e_dump_buffer, "jump-index=exit");
		} break;
		case JAG_DEC_SCOPE_INDEX_NONE : {
			cat_string_wo_append_chars(e_dump_buffer, "jump-index=none");
		} break;
		default : {
			cat_string_wo_append_chars(e_dump_buffer, "jump-index=");
			cat_string_wo_append_decimal(e_dump_buffer, priv->jump_index);
		} break;
	}

	cat_string_wo_append_chars(e_dump_buffer, "]");
	return e_dump_buffer;
}
