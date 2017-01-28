/*
   File:    groibnfrhsrulepart.h
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

#ifndef PARSER_FILE_GROIBNFRHSRULEPART_H_
#define PARSER_FILE_GROIBNFRHSRULEPART_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_IBNF_RHS_RULE_PART                 (gro_ibnf_rhs_rule_part_get_type())
#define GRO_IBNF_RHS_RULE_PART(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), GRO_TYPE_IBNF_RHS_RULE_PART, GroIBnfRhsRulePart))
#define GRO_IS_IBNF_RHS_RULE_PART(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), GRO_TYPE_IBNF_RHS_RULE_PART))
#define GRO_IBNF_RHS_RULE_PART_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), GRO_TYPE_IBNF_RHS_RULE_PART, GroIBnfRhsRulePartInterface))


typedef struct _GroIBnfRhsRulePart               GroIBnfRhsRulePart; /* dummy object */
typedef struct _GroIBnfRhsRulePartInterface      GroIBnfRhsRulePartInterface;


struct _GroIBnfRhsRulePartInterface {
	GTypeInterface parent_iface;
};

GType gro_ibnf_rhs_rule_part_get_type(void);


G_END_DECLS

#endif /* PARSER_FILE_GROIBNFRHSRULEPART_H_ */
