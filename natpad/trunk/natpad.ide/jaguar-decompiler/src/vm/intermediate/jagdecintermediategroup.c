/*
   File:    jagdecintermediategroup.c
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

#include "jagdecintermediategroup.h"
#include "jagdeciintermediatenode.h"
#include "jagdecintermediatenodescope.h"
#include "jagdecintermediateruncontext.h"
#include <stdlib.h>

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateGroup"
#include <logging/catlog.h>

struct _JagDecIntermediateGroupPrivate {
	CatArrayWo *e_group_list;
	gboolean scope_valid;
	JagDecIntermediateNodeScope *scope;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateGroup, jag_dec_intermediate_group, G_TYPE_OBJECT, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_group_class_init(JagDecIntermediateGroupClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateGroupPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_group_init(JagDecIntermediateGroup *instance) {
	JagDecIntermediateGroupPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_GROUP, JagDecIntermediateGroupPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	JagDecIntermediateGroup *instance = JAG_DEC_INTERMEDIATE_GROUP(object);
	JagDecIntermediateGroupPrivate *priv = instance->priv;
	cat_unref_ptr(priv->e_group_list);
	cat_unref_ptr(priv->scope);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

void jag_dec_intermediate_group_construct(JagDecIntermediateGroup *intermediate_group) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(intermediate_group);
	priv->e_group_list = cat_array_wo_new();
	priv->scope_valid = FALSE;
	priv->scope = jag_dec_intermediate_node_scope_new(-1, -1);
}

void jag_dec_intermediate_group_construct_with_nodes(JagDecIntermediateGroup *intermediate_group, CatArrayWo *e_intermediate_nodes) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(intermediate_group);
	priv->e_group_list = cat_array_wo_new();
	if (cat_array_wo_find_index(e_intermediate_nodes, NULL, -1)>=0) {
		cat_log_error("given nodes contain NULL");
		while(TRUE) {
			int idx = cat_array_wo_find_index(e_intermediate_nodes, NULL, -1);
			if (idx>=0) {
				cat_array_wo_remove(e_intermediate_nodes, idx, NULL);
			} else {
				break;
			}
		}
	}
	cat_array_wo_append_all(priv->e_group_list, e_intermediate_nodes);
	priv->scope_valid = FALSE;
	priv->scope = jag_dec_intermediate_node_scope_new(-1, -1);
}


JagDecIntermediateGroup *jag_dec_intermediate_group_new() {
	JagDecIntermediateGroup *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_GROUP, NULL);
	cat_ref_anounce(result);
	jag_dec_intermediate_group_construct(result);
	return result;
}

JagDecIntermediateGroup *jag_dec_intermediate_group_new_with_nodes(CatArrayWo *e_intermediate_nodes) {
	JagDecIntermediateGroup *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_GROUP, NULL);
	cat_ref_anounce(result);
	jag_dec_intermediate_group_construct_with_nodes(result, e_intermediate_nodes);
	return result;

}




gboolean jag_dec_intermediate_group_add(JagDecIntermediateGroup *intermediate_group, JagDecIIntermediateNode *node) {
	if (node==NULL) {
		cat_log_error("node is NULL");
		return FALSE;
	}
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(intermediate_group);
	int foundIdx = cat_array_wo_find_index(priv->e_group_list, node, -1);
	if (foundIdx>=0) {
		return FALSE;
	}
	priv->scope_valid = FALSE;
	cat_array_wo_append(priv->e_group_list, (GObject *) node);
	return TRUE;
}

void jag_dec_intermediate_group_add_at(JagDecIntermediateGroup *intermediate_group, int index, JagDecIIntermediateNode *node) {
	if (node==NULL) {
		cat_log_error("node is NULL");
		return;
	}
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(intermediate_group);
	priv->scope_valid = FALSE;
	cat_array_wo_insert(priv->e_group_list, (GObject *) node, index);
}


void jag_dec_intermediate_group_replace(JagDecIntermediateGroup *intermediate_group, int index, JagDecIIntermediateNode *node) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(intermediate_group);
	if (node==NULL) {
		cat_log_error("replacing with node with NULL");
		return;
	}
	priv->scope_valid = FALSE;
	cat_array_wo_set(priv->e_group_list, (GObject *) node, index, NULL);
}


JagDecIIntermediateNode *jag_dec_intermediate_group_get(JagDecIntermediateGroup *intermediate_group, int index) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(intermediate_group);
	return (JagDecIIntermediateNode *) cat_array_wo_get(priv->e_group_list, index);
}

JagDecIIntermediateNode *jag_dec_intermediate_group_get_last(JagDecIntermediateGroup *intermediate_group) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(intermediate_group);
	return (JagDecIIntermediateNode *) cat_array_wo_get_last(priv->e_group_list);
}

CatArrayWo *jag_dec_intermediate_group_get_all(JagDecIntermediateGroup *intermediate_group) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(intermediate_group);
	return priv->e_group_list;
}


void jag_dec_intermediate_group_remove(JagDecIntermediateGroup *intermediate_group, JagDecIIntermediateNode *node_to_remove) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(intermediate_group);
	int node_idx = cat_array_wo_find_index(priv->e_group_list, node_to_remove, -1);
	if (node_idx>=0) {
		cat_array_wo_remove(priv->e_group_list, node_idx, NULL);
		priv->scope_valid = FALSE;
	}
}

JagDecIIntermediateNode *jag_dec_intermediate_group_remove_at(JagDecIntermediateGroup *intermediate_group, int index) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(intermediate_group);
	priv->scope_valid = FALSE;
	JagDecIIntermediateNode *result = NULL;
	cat_array_wo_remove(priv->e_group_list, index, (GObject **) (&result));
	return result;
}

int jag_dec_intermediate_group_find_index(JagDecIntermediateGroup *intermediate_group, JagDecIIntermediateNode *node) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(intermediate_group);
	return cat_array_wo_find_index(priv->e_group_list, node, -1);
}

JagDecIIntermediateNode *jag_dec_intermediate_group_find_node(JagDecIntermediateGroup *intermediate_group, int index) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(intermediate_group);
	JagDecIIntermediateNode *result = NULL;
	CatIIterator *iter = cat_array_wo_iterator(priv->e_group_list);
	while(cat_iiterator_has_next(iter)) {
		JagDecIIntermediateNode *node = (JagDecIIntermediateNode *) cat_iiterator_next(iter);
		if (jag_dec_iintermediate_node_contains(node, index)) {
			result = node;
			break;
		}
	}
	cat_unref_ptr(iter);
	return result;
}


int jag_dec_intermediate_group_count(JagDecIntermediateGroup *intermediate_group) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(intermediate_group);
	return cat_array_wo_size(priv->e_group_list);
}


void jag_dec_intermediate_group_dump(JagDecIntermediateGroup *intermediate_group, CatStringWo *a_prefix) {
	cat_log_on_debug({
		CatStringWo *e_buffer = cat_string_wo_new();
		jag_dec_iintermediate_node_dump((JagDecIIntermediateNode *) intermediate_group, e_buffer, a_prefix);
		cat_log_debug("%s", cat_string_wo_getchars(e_buffer));
		cat_unref_ptr(e_buffer);
	})
}




/********************* start JagDecIIntermediateNodeInterface implementation *********************/


