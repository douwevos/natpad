/*
   File:    grobnfrhsunarypart.h
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

#ifndef PARSER_FILE_GROBNFRHSUNARYPART_H_
#define PARSER_FILE_GROBNFRHSUNARYPART_H_

#include "groibnfrhsrulepart.h"
#include <caterpillar.h>

G_BEGIN_DECLS

typedef enum _GroBnfRhsUnaryOp GroBnfRhsUnaryOp;

enum _GroBnfRhsUnaryOp {
	GRO_UNARY_OP_ZERO_OR_MORE,
	GRO_UNARY_OP_ONE_OR_MORE,
	GRO_UNARY_OP_OPTIONAL
};


#define GRO_TYPE_BNF_RHS_UNARY_PART              (gro_bnf_rhs_unary_part_get_type())
#define GRO_BNF_RHS_UNARY_PART(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_rhs_unary_part_get_type(), GroBnfRhsUnaryPart))
#define GRO_BNF_RHS_UNARY_PART_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_RHS_UNARY_PART, GroBnfRhsUnaryPartClass))
#define GRO_IS_BNF_RHS_UNARY_PART(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_RHS_UNARY_PART))
#define GRO_IS_BNF_RHS_UNARY_PART_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_RHS_UNARY_PART))
#define GRO_BNF_RHS_UNARY_PART_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_RHS_UNARY_PART, GroBnfRhsUnaryPartClass))


typedef struct _GroBnfRhsUnaryPart               GroBnfRhsUnaryPart;
typedef struct _GroBnfRhsUnaryPartPrivate        GroBnfRhsUnaryPartPrivate;
typedef struct _GroBnfRhsUnaryPartClass          GroBnfRhsUnaryPartClass;


struct _GroBnfRhsUnaryPart {
	GObject parent;
};

struct _GroBnfRhsUnaryPartClass {
	GObjectClass parent_class;
};


GType gro_bnf_rhs_unary_part_get_type();

GroBnfRhsUnaryPart *gro_bnf_rhs_unary_part_new(GroIBnfRhsRulePart *rr_part, GroBnfRhsUnaryOp op);

GroIBnfRhsRulePart *gro_bnf_rhs_unary_part_get_part(GroBnfRhsUnaryPart *un_part);
GroBnfRhsUnaryOp gro_bnf_rhs_unary_part_get_operator(GroBnfRhsUnaryPart *un_part);

G_END_DECLS

#endif /* PARSER_FILE_GROBNFRHSUNARYPART_H_ */
