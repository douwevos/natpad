/*
   File:    leakeynodecontext.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 30, 2013
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

#include "leakeynodecontext.h"
#include "leakeynode.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "LeaKeyNodeContext"
#include <logging/catlog.h>

struct _LeaKeyNodeContextPrivate {
	LeaKeyContext *key_context;
};

G_DEFINE_TYPE(LeaKeyNodeContext, lea_key_node_context, LEA_TYPE_KEY_NODE)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static int l_node_hash(LeaKeyNode *self);
static gboolean l_node_equal(LeaKeyNode *node_a, LeaKeyNode *node_b);

static void lea_key_node_context_class_init(LeaKeyNodeContextClass *clazz) {
	g_type_class_add_private(clazz, sizeof(LeaKeyNodeContextPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;

	LeaKeyNodeClass *node_clazz = LEA_KEY_NODE_CLASS(clazz);
	node_clazz->hash = l_node_hash;
	node_clazz->equal = l_node_equal;
}

static void lea_key_node_context_init(LeaKeyNodeContext *instance) {
	LeaKeyNodeContextPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, LEA_TYPE_KEY_NODE_CONTEXT, LeaKeyNodeContextPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	LeaKeyNodeContext *instance = LEA_KEY_NODE_CONTEXT(object);
	LeaKeyNodeContextPrivate *priv = instance->priv;
	cat_unref_ptr(priv->key_context);
	G_OBJECT_CLASS(lea_key_node_context_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(lea_key_node_context_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}


LeaKeyNodeContext *lea_key_node_context_new(LeaKeyContext *key_context) {
	LeaKeyNodeContext *result = g_object_new(LEA_TYPE_KEY_NODE_CONTEXT, NULL);
	cat_ref_anounce(result);
	LeaKeyNodeContextPrivate *priv = result->priv;
	priv->key_context = cat_ref_ptr(key_context);
	lea_key_node_construct((LeaKeyNode *) result);
	return result;
}




static int l_node_hash(LeaKeyNode *self) {
	LeaKeyNodeContextPrivate *priv = LEA_KEY_NODE_CONTEXT_GET_PRIVATE(self);
	return lea_key_context_hash(priv->key_context);
}


static gboolean l_node_equal(LeaKeyNode *node_a, LeaKeyNode *node_b) {
	if (node_a==node_b) {
		return TRUE;
	}
	if (node_a==NULL || node_b==NULL) {
		return FALSE;
	}
	LeaKeyNodeContextPrivate *priv_a = LEA_KEY_NODE_CONTEXT_GET_PRIVATE(node_a);
	LeaKeyNodeContextPrivate *priv_b = LEA_KEY_NODE_CONTEXT_GET_PRIVATE(node_b);
	return lea_key_context_equal(priv_a->key_context, priv_b->key_context);
}
