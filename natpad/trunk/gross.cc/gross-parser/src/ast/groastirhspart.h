/*
   File:    groastirhspart.h
   Project: gross-parser
   Author:  Douwe Vos
   Date:    Nov 28, 2016
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

#ifndef AST_GROASTIRHSPART_H_
#define AST_GROASTIRHSPART_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define GROAST_TYPE_IBNF_RHS_RULE_PART                 (groast_irhs_part_get_type())
#define GROAST_IBNF_RHS_RULE_PART(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), GROAST_TYPE_IBNF_RHS_RULE_PART, GroAstIRhsPart))
#define GROAST_IS_IBNF_RHS_RULE_PART(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), GROAST_TYPE_IBNF_RHS_RULE_PART))
#define GROAST_IBNF_RHS_RULE_PART_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), GROAST_TYPE_IBNF_RHS_RULE_PART, GroAstIRhsPartInterface))


typedef struct _GroAstIRhsPart               GroAstIRhsPart; /* dummy object */
typedef struct _GroAstIRhsPartInterface      GroAstIRhsPartInterface;


struct _GroAstIRhsPartInterface {
	GTypeInterface parent_iface;
};

GType groast_irhs_part_get_type(void);


G_END_DECLS

#endif /* AST_GROASTIRHSPART_H_ */
