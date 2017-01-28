/*
   File:    grobnfrhsrule.h
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

#ifndef PARSER_FILE_GROBNFRHSRULE_H_
#define PARSER_FILE_GROBNFRHSRULE_H_

#include "grobnfstring.h"
#include "grobnfrhsrulepartlist.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_BNF_RHS_RULE              (gro_bnf_rhs_rule_get_type())
#define GRO_BNF_RHS_RULE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_rhs_rule_get_type(), GroBnfRhsRule))
#define GRO_BNF_RHS_RULE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_RHS_RULE, GroBnfRhsRuleClass))
#define GRO_IS_BNF_RHS_RULE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_RHS_RULE))
#define GRO_IS_BNF_RHS_RULE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_RHS_RULE))
#define GRO_BNF_RHS_RULE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_RHS_RULE, GroBnfRhsRuleClass))


typedef struct _GroBnfRhsRule               GroBnfRhsRule;
typedef struct _GroBnfRhsRulePrivate        GroBnfRhsRulePrivate;
typedef struct _GroBnfRhsRuleClass          GroBnfRhsRuleClass;


struct _GroBnfRhsRule {
	GObject parent;
};

struct _GroBnfRhsRuleClass {
	GObjectClass parent_class;
};


GType gro_bnf_rhs_rule_get_type();

GroBnfRhsRule *gro_bnf_rhs_rule_new(GroBnfString *lab, GroBnfRhsRulePartList *rr_part_list);

G_END_DECLS

#endif /* PARSER_FILE_GROBNFRHSRULE_H_ */
