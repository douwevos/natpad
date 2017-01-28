/*
   File:    jagastindexermodifiers.h
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


#ifndef JAGASTINDEXERMODIFIERS_H_
#define JAGASTINDEXERMODIFIERS_H_



#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_TYPE_AST_INDEXER_MODIFIERS              (jag_ast_indexer_modifiers_get_type())
#define JAG_AST_INDEXER_MODIFIERS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_indexer_modifiers_get_type(), JagAstIndexerModifiers))
#define JAG_AST_INDEXER_MODIFIERS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_TYPE_AST_INDEXER_MODIFIERS, JagAstIndexerModifiersClass))
#define JAG_IS_AST_INDEXER_MODIFIERS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_TYPE_AST_INDEXER_MODIFIERS))
#define JAG_IS_AST_INDEXER_MODIFIERS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_TYPE_AST_INDEXER_MODIFIERS))
#define JAG_AST_INDEXER_MODIFIERS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_TYPE_AST_INDEXER_MODIFIERS, JagAstIndexerModifiersClass))
#define JAG_AST_INDEXER_MODIFIERS_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_TYPE_AST_INDEXER_MODIFIERS, JagAstIndexerModifiersPrivate))

typedef struct _JagAstIndexerModifiers               JagAstIndexerModifiers;
typedef struct _JagAstIndexerModifiersPrivate        JagAstIndexerModifiersPrivate;
typedef struct _JagAstIndexerModifiersClass          JagAstIndexerModifiersClass;

typedef enum _JagAstIndexerModifier          JagAstIndexerModifier;


struct _JagAstIndexerModifiers {
	GObject parent;
	JagAstIndexerModifiersPrivate *priv;
};

struct _JagAstIndexerModifiersClass {
	GObjectClass parent_class;
};


enum _JagAstIndexerModifier {
	JAG_AST_INDEXER_MODIFIER_NONE,
	JAG_AST_INDEXER_MODIFIER_PUBLIC,
	JAG_AST_INDEXER_MODIFIER_PROTECTED,
	JAG_AST_INDEXER_MODIFIER_PRIVATE,
	JAG_AST_INDEXER_MODIFIER_STATIC,
	JAG_AST_INDEXER_MODIFIER_ABSTRACT,
	JAG_AST_INDEXER_MODIFIER_FINAL,
	JAG_AST_INDEXER_MODIFIER_NATIVE,
	JAG_AST_INDEXER_MODIFIER_SYNCHRONIZED,
	JAG_AST_INDEXER_MODIFIER_TRANSIENT,
	JAG_AST_INDEXER_MODIFIER_VOLATILE,
	JAG_AST_INDEXER_MODIFIER_STRICTFP
};

GType jag_ast_indexer_modifiers_get_type();

JagAstIndexerModifiers *jag_ast_indexer_modifiers_new(JagAstIndexerModifier initial_modifier);

JagAstIndexerModifiers *jag_ast_indexer_modifiers_include(JagAstIndexerModifiers *modifiers, JagAstIndexerModifiers *modifiers_to_include);

void jag_ast_indexer_modifiers_set(JagAstIndexerModifiers *modifiers, JagAstIndexerModifier modifier);

gboolean jag_ast_indexer_modifiers_is_public(JagAstIndexerModifiers *modifiers);

gboolean jag_ast_indexer_modifiers_is_protected(JagAstIndexerModifiers *modifiers);

gboolean jag_ast_indexer_modifiers_is_private(JagAstIndexerModifiers *modifiers);

gboolean jag_ast_indexer_modifiers_is_static(JagAstIndexerModifiers *modifiers);

gboolean jag_ast_indexer_modifiers_is_abstract(JagAstIndexerModifiers *modifiers);

gboolean jag_ast_indexer_modifiers_is_final(JagAstIndexerModifiers *modifiers);

gboolean jag_ast_indexer_modifiers_is_native(JagAstIndexerModifiers *modifiers);

gboolean jag_ast_indexer_modifiers_is_synchronized(JagAstIndexerModifiers *modifiers);

gboolean jag_ast_indexer_modifiers_is_transient(JagAstIndexerModifiers *modifiers);

gboolean jag_ast_indexer_modifiers_is_volatile(JagAstIndexerModifiers *modifiers);

gboolean jag_ast_indexer_modifiers_is_strictfp(JagAstIndexerModifiers *modifiers);






G_END_DECLS


#endif /* JAGASTINDEXERMODIFIERS_H_ */
