/*
   File:    grobnftypedefinition.h
   Project: gross
   Author:  Douwe Vos
   Date:    Nov 17, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef PARSER_FILE_GROBNFTYPEDEFINITION_H_
#define PARSER_FILE_GROBNFTYPEDEFINITION_H_

#include "grobnfstring.h"
#include "grobnfidlist.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_BNF_TYPE_DEFINITION              (gro_bnf_type_definition_get_type())
#define GRO_BNF_TYPE_DEFINITION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_type_definition_get_type(), GroBnfTypeDefinition))
#define GRO_BNF_TYPE_DEFINITION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_TYPE_DEFINITION, GroBnfTypeDefinitionClass))
#define GRO_IS_BNF_TYPE_DEFINITION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_TYPE_DEFINITION))
#define GRO_IS_BNF_TYPE_DEFINITION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_TYPE_DEFINITION))
#define GRO_BNF_TYPE_DEFINITION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_TYPE_DEFINITION, GroBnfTypeDefinitionClass))


typedef struct _GroBnfTypeDefinition               GroBnfTypeDefinition;
typedef struct _GroBnfTypeDefinitionPrivate        GroBnfTypeDefinitionPrivate;
typedef struct _GroBnfTypeDefinitionClass          GroBnfTypeDefinitionClass;


struct _GroBnfTypeDefinition {
	GObject parent;
};

struct _GroBnfTypeDefinitionClass {
	GObjectClass parent_class;
};


GType gro_bnf_type_definition_get_type();

GroBnfTypeDefinition *gro_bnf_type_definition_new(GroBnfString *sym_type, GroBnfIdList *id_list);

GroBnfString *gro_bnf_type_definition_get_symbol_type(GroBnfTypeDefinition *type_def);
GroBnfIdList *gro_bnf_type_definition_get_id_list(GroBnfTypeDefinition *type_def);

G_END_DECLS

#endif /* PARSER_FILE_GROBNFTYPEDEFINITION_H_ */
