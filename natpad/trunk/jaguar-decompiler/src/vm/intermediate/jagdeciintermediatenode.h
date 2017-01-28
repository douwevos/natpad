/*
   File:    jagdeciintermediatenode.h
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


#ifndef JAGDECIINTERMEDIATENODE_H_
#define JAGDECIINTERMEDIATENODE_H_


#include <caterpillar.h>

#define JAG_DEC_TYPE_IINTERMEDIATE_NODE                 (jag_dec_iintermediate_node_get_type())
#define JAG_DEC_IINTERMEDIATE_NODE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_DEC_TYPE_IINTERMEDIATE_NODE, JagDecIIntermediateNode))
#define JAG_DEC_IS_IINTERMEDIATE_NODE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_DEC_TYPE_IINTERMEDIATE_NODE))
#define JAG_DEC_IINTERMEDIATE_NODE_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_DEC_TYPE_IINTERMEDIATE_NODE, JagDecIIntermediateNodeInterface))


typedef struct _JagDecIIntermediateNode               JagDecIIntermediateNode; /* dummy object */
typedef struct _JagDecIIntermediateNodeInterface      JagDecIIntermediateNodeInterface;

struct _JagDecIntermediateRunContext;

struct _JagDecIIntermediateNodeInterface {
	GTypeInterface parent_iface;


	void (*connectStageOne)(JagDecIIntermediateNode *self, struct _JagDecIntermediateRunContext *groupConnectRuntime);
	void (*resolveBreaks)(JagDecIIntermediateNode *self, int jumpIndex);

	struct _JagDecIntermediateNodeScope *(*getScopeRef)(JagDecIIntermediateNode *self);

	gboolean (*contains)(JagDecIIntermediateNode *self, int mnemonicIndex);

//void dump(JagDecIIntermediateNode *self, StringBuilder dumpBuffer, String prefix);

	gboolean (*tryPrepend)(JagDecIIntermediateNode *self, struct _JagDecIIntermediateNode *node);

	gboolean (*isSame)(JagDecIIntermediateNode *self, struct _JagDecIIntermediateNode *otherNode);

	struct _JagDecIIntermediateNode *(*makeCopy)(JagDecIIntermediateNode *self, gboolean deep);

	void (*dump)(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix);

};

GType jag_dec_iintermediate_node_get_type(void);


void jag_dec_iintermediate_node_connect_stage_one(JagDecIIntermediateNode *self, struct _JagDecIntermediateRunContext *groupConnectRuntime);
void jag_dec_iintermediate_node_resolve_breaks(JagDecIIntermediateNode *self, int jumpIndex);

struct _JagDecIntermediateNodeScope *jag_dec_iintermediate_node_get_scope_ref(JagDecIIntermediateNode *self);

gboolean jag_dec_iintermediate_node_contains(JagDecIIntermediateNode *self, int mnemonicIndex);


gboolean jag_dec_iintermediate_node_try_prepend(JagDecIIntermediateNode *self, struct _JagDecIIntermediateNode *node);

gboolean jag_dec_iintermediate_node_is_same(JagDecIIntermediateNode *self, struct _JagDecIIntermediateNode *otherNode);

struct _JagDecIIntermediateNode *jag_dec_iintermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep);


void jag_dec_iintermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix);


#endif /* JAGDECIINTERMEDIATENODE_H_ */