static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(self);
	jag_dec_intermediate_run_context_clear_pseudo_stack(group_connect_runtime);
//	log.indent();
	CatIIterator *iter = cat_array_wo_iterator(priv->e_group_list);
	while(cat_iiterator_has_next(iter)) {
		JagDecIIntermediateNode *node = (JagDecIIntermediateNode *) cat_iiterator_next(iter);
		cat_log_debug("%o", node);
		cat_log_indent_level++;
//		try {
			jag_dec_iintermediate_node_connect_stage_one(node, group_connect_runtime);
//		} catch(Throwable t) {
//			t.printStackTrace();
//		}
		jag_dec_intermediate_run_context_dump_stacks(group_connect_runtime);
		cat_log_indent_level--;
	}
	cat_unref_ptr(iter);

//	log.dedent();
}

static void l_intermediate_node_resolve_breaks(JagDecIIntermediateNode *self, int jump_index) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(self);
	CatIIterator *iter = cat_array_wo_iterator(priv->e_group_list);
	while(cat_iiterator_has_next(iter)) {
		JagDecIIntermediateNode *node = (JagDecIIntermediateNode *) cat_iiterator_next(iter);
		jag_dec_iintermediate_node_resolve_breaks(node, jump_index);
	}
	cat_unref_ptr(iter);

}

static JagDecIntermediateNodeScope *l_intermediate_node_get_scope_ref(JagDecIIntermediateNode *self) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(self);

	if (!priv->scope_valid) {
		priv->scope_valid = TRUE;
		jag_dec_intermediate_node_scope_unset(priv->scope);
		if (cat_array_wo_size(priv->e_group_list)>0) {
			JagDecIIntermediateNode *lastNode = (JagDecIIntermediateNode *) cat_array_wo_get_last(priv->e_group_list);
			cat_log_trace("lastNode=%p  %s", lastNode, (lastNode==NULL ? "nil" : g_type_name_from_instance((GTypeInstance *) lastNode)));
			JagDecIntermediateNodeScope *last_node_scope = jag_dec_iintermediate_node_get_scope_ref(lastNode);
			jag_dec_intermediate_node_scope_copy_from(priv->scope, last_node_scope);
			cat_unref_ptr(last_node_scope);
		}
	}
	return cat_ref_ptr(priv->scope);
}

