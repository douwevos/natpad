/*
   File:    jagdeciintermediatenode.c
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

#include "jagdeciintermediatenode.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIIntermediateNode"
#include <logging/catlog.h>


G_DEFINE_INTERFACE(JagDecIIntermediateNode, jag_dec_iintermediate_node, G_TYPE_OBJECT);

static void jag_dec_iintermediate_node_default_init (JagDecIIntermediateNodeInterface *iface) {
	iface->dump = NULL;
}

void jag_dec_iintermediate_node_connect_stage_one(JagDecIIntermediateNode *self, struct _JagDecIntermediateRunContext *groupConnectRuntime) {
	g_return_if_fail(JAG_DEC_IS_IINTERMEDIATE_NODE(self));
	JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(self)->connectStageOne(self, groupConnectRuntime);
}

void jag_dec_iintermediate_node_resolve_breaks(JagDecIIntermediateNode *self, int jumpIndex)  {
	g_return_if_fail(JAG_DEC_IS_IINTERMEDIATE_NODE(self));
	JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(self)->resolveBreaks(self, jumpIndex);
}

struct _JagDecIntermediateNodeScope *jag_dec_iintermediate_node_get_scope_ref(JagDecIIntermediateNode *self) {
	g_return_val_if_fail(JAG_DEC_IS_IINTERMEDIATE_NODE(self), NULL);
	return JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(self)->getScopeRef(self);
}

gboolean jag_dec_iintermediate_node_contains(JagDecIIntermediateNode *self, int mnemonicIndex) {
	g_return_val_if_fail(JAG_DEC_IS_IINTERMEDIATE_NODE(self), FALSE);
	return JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(self)->contains(self, mnemonicIndex);
}


gboolean jag_dec_iintermediate_node_try_prepend(JagDecIIntermediateNode *self, struct _JagDecIIntermediateNode *node) {
	g_return_val_if_fail(JAG_DEC_IS_IINTERMEDIATE_NODE(self), FALSE);
	return JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(self)->tryPrepend(self, node);
}

gboolean jag_dec_iintermediate_node_is_same(JagDecIIntermediateNode *self, struct _JagDecIIntermediateNode *otherNode) {
	if (self==otherNode) {
		return TRUE;
	}
	if (self==NULL || otherNode==NULL) {
		return FALSE;
	}
	g_return_val_if_fail(JAG_DEC_IS_IINTERMEDIATE_NODE(self), FALSE);
	return JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(self)->isSame(self, otherNode);
}

struct _JagDecIIntermediateNode *jag_dec_iintermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	g_return_val_if_fail(JAG_DEC_IS_IINTERMEDIATE_NODE(self), NULL);
	return JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(self)->makeCopy(self, deep);
}

void jag_dec_iintermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	g_return_if_fail(JAG_DEC_IS_IINTERMEDIATE_NODE(self));
	JagDecIIntermediateNodeInterface *iface = JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(self);
	if (iface->dump) {
		iface->dump(self, e_dump_buffer, prefix);
	} else {
		cat_log_error("JagDecIIntermediateNode->dump not implemented for instance:%s", g_type_name_from_instance((GTypeInstance *) self));
	}
}
