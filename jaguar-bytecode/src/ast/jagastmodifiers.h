/*
   File:    jagastmodifiers.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 31, 2012
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


#ifndef JAGASTMODIFIERS_H_
#define JAGASTMODIFIERS_H_

#include <caterpillar.h>

G_BEGIN_DECLS

enum _JagAstModifier {
	JAG_AST_MODIFIER_PUBLIC = 0x0001,
	JAG_AST_MODIFIER_PRIVATE = 0x0002,
	JAG_AST_MODIFIER_PROTECTED = 0x0004,
	JAG_AST_MODIFIER_STATIC = 0x0008,
	JAG_AST_MODIFIER_FINAL = 0x0010,
	JAG_AST_MODIFIER_SYNCHRONIZED = 0x0020,
	JAG_AST_MODIFIER_VOLATILE = 0x0040,
	JAG_AST_MODIFIER_TRANSIENT = 0x0080,
	JAG_AST_MODIFIER_NATIVE = 0x100,
	JAG_AST_MODIFIER_INTERFACE = 0x200
};

typedef enum _JagAstModifier JagAstModifier;

#define JAG_AST_TYPE_MODIFIERS              (jag_ast_modifiers_get_type())
#define JAG_AST_MODIFIERS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_ast_modifiers_get_type(), JagAstModifiers))
#define JAG_AST_MODIFIERS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_AST_TYPE_MODIFIERS, JagAstModifiersClass))
#define JAG_AST_IS_MODIFIERS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_AST_TYPE_MODIFIERS))
#define JAG_AST_IS_MODIFIERS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_AST_TYPE_MODIFIERS))
#define JAG_AST_MODIFIERS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_AST_TYPE_MODIFIERS, JagAstModifiersClass))
#define JAG_AST_MODIFIERS_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_AST_TYPE_MODIFIERS, JagAstModifiersPrivate))

typedef struct _JagAstModifiers               JagAstModifiers;
typedef struct _JagAstModifiersPrivate        JagAstModifiersPrivate;
typedef struct _JagAstModifiersClass          JagAstModifiersClass;


struct _JagAstModifiers {
	GObject parent;
	JagAstModifiersPrivate *priv;
};

struct _JagAstModifiersClass {
	GObjectClass parent_class;
};


GType jag_ast_modifiers_get_type();

JagAstModifiers *jag_ast_modifiers_new(short modifiers);


CatStringWo *jag_ast_modifiers_as_text(JagAstModifiers *modifiers);

gboolean jag_ast_modifiers_is_static(JagAstModifiers *modifiers);

gboolean jag_ast_modifiers_equal(JagAstModifiers *modifiers_a, JagAstModifiers *modifiers_b);


const char *jag_ast_modifier_as_text(JagAstModifier modifier);

G_END_DECLS


#endif /* JAGASTMODIFIERS_H_ */