static gboolean l_intermediate_node_contains(JagDecIIntermediateNode *self, int mnemonic_index) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(self);
	gboolean result = FALSE;
	CatIIterator *iter = cat_array_wo_iterator(priv->e_group_list);
	while(cat_iiterator_has_next(iter)) {
		JagDecIIntermediateNode *node = (JagDecIIntermediateNode *) cat_iiterator_next(iter);
		if (jag_dec_iintermediate_node_contains(node, mnemonic_index)) {
			result = TRUE;
			break;
		}
	}
	cat_unref_ptr(iter);
	return result;
}

static gboolean l_intermediate_node_try_prepend(JagDecIIntermediateNode *self, JagDecIIntermediateNode *node) {
	if (node==NULL) {
		cat_log_error("node is NULL");
		return FALSE;
	}
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(self);
	priv->scope_valid = FALSE;
	if (cat_array_wo_size(priv->e_group_list)!=0) {
		JagDecIIntermediateNode *firstNode = (JagDecIIntermediateNode *) cat_array_wo_get_first(priv->e_group_list);
		if (jag_dec_iintermediate_node_try_prepend(firstNode, node)) {
			return TRUE;
		}
	}

	cat_array_wo_insert(priv->e_group_list, (GObject *) node, 0);
	return TRUE;
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
	JagDecIntermediateGroup *this_node = JAG_DEC_INTERMEDIATE_GROUP(self);
//	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(self);
	if (other_node == self) {
		return TRUE;
	}
	if (JAG_DEC_IS_INTERMEDIATE_GROUP(other_node)) {
		JagDecIntermediateGroup *that = JAG_DEC_INTERMEDIATE_GROUP(other_node);
		if (jag_dec_intermediate_group_count(that)!=jag_dec_intermediate_group_count(this_node)) {
			return FALSE;
		}
		int idx;
		int count = jag_dec_intermediate_group_count(this_node);
		for(idx=0; idx<count; idx++) {
			JagDecIIntermediateNode *t = jag_dec_intermediate_group_get(that, idx);
			JagDecIIntermediateNode *t2 = jag_dec_intermediate_group_get(this_node, idx);
			if (!jag_dec_iintermediate_node_is_same(t2, t)) {
				return FALSE;
			}
		}
		return TRUE;
	}
	return FALSE;

}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(self);
	if (deep) {
		JagDecIntermediateGroup *result = jag_dec_intermediate_group_new();
		CatIIterator *iter = cat_array_wo_iterator(priv->e_group_list);
		while(cat_iiterator_has_next(iter)) {
			JagDecIIntermediateNode *sub = (JagDecIIntermediateNode *) cat_iiterator_next(iter);
			JagDecIIntermediateNode *copy_of_sub = jag_dec_iintermediate_node_make_copy(sub, TRUE);
			jag_dec_intermediate_group_add(result, copy_of_sub);
			cat_unref_ptr(copy_of_sub);
		}
		cat_unref_ptr(iter);
		return (JagDecIIntermediateNode *) result;
	}
	return (JagDecIIntermediateNode *) jag_dec_intermediate_group_new_with_nodes(priv->e_group_list);
}

static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	JagDecIntermediateGroupPrivate *priv = JAG_DEC_INTERMEDIATE_GROUP_GET_PRIVATE(self);

	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	char buf[50];
	sprintf(buf, "(%p)", self);
	cat_string_wo_append_chars(e_dump_buffer, buf);
	cat_string_wo_append_chars(e_dump_buffer, ": ");
	JagDecIntermediateNodeScope *node_scope = l_intermediate_node_get_scope_ref(self);
	jag_dec_intermediate_node_scope_as_text(node_scope, e_dump_buffer);
	cat_unref_ptr(node_scope);

	cat_string_wo_append_char(e_dump_buffer, '\n');
	CatStringWo *e_prefix = cat_string_wo_clone(prefix, CAT_CLONE_DEPTH_FULL);
	cat_string_wo_append_chars(e_prefix, "   ");

	CatIIterator *iter = cat_array_wo_iterator(priv->e_group_list);
	while(cat_iiterator_has_next(iter)) {
		JagDecIIntermediateNode *child = (JagDecIIntermediateNode *) cat_iiterator_next(iter);
		jag_dec_iintermediate_node_dump(child, e_dump_buffer, e_prefix);
	}
	cat_unref_ptr(iter);
	cat_unref_ptr(e_prefix);
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
