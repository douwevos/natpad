/*
   File:    jagdecvaluetypedefinition.h
   Project: jaguar-decompiler
   Author:  Douwe Vos
   Date:    Nov 6, 2012
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


#ifndef JAGDECVALUETYPEDEFINITION_H_
#define JAGDECVALUETYPEDEFINITION_H_

#include <caterpillar.h>
#include <jaguarbytecode.h>

G_BEGIN_DECLS

enum _JagDecValueTypeDefinitionKind {
	JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CATCHED_EXCEPTION,
	JAG_DEC_VALUE_TYPE_DEFINITION_KIND_ARGUMENT,
	JAG_DEC_VALUE_TYPE_DEFINITION_KIND_LOCALVALUETABLE,
	JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONDITION,
	JAG_DEC_VALUE_TYPE_DEFINITION_KIND_NULL,
	JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CONSTANT,
	JAG_DEC_VALUE_TYPE_DEFINITION_KIND_NEW,
	JAG_DEC_VALUE_TYPE_DEFINITION_KIND_SUPER,
	JAG_DEC_VALUE_TYPE_DEFINITION_KIND_THIS,
	JAG_DEC_VALUE_TYPE_DEFINITION_KIND_RETURN,
	JAG_DEC_VALUE_TYPE_DEFINITION_KIND_CAST,
	JAG_DEC_VALUE_TYPE_DEFINITION_KIND_INSTANCEOF
};

typedef enum _JagDecValueTypeDefinitionKind JagDecValueTypeDefinitionKind;

#define JAG_DEC_TYPE_VALUE_TYPE_DEFINITION              (jag_dec_value_type_definition_get_type())
#define JAG_DEC_VALUE_TYPE_DEFINITION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_dec_value_type_definition_get_type(), JagDecValueTypeDefinition))
#define JAG_DEC_VALUE_TYPE_DEFINITION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_DEC_TYPE_VALUE_TYPE_DEFINITION, JagDecValueTypeDefinitionClass))
#define JAG_DEC_IS_VALUE_TYPE_DEFINITION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_DEC_TYPE_VALUE_TYPE_DEFINITION))
#define JAG_DEC_IS_VALUE_TYPE_DEFINITION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_DEC_TYPE_VALUE_TYPE_DEFINITION))
#define JAG_DEC_VALUE_TYPE_DEFINITION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_DEC_TYPE_VALUE_TYPE_DEFINITION, JagDecValueTypeDefinitionClass))
#define JAG_DEC_VALUE_TYPE_DEFINITION_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_DEC_TYPE_VALUE_TYPE_DEFINITION, JagDecValueTypeDefinitionPrivate))

typedef struct _JagDecValueTypeDefinition               JagDecValueTypeDefinition;
typedef struct _JagDecValueTypeDefinitionPrivate        JagDecValueTypeDefinitionPrivate;
typedef struct _JagDecValueTypeDefinitionClass          JagDecValueTypeDefinitionClass;


struct _JagDecValueTypeDefinition {
	GObject parent;
	JagDecValueTypeDefinitionPrivate *priv;
};

struct _JagDecValueTypeDefinitionClass {
	GObjectClass parent_class;
};


GType jag_dec_value_type_definition_get_type();

JagDecValueTypeDefinition *jag_dec_value_type_definition_new(JagAstDeclarationType *typeDefinition, JagDecValueTypeDefinitionKind kind);

JagDecValueTypeDefinition *jag_dec_value_type_definition_clone(JagDecValueTypeDefinition *source);

JagDecValueTypeDefinitionKind jag_dec_value_type_definition_get_kind(JagDecValueTypeDefinition *definition);

gboolean jag_dec_value_type_definition_is_included(JagDecValueTypeDefinition *definition);

void jag_dec_value_type_definition_set_included(JagDecValueTypeDefinition *definition, gboolean included_val);

JagAstDeclarationType *jag_dec_value_type_definition_get_declaration_type(JagDecValueTypeDefinition *definition);


int jag_dec_value_type_definition_hash(JagDecValueTypeDefinition *definition);

gboolean jag_dec_value_type_definition_equal(JagDecValueTypeDefinition *definition_a, JagDecValueTypeDefinition *definition_b);



int jag_dec_value_type_definition_kind_get_prio(JagDecValueTypeDefinitionKind kind);

G_END_DECLS


#endif /* JAGDECVALUETYPEDEFINITION_H_ */
