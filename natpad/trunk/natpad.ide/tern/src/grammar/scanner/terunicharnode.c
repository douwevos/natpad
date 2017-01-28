/*
   File:    terunicharnode.c
   Project: tern
   Author:  Douwe Vos
   Date:    Jun 2, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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

#include "terunicharnode.h"
#include <caterpillar.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerUnicharNode"
#include <logging/catlog.h>

struct _TerUnicharNodePrivate {
	gunichar unichar;
	gushort level;
	CatHashMapWo *e_children;
	gushort code_action;
	gushort code_keyword_ns1;
	gushort code_keyword_ns2;
};

G_DEFINE_TYPE (TerUnicharNode, ter_unichar_node, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void ter_unichar_node_class_init(TerUnicharNodeClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(TerUnicharNodePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_unichar_node_init(TerUnicharNode *instance) {
	TerUnicharNodePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, TER_TYPE_UNICHAR_NODE, TerUnicharNodePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TerUnicharNode *instance = TER_UNICHAR_NODE(object);
	TerUnicharNodePrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_children);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

TerUnicharNode *ter_unichar_node_new(gunichar unichar, gushort level) {
	TerUnicharNode *result = g_object_new(TER_TYPE_UNICHAR_NODE, NULL);
	cat_ref_anounce(result);
	TerUnicharNodePrivate *priv = result->priv;
	priv->unichar = unichar;
	priv->e_children = NULL;
	priv->level = level;
	priv->code_keyword_ns1 = 0;
	priv->code_keyword_ns2 = 0;
	priv->code_action = 0;
	return result;
}


TerUnicharNode *ter_unichar_node_get(TerUnicharNode *node, gunichar unichar) {
	TerUnicharNodePrivate *priv = TER_UNICHAR_NODE_GET_PRIVATE(node);
	TerUnicharNode *result = NULL;
	if (priv->e_children) {
		CatUnichar *key = cat_unichar_new(unichar);
		result = (TerUnicharNode *) cat_hash_map_wo_get(priv->e_children, (GObject *) key);
		cat_unref(key);
	}
	return result;
}

TerUnicharNode *ter_unichar_node_get_or_create(TerUnicharNode *node, gunichar unichar) {
	TerUnicharNodePrivate *priv = TER_UNICHAR_NODE_GET_PRIVATE(node);
	TerUnicharNode *result = ter_unichar_node_get(node, unichar);
	if (result==NULL) {
		CatUnichar *uchobj = cat_unichar_new(unichar);
		result = ter_unichar_node_new(unichar, priv->level+1);
		if (priv->e_children == NULL) {
			priv->e_children = cat_hash_map_wo_new((GHashFunc) cat_unichar_hash, (GEqualFunc) cat_unichar_equal);
		}
		cat_hash_map_wo_put(priv->e_children, (GObject *) uchobj, (GObject *) result);
		cat_unref_ptr(uchobj);
		cat_unref(result);
	}
	return result;
}


void ter_unichar_node_set_keyword_ns1(TerUnicharNode *node, gushort code) {
	TER_UNICHAR_NODE_GET_PRIVATE(node)->code_keyword_ns1 = code;
}

gushort ter_unichar_node_get_keyword_ns1(TerUnicharNode *node) {
	return TER_UNICHAR_NODE_GET_PRIVATE(node)->code_keyword_ns1;
}


gushort ter_unichar_node_get_level(TerUnicharNode *node) {
	return TER_UNICHAR_NODE_GET_PRIVATE(node)->level;
}


void ter_unichar_node_dump(TerUnicharNode *node, CatStringWo *a_indent) {
	if (a_indent==NULL) {
		a_indent = cat_string_wo_new_with("");
	} else {
		cat_ref_ptr(a_indent);
	}
	TerUnicharNodePrivate *priv = TER_UNICHAR_NODE_GET_PRIVATE(node);

	cat_log_print("DUMP", "%s %p unichar:%d-%c, kw-ns1:%d, kw-ns2:%d, action=%d, level=%d", cat_string_wo_getchars(a_indent), node, priv->unichar, (char) priv->unichar, priv->code_keyword_ns1, priv->code_keyword_ns2, priv->code_action, priv->level);
	if (priv->e_children) {
		CatStringWo *e_nindent = cat_string_wo_clone(a_indent, CAT_CLONE_DEPTH_MAIN);
		cat_string_wo_append_chars(e_nindent, " ");
		CatArrayWo *e_children = cat_hash_map_wo_enlist_values(priv->e_children, NULL);
		if (e_children) {
			CatIIterator *iter = cat_array_wo_iterator(e_children);
			while(cat_iiterator_has_next(iter)) {
				TerUnicharNode *child_node = (TerUnicharNode *) cat_iiterator_next(iter);
				ter_unichar_node_dump(child_node, e_nindent);
			}
			cat_unref_ptr(iter);
			cat_unref_ptr(e_children);
		}
		cat_unref_ptr(e_nindent);
	}
	cat_unref_ptr(a_indent);
}
