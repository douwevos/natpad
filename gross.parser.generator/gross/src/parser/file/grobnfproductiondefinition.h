/*
   File:    grobnfproductiondefinition.h
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

#ifndef PARSER_FILE_GROBNFPRODUCTIONDEFINITION_H_
#define PARSER_FILE_GROBNFPRODUCTIONDEFINITION_H_

#include "grobnfstring.h"
#include "grobnfrhsrulelist.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_BNF_PRODUCTION_DEFINITION              (gro_bnf_production_definition_get_type())
#define GRO_BNF_PRODUCTION_DEFINITION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_production_definition_get_type(), GroBnfProductionDefinition))
#define GRO_BNF_PRODUCTION_DEFINITION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_PRODUCTION_DEFINITION, GroBnfProductionDefinitionClass))
#define GRO_IS_BNF_PRODUCTION_DEFINITION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_PRODUCTION_DEFINITION))
#define GRO_IS_BNF_PRODUCTION_DEFINITION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_PRODUCTION_DEFINITION))
#define GRO_BNF_PRODUCTION_DEFINITION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_PRODUCTION_DEFINITION, GroBnfProductionDefinitionClass))


typedef struct _GroBnfProductionDefinition               GroBnfProductionDefinition;
typedef struct _GroBnfProductionDefinitionPrivate        GroBnfProductionDefinitionPrivate;
typedef struct _GroBnfProductionDefinitionClass          GroBnfProductionDefinitionClass;


struct _GroBnfProductionDefinition {
	GObject parent;
};

struct _GroBnfProductionDefinitionClass {
	GObjectClass parent_class;
};


GType gro_bnf_production_definition_get_type();

GroBnfProductionDefinition *gro_bnf_production_definition_new(GroBnfString *lhs_id, GroBnfRhsRuleList *rhs_rule_list);

GroBnfString *gro_bnf_production_definition_get_lhs_id(GroBnfProductionDefinition *definition);
GroBnfRhsRuleList *gro_bnf_production_definition_get_rhs_rule_list(GroBnfProductionDefinition *definition);

G_END_DECLS

#endif /* PARSER_FILE_GROBNFPRODUCTIONDEFINITION_H_ */
