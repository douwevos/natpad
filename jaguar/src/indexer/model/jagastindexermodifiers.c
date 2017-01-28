/*
   File:    jagastindexermodifiers.c
   Project: jaguar
   Author:  Douwe Vos
   Date:    Aug 26, 2012
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

#include "jagastindexermodifiers.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstIndexerModifiers"
#include <logging/catlog.h>


#define L_AST_MODIFIER_PUBLIC           0x0001
#define L_AST_MODIFIER_PROTECTED        0x0002
#define L_AST_MODIFIER_PRIVATE          0x0004
#define L_AST_MODIFIER_STATIC           0x0008
#define L_AST_MODIFIER_ABSTRACT         0x0010
#define L_AST_MODIFIER_FINAL            0x0020
#define L_AST_MODIFIER_NATIVE           0x0040
#define L_AST_MODIFIER_SYNCHRONIZED     0x0080
#define L_AST_MODIFIER_TRANSIENT        0x0100
#define L_AST_MODIFIER_VOLATILE         0x0200
#define L_AST_MODIFIER_STRICTFP         0x0400


struct _JagAstIndexerModifiersPrivate {
	int modifiers;
};

G_DEFINE_TYPE (JagAstIndexerModifiers, jag_ast_indexer_modifiers, G_TYPE_OBJECT)

static gpointer parent_class = NULL;

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_indexer_modifiers_class_init(JagAstIndexerModifiersClass *clazz) {
	parent_class = g_type_class_peek_parent(clazz);
	g_type_class_add_private(clazz, sizeof(JagAstIndexerModifiersPrivate));

	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_indexer_modifiers_init(JagAstIndexerModifiers *instance) {
	JagAstIndexerModifiersPrivate *priv = G_TYPE_INSTANCE_GET_PRIVATE(instance, JAG_TYPE_AST_INDEXER_MODIFIERS, JagAstIndexerModifiersPrivate);
	instance->priv = priv;
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
//	JagAstIndexerModifiers *instance = jag_ast_indexer_modifiers(object);
//	JagAstIndexerModifiersPrivate *priv = instance->priv;
	G_OBJECT_CLASS(parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}



JagAstIndexerModifiers *jag_ast_indexer_modifiers_new(JagAstIndexerModifier initial_modifier) {
	JagAstIndexerModifiers *result = g_object_new(JAG_TYPE_AST_INDEXER_MODIFIERS, NULL);
	cat_ref_anounce(result);
	JagAstIndexerModifiersPrivate *priv = result->priv;
	priv->modifiers = 0;
	jag_ast_indexer_modifiers_set(result, initial_modifier);
	return result;
}

void jag_ast_indexer_modifiers_set(JagAstIndexerModifiers *modifiers, JagAstIndexerModifier modifier) {
	JagAstIndexerModifiersPrivate *priv = JAG_AST_INDEXER_MODIFIERS_GET_PRIVATE(modifiers);
	switch(modifier) {
		case JAG_AST_INDEXER_MODIFIER_PUBLIC : { priv->modifiers = priv->modifiers | L_AST_MODIFIER_PUBLIC; } break;
		case JAG_AST_INDEXER_MODIFIER_PROTECTED : { priv->modifiers = priv->modifiers | L_AST_MODIFIER_PROTECTED; } break;
		case JAG_AST_INDEXER_MODIFIER_PRIVATE : { priv->modifiers = priv->modifiers | L_AST_MODIFIER_PRIVATE; } break;
		case JAG_AST_INDEXER_MODIFIER_STATIC : { priv->modifiers = priv->modifiers | L_AST_MODIFIER_STATIC; } break;
		case JAG_AST_INDEXER_MODIFIER_ABSTRACT : { priv->modifiers = priv->modifiers | L_AST_MODIFIER_ABSTRACT; } break;
		case JAG_AST_INDEXER_MODIFIER_FINAL : { priv->modifiers = priv->modifiers | L_AST_MODIFIER_FINAL; } break;
		case JAG_AST_INDEXER_MODIFIER_NATIVE : { priv->modifiers = priv->modifiers | L_AST_MODIFIER_NATIVE; } break;
		case JAG_AST_INDEXER_MODIFIER_SYNCHRONIZED : { priv->modifiers = priv->modifiers | L_AST_MODIFIER_SYNCHRONIZED; } break;
		case JAG_AST_INDEXER_MODIFIER_TRANSIENT : { priv->modifiers = priv->modifiers | L_AST_MODIFIER_TRANSIENT; } break;
		case JAG_AST_INDEXER_MODIFIER_VOLATILE : { priv->modifiers = priv->modifiers | L_AST_MODIFIER_VOLATILE; } break;
		case JAG_AST_INDEXER_MODIFIER_STRICTFP : { priv->modifiers = priv->modifiers | L_AST_MODIFIER_STRICTFP; } break;
		case JAG_AST_INDEXER_MODIFIER_NONE : break;
	}
}

JagAstIndexerModifiers *jag_ast_indexer_modifiers_include(JagAstIndexerModifiers *modifiers, JagAstIndexerModifiers *modifiers_to_include) {
	JagAstIndexerModifiersPrivate *priv = JAG_AST_INDEXER_MODIFIERS_GET_PRIVATE(modifiers);
	JagAstIndexerModifiersPrivate *priv_other = JAG_AST_INDEXER_MODIFIERS_GET_PRIVATE(modifiers_to_include);
	priv->modifiers = priv->modifiers|priv_other->modifiers;
	return modifiers;
}



gboolean jag_ast_indexer_modifiers_is_public(JagAstIndexerModifiers *modifiers) {
	JagAstIndexerModifiersPrivate *priv = JAG_AST_INDEXER_MODIFIERS_GET_PRIVATE(modifiers);
	return (priv->modifiers&L_AST_MODIFIER_PUBLIC)!=0 ? TRUE : FALSE;
}

gboolean jag_ast_indexer_modifiers_is_protected(JagAstIndexerModifiers *modifiers) {
	JagAstIndexerModifiersPrivate *priv = JAG_AST_INDEXER_MODIFIERS_GET_PRIVATE(modifiers);
	return (priv->modifiers&L_AST_MODIFIER_PROTECTED)!=0 ? TRUE : FALSE;
}

gboolean jag_ast_indexer_modifiers_is_private(JagAstIndexerModifiers *modifiers) {
	JagAstIndexerModifiersPrivate *priv = JAG_AST_INDEXER_MODIFIERS_GET_PRIVATE(modifiers);
	return (priv->modifiers&L_AST_MODIFIER_PRIVATE)!=0 ? TRUE : FALSE;
}

gboolean jag_ast_indexer_modifiers_is_static(JagAstIndexerModifiers *modifiers) {
	JagAstIndexerModifiersPrivate *priv = JAG_AST_INDEXER_MODIFIERS_GET_PRIVATE(modifiers);
	return (priv->modifiers&L_AST_MODIFIER_STATIC)!=0 ? TRUE : FALSE;
}

gboolean jag_ast_indexer_modifiers_is_abstract(JagAstIndexerModifiers *modifiers) {
	JagAstIndexerModifiersPrivate *priv = JAG_AST_INDEXER_MODIFIERS_GET_PRIVATE(modifiers);
	return (priv->modifiers&L_AST_MODIFIER_ABSTRACT)!=0 ? TRUE : FALSE;
}

gboolean jag_ast_indexer_modifiers_is_final(JagAstIndexerModifiers *modifiers) {
	JagAstIndexerModifiersPrivate *priv = JAG_AST_INDEXER_MODIFIERS_GET_PRIVATE(modifiers);
	return (priv->modifiers&L_AST_MODIFIER_FINAL)!=0 ? TRUE : FALSE;
}

gboolean jag_ast_indexer_modifiers_is_native(JagAstIndexerModifiers *modifiers) {
	JagAstIndexerModifiersPrivate *priv = JAG_AST_INDEXER_MODIFIERS_GET_PRIVATE(modifiers);
	return (priv->modifiers&L_AST_MODIFIER_NATIVE)!=0 ? TRUE : FALSE;
}

gboolean jag_ast_indexer_modifiers_is_synchronized(JagAstIndexerModifiers *modifiers) {
	JagAstIndexerModifiersPrivate *priv = JAG_AST_INDEXER_MODIFIERS_GET_PRIVATE(modifiers);
	return (priv->modifiers&L_AST_MODIFIER_SYNCHRONIZED)!=0 ? TRUE : FALSE;
}

gboolean jag_ast_indexer_modifiers_is_transient(JagAstIndexerModifiers *modifiers) {
	JagAstIndexerModifiersPrivate *priv = JAG_AST_INDEXER_MODIFIERS_GET_PRIVATE(modifiers);
	return (priv->modifiers&L_AST_MODIFIER_TRANSIENT)!=0 ? TRUE : FALSE;
}

gboolean jag_ast_indexer_modifiers_is_volatile(JagAstIndexerModifiers *modifiers) {
	JagAstIndexerModifiersPrivate *priv = JAG_AST_INDEXER_MODIFIERS_GET_PRIVATE(modifiers);
	return (priv->modifiers&L_AST_MODIFIER_VOLATILE)!=0 ? TRUE : FALSE;
}

gboolean jag_ast_indexer_modifiers_is_strictfp(JagAstIndexerModifiers *modifiers) {
	JagAstIndexerModifiersPrivate *priv = JAG_AST_INDEXER_MODIFIERS_GET_PRIVATE(modifiers);
	return (priv->modifiers&L_AST_MODIFIER_STRICTFP)!=0 ? TRUE : FALSE;
}





