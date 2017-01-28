/*
   File:    jagdecintermediategoto.c
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 15, 2012
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

#include "jagdecintermediategoto.h"
#include "../../subcode/impl/jagdecsubcodebreak.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagDecIntermediateGoto"
#include <logging/catlog.h>

struct _JagDecIntermediateGotoPrivate {
	gboolean is_a_break;
};

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface);

G_DEFINE_TYPE_WITH_CODE(JagDecIntermediateGoto, jag_dec_intermediate_goto, JAG_DEC_TYPE_ABSTRACT_INTERMEDIATE_ITEM, {
		G_IMPLEMENT_INTERFACE(JAG_DEC_TYPE_IINTERMEDIATE_NODE, l_intermediate_node_iface_init);
});

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_dec_intermediate_goto_class_init(JagDecIntermediateGotoClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagDecIntermediateGotoPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_dec_intermediate_goto_init(JagDecIntermediateGoto *instance) {
	JagDecIntermediateGotoPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_DEC_TYPE_INTERMEDIATE_GOTO, JagDecIntermediateGotoPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagDecIntermediateGoto *instance = JAG_DEC_INTERMEDIATE_GOTO(object);
//	JagDecIntermediateGotoPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagDecIntermediateGoto *jag_dec_intermediate_goto_new(int mnemonic_index, int exitIndex) {
	JagDecIntermediateGoto *result = g_object_new(JAG_DEC_TYPE_INTERMEDIATE_GOTO, NULL);
	cat_ref_anounce(result);
	JagDecIntermediateGotoPrivate *priv = result->priv;
	priv->is_a_break = FALSE;
	jag_dec_abstract_intermediate_item_construct((JagDecAbstractIntermediateItem *) result, mnemonic_index);
	JagDecIntermediateNodeScope *scope = jag_dec_iintermediate_node_get_scope_ref((JagDecIIntermediateNode *) result);
	jag_dec_intermediate_node_scope_set_exit_index(scope, exitIndex);
	cat_unref_ptr(scope);
	return result;
}









/********************* start JagDecIIntermediateNodeInterface implementation *********************/

static void l_intermediate_node_connect_stage_one(JagDecIIntermediateNode *self, JagDecIntermediateRunContext *group_connect_runtime) {
	JagDecIntermediateGotoPrivate *priv = JAG_DEC_INTERMEDIATE_GOTO_GET_PRIVATE(self);
	if (priv->is_a_break) {
		int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
		JagDecSubCodeBreak *sub_code = jag_dec_sub_code_break_new(mnemonic_index);
		jag_dec_intermediate_run_context_add_sub_code(group_connect_runtime, (JagDecISubCode *) sub_code);
		cat_unref_ptr(sub_code);
	}
}

static gboolean l_intermediate_node_is_same(JagDecIIntermediateNode *self, JagDecIIntermediateNode *other_node) {
//	JagDecIntermediateGotoPrivate *priv = JAG_DEC_INTERMEDIATE_GOTO_GET_PRIVATE(self);
	if (JAG_DEC_IS_INTERMEDIATE_GOTO(other_node)) {
		JagDecIntermediateNodeScope *scope = jag_dec_iintermediate_node_get_scope_ref((JagDecIIntermediateNode *) self);
		JagDecIntermediateNodeScope *oscope = jag_dec_iintermediate_node_get_scope_ref((JagDecIIntermediateNode *) other_node);
		gboolean result = jag_dec_intermediate_node_scope_get_exit_index(scope)==jag_dec_intermediate_node_scope_get_exit_index(oscope);
		cat_unref_ptr(scope);
		cat_unref_ptr(oscope);
		return result;
	}
	return FALSE;
}

static JagDecIIntermediateNode *l_intermediate_node_make_copy(JagDecIIntermediateNode *self, gboolean deep) {
//	JagDecIntermediateGotoPrivate *priv = JAG_DEC_INTERMEDIATE_GOTO_GET_PRIVATE(self);
	int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
	JagDecIntermediateNodeScope *scope = jag_dec_iintermediate_node_get_scope_ref((JagDecIIntermediateNode *) self);
	int exit_index = jag_dec_intermediate_node_scope_get_exit_index(scope);
	cat_unref_ptr(scope);
	return (JagDecIIntermediateNode *) jag_dec_intermediate_goto_new(mnemonic_index, exit_index);
}

static void l_intermediate_node_resolve_breaks(JagDecIIntermediateNode *self, int jumpIndex) {
	JagDecIntermediateGotoPrivate *priv = JAG_DEC_INTERMEDIATE_GOTO_GET_PRIVATE(self);
	JagDecIntermediateNodeScope *scope = jag_dec_iintermediate_node_get_scope_ref((JagDecIIntermediateNode *) self);
	if (jumpIndex==jag_dec_intermediate_node_scope_get_exit_index(scope)) {
		int mnemonic_index = jag_dec_abstract_intermediate_item_get_mnemonic_index((JagDecAbstractIntermediateItem *) self);
		jag_dec_intermediate_node_scope_set_exit_index(scope, mnemonic_index+1);
		priv->is_a_break = TRUE;
	}
	cat_unref_ptr(scope);
}


static void l_intermediate_node_dump(JagDecIIntermediateNode *self, CatStringWo *e_dump_buffer, CatStringWo *prefix) {
	JagDecIntermediateGotoPrivate *priv = JAG_DEC_INTERMEDIATE_GOTO_GET_PRIVATE(self);
	cat_string_wo_append(e_dump_buffer, prefix);
	const char *name = g_type_name_from_instance((GTypeInstance *) self);
	cat_string_wo_append_chars(e_dump_buffer, name+6);
	cat_string_wo_append_chars(e_dump_buffer, ": ");
	JagDecIntermediateNodeScope *node_scope = jag_dec_iintermediate_node_get_scope_ref(self);
	jag_dec_intermediate_node_scope_as_text(node_scope, e_dump_buffer);
	cat_unref_ptr(node_scope);
	cat_string_wo_append_chars(e_dump_buffer, ", ");
	cat_string_wo_append_chars(e_dump_buffer, "[is-a-break=");
	cat_string_wo_append_decimal(e_dump_buffer, priv->is_a_break);
	cat_string_wo_append_chars(e_dump_buffer, "]\n");
}

static void l_intermediate_node_iface_init(JagDecIIntermediateNodeInterface *iface) {
	JagDecIIntermediateNodeInterface *p_iface = g_type_interface_peek_parent(iface);
	iface->connectStageOne = l_intermediate_node_connect_stage_one;
	iface->makeCopy = l_intermediate_node_make_copy;
	iface->isSame = l_intermediate_node_is_same;
	iface->dump = l_intermediate_node_dump;

	iface->contains = p_iface->contains;
	iface->getScopeRef = p_iface->getScopeRef;
	iface->resolveBreaks = l_intermediate_node_resolve_breaks;
	iface->tryPrepend = p_iface->tryPrepend;
}

/********************* end JagDecIIntermediateNodeInterface implementation *********************/

