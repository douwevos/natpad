/*
   File:    jagdecabstractintermediateitem.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 9, 2012
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

#include "jagdecabstractintermediateitem.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecAbstractIntermediateItem"
#include <logging/catlog.h>

struct _JagDecAbstractIntermediateItemPrivate {
	int mnemonic_index;
	JagDecIntermediateNodeScope *node_scope;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_ABSTRACT_TYPE_WITH_CODE(JagDecAbstractIntermediateItem, jag_dec_abstract_intermediate_item, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_abstract_intermediate_item_class_init(JagDecAbstractIntermediateItemClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecAbstractIntermediateItemPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_abstract_intermediate_item_init(JagDecAbstractIntermediateItem *instance) {
	JagDecAbstractIntermediateItemPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, JagDecAbstractIntermediateItemPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecAbstractIntermediateItem *instance = JAG_DEC_ABSTRACT_INTERMEDIATE_ITEM(object);
	JagDecAbstractIntermediateItemPrivate *priv = instance->priv;
	cat_unref_ptr(priv->node_scope);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void jag_dec_abstract_intermediate_item_construct(JagDecAbstractIntermediateItem *item, int mnemonic_index) {
	JagDecAbstractIntermediateItemPrivate *priv = JAG_DEC_ABSTRACT_INTERMEDIATE_ITEM_GET_PRIVATE(item);
	priv->mnemonic_index = mnemonic_index;
	priv->node_scope = jag_dec_intermediate_node_scope_new(mnemonic_index+1, JAG_DEC_SCOPE_INDEX_NONE);
}


int jag_dec_abstract_intermediate_item_get_mnemonic_index(JagDecAbstractIntermediateItem *item) {
	JagDecAbstractIntermediateItemPrivate *priv = JAG_DEC_ABSTRACT_INTERMEDIATE_ITEM_GET_PRIVATE(item);
	return priv->mnemonic_index;
}



/********************* start JagDecIIntermediateNodeInterface implementation *********************/


static JagDecIntermediateNodeScope *l_intermediate_node_get_scope_ref(JagDecIIntermediateNode *self) {
	JagDecAbstractIntermediateItemPrivate *priv = JAG_DEC_ABSTRACT_INTERMEDIATE_ITEM_GET_PRIVATE(self);
	return cat_ref_ptr(priv->node_scope);
}

static void l_intermediate_node_resolve_breaks(JagDecIIntermediateNode *self, int jump_index) {
}

static gboolean l_intermediate_node_contains(JagDecIIntermediateNode *self, int mnemonic_index) {
	JagDecAbstractIntermediateItemPrivate *priv = JAG_DEC_ABSTRACT_INTERMEDIATE_ITEM_GET_PRIVATE(self);
	cat_log_trace("JagDecIIntermediateNodeInterface->contains %s   priv->mnemonic_index=%d  mnemonic_index=%d ", g_type_name_from_instance((GTypeInstance *) self), priv->mnemonic_index, mnemonic_index);
	return priv->mnemonic_index == mnemonic_index;
}

static gboolean l_intermediate_node_try_prepend(JagDecIIntermediateNode *self, JagDecIIntermediateNode *node) {
	return FALSE;
}


static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	cat_log_error("JagDecIIntermediateNodeInterface->connectStageOne not implemented by:%s", g_type_name_from_instance((GTypeInstance *) self));
}




static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	cat_log_error("JagDecIIntermediateNodeInterface->isSame not implemented by:%s", g_type_name_from_instance((GTypeInstance *) self));
	return self==other_node;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	cat_log_error("JagDecIIntermediateNodeInterface->makeCopy not implemented by:%s", g_type_name_from_instance((GTypeInstance *) self));
	return NULL;
}

static void l_intermediate_node_dump(JagDecIIntermediateNode *item, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) item);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	cat_string_wo_append_char(e_dump_buffer, '\n');
}


static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface) {
	iface->connectStageOne = l_intermediate_node_connect_stage_one;
	iface->contains = l_intermediate_node_contains;
	iface->getScopeRef = l_intermediate_node_get_scope_ref;
	iface->isSame = l_intermediate_node_is_same;
	iface->makeCopy = l_intermediate_node_make_copy;
	iface->resolveBreaks = l_intermediate_node_resolve_breaks;
	iface->tryPrepend = l_intermediate_node_try_prepend;
	iface->dump = l_intermediate_node_dump;
}

/********************* end JagDecIIntermediateNodeInterface implementation *********************/
