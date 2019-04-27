/*
   File:    jagastmodifiers.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 30, 2012
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

#include "jagastmodifiers.h"

#include <logging/catlogdefs.h>
#define CAT_LOG_LEVEL CAT_LOG_WARN
#define CAT_LOG_CLAZZ "JagAstModifiers"
#include <logging/catlog.h>

struct _JagAstModifiersPrivate {
	int modifiers;
};

G_DEFINE_TYPE_WITH_PRIVATE(JagAstModifiers, jag_ast_modifiers, G_TYPE_OBJECT)

static void l_dispose(GObject *object);
static void l_finalize(GObject *object);

static void jag_ast_modifiers_class_init(JagAstModifiersClass *clazz) {
	GObjectClass *object_class = G_OBJECT_CLASS(clazz);
	object_class->dispose = l_dispose;
	object_class->finalize = l_finalize;
}

static void jag_ast_modifiers_init(JagAstModifiers *instance) {
}

static void l_dispose(GObject *object) {
	cat_log_detail("dispose:%p", object);
	G_OBJECT_CLASS(jag_ast_modifiers_parent_class)->dispose(object);
	cat_log_detail("disposed:%p", object);
}

static void l_finalize(GObject *object) {
	cat_log_detail("finalize:%p", object);
	cat_ref_denounce(object);
	G_OBJECT_CLASS(jag_ast_modifiers_parent_class)->finalize(object);
	cat_log_detail("finalized:%p", object);
}

JagAstModifiers *jag_ast_modifiers_new(short modifiers) {
	JagAstModifiers *result = g_object_new(JAG_AST_TYPE_MODIFIERS, NULL);
	cat_ref_anounce(result);
	JagAstModifiersPrivate *priv = jag_ast_modifiers_get_instance_private(result);
	priv->modifiers = modifiers;
	return result;
}

void jag_ast_modifiers_set(JagAstModifiers *modifiers, JagAstModifier modifier) {
	JagAstModifiersPrivate *priv = jag_ast_modifiers_get_instance_private(modifiers);
	priv->modifiers |= modifier;
}


CatStringWo *jag_ast_modifiers_as_text(JagAstModifiers *modifiers) {
	JagAstModifiersPrivate *priv = jag_ast_modifiers_get_instance_private(modifiers);
	int mods = priv->modifiers;
	CatStringWo *e_result = cat_string_wo_clone(CAT_S(cat_s_empty_string), CAT_CLONE_DEPTH_MAIN);

	if ((mods & JAG_AST_MODIFIER_PUBLIC)!=0) {
		cat_string_wo_append_chars(e_result, "public ");
	}
	if ((mods & JAG_AST_MODIFIER_PRIVATE)!=0) {
		cat_string_wo_append_chars(e_result, "private ");
	}
	if ((mods & JAG_AST_MODIFIER_PROTECTED)!=0) {
		cat_string_wo_append_chars(e_result, "protected ");
	}
	if ((mods & JAG_AST_MODIFIER_FINAL)!=0) {
		cat_string_wo_append_chars(e_result, "final ");
	}
	if ((mods & JAG_AST_MODIFIER_STATIC)!=0) {
		cat_string_wo_append_chars(e_result, "static ");
	}
	if ((mods & JAG_AST_MODIFIER_VOLATILE)!=0) {
		cat_string_wo_append_chars(e_result, "volatile ");
	}
	if ((mods & JAG_AST_MODIFIER_TRANSIENT)!=0) {
		cat_string_wo_append_chars(e_result, "transient ");
	}
	if ((mods & JAG_AST_MODIFIER_NATIVE)!=0) {
		cat_string_wo_append_chars(e_result, "native ");
	}
	if ((mods & JAG_AST_MODIFIER_SYNCHRONIZED)!=0) {
		cat_string_wo_append_chars(e_result, "synchronized ");
	}
	if ((mods & JAG_AST_MODIFIER_INTERFACE)!=0) {
		cat_string_wo_append_chars(e_result, "interface ");
	}
	return e_result;
}


gboolean jag_ast_modifiers_is_static(JagAstModifiers *modifiers) {
	JagAstModifiersPrivate *priv = jag_ast_modifiers_get_instance_private(modifiers);
	return ((priv->modifiers & JAG_AST_MODIFIER_STATIC)!=0);
}

gboolean jag_ast_modifiers_equal(JagAstModifiers *modifiers_a, JagAstModifiers *modifiers_b) {
	if (modifiers_a==modifiers_b) {
		return TRUE;
	}
	if (modifiers_a==NULL || modifiers_b==NULL) {
		return FALSE;
	}
	JagAstModifiersPrivate *priv_a = jag_ast_modifiers_get_instance_private(modifiers_a);
	JagAstModifiersPrivate *priv_b = jag_ast_modifiers_get_instance_private(modifiers_b);
	return priv_a->modifiers==priv_b->modifiers;
}


const char *jag_ast_modifier_as_text(JagAstModifier modifier) {
	switch(modifier) {
		case JAG_AST_MODIFIER_PUBLIC : return "public";
		case JAG_AST_MODIFIER_PRIVATE : return "private";
		case JAG_AST_MODIFIER_PROTECTED : return "protected";
		case JAG_AST_MODIFIER_STATIC : return "static";
		case JAG_AST_MODIFIER_FINAL : return "final";
		case JAG_AST_MODIFIER_SYNCHRONIZED : return "synchronized";
		case JAG_AST_MODIFIER_VOLATILE : return "volatile";
		case JAG_AST_MODIFIER_TRANSIENT : return "transient";
		case JAG_AST_MODIFIER_NATIVE : return "native";
		case JAG_AST_MODIFIER_INTERFACE : return "interface";
	}
	return "???";
}
