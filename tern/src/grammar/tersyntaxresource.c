/*
   File:    tersyntaxresource.c
   Project: tern
   Author:  Douwe Vos
   Date:    Aug 6, 2012
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

#include "tersyntaxresource.h"
#include "scanner/tergrammarreader.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "TerSyntaxResource"
#include <logging/catlog.h>

struct _TerSyntaxResourcePrivate {
	CatTreeNode *vip_tree_node;
	TerSyntax *syntax;
	int syntax_version;
};

G_DEFINE_TYPE (TerSyntaxResource, ter_syntax_resource, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void ter_syntax_resource_class_init(TerSyntaxResourceClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(TerSyntaxResourcePrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void ter_syntax_resource_init(TerSyntaxResource *instance) {
	TerSyntaxResourcePrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, TER_TYPE_SYNTAX_RESOURCE, TerSyntaxResourcePrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	TerSyntaxResource *instance = TER_SYNTAX_RESOURCE(object);
	TerSyntaxResourcePrivate *priv = instance->priv;
	cat_unref_ptr(priv->vip_tree_node)
	cat_unref_ptr(priv->syntax);
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

TerSyntaxResource *ter_syntax_resource_new(CatTreeNode *vip_tree_node) {
	TerSyntaxResource *result = g_object_new(TER_TYPE_SYNTAX_RESOURCE, NULL);
	cat_ref_anounce(result);
	TerSyntaxResourcePrivate *priv = result->priv;
	cat_log_debug("syntax_resource.priv=%p", priv);
	priv->vip_tree_node = cat_ref_ptr(vip_tree_node);
	priv->syntax = NULL;
	priv->syntax_version = 0;
	return result;
}



gboolean ter_syntax_resource_reload_syntax(TerSyntaxResource *syntax_resource) {
	cat_log_debug("syntax_resource=%p", syntax_resource);
	TerSyntaxResourcePrivate *priv = TER_SYNTAX_RESOURCE_GET_PRIVATE(syntax_resource);
	cat_log_debug("syntax_resource.priv=%p", priv);
	cat_log_debug("priv->vip_tree_node=%p", priv->vip_tree_node);
	VipNode *vip_node = (VipNode *) cat_tree_node_get_content(priv->vip_tree_node);
	VipIResource *resource = vip_node_get_content(vip_node);
	cat_log_debug("vip_node=%o", vip_node);
	gboolean result = FALSE;
	if (VIP_IS_IFILE(resource)) {
		CatIInputStream *in_stream = vip_ifile_open_input_stream((VipIFile *) resource);
		TerSyntax *ts = ter_grammar_reader_read(in_stream);
		cat_ref_swap(priv->syntax, ts);
		result = TRUE;
	}
	return result;
}


TerSyntax *ter_syntax_resource_get_or_load_syntax(TerSyntaxResource *syntax_resource) {
	cat_log_debug("syntax_resource=%p", syntax_resource);
	TerSyntaxResourcePrivate *priv = TER_SYNTAX_RESOURCE_GET_PRIVATE(syntax_resource);
	cat_log_debug("syntax_resource.priv=%p", priv);
	if (priv->syntax==NULL) {
		ter_syntax_resource_reload_syntax(syntax_resource);
	}
	return priv->syntax;
}

