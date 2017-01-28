/*
   File:    grobnfrhsrulelist.h
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

#ifndef PARSER_FILE_GROBNFRHSRULELIST_H_
#define PARSER_FILE_GROBNFRHSRULELIST_H_

#include "grobnfrhsrule.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_BNF_RHS_RULE_LIST              (gro_bnf_rhs_rule_list_get_type())
#define GRO_BNF_RHS_RULE_LIST(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_rhs_rule_list_get_type(), GroBnfRhsRuleList))
#define GRO_BNF_RHS_RULE_LIST_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_RHS_RULE_LIST, GroBnfRhsRuleListClass))
#define GRO_IS_BNF_RHS_RULE_LIST(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_RHS_RULE_LIST))
#define GRO_IS_BNF_RHS_RULE_LIST_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_RHS_RULE_LIST))
#define GRO_BNF_RHS_RULE_LIST_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_RHS_RULE_LIST, GroBnfRhsRuleListClass))


typedef struct _GroBnfRhsRuleList               GroBnfRhsRuleList;
typedef struct _GroBnfRhsRuleListPrivate        GroBnfRhsRuleListPrivate;
typedef struct _GroBnfRhsRuleListClass          GroBnfRhsRuleListClass;


struct _GroBnfRhsRuleList {
	GObject parent;
};

struct _GroBnfRhsRuleListClass {
	GObjectClass parent_class;
};


GType gro_bnf_rhs_rule_list_get_type();

GroBnfRhsRuleList *gro_bnf_rhs_rule_list_new(GroBnfRhsRuleList *linked, GroBnfRhsRule *rule);

G_END_DECLS

#endif /* PARSER_FILE_GROBNFRHSRULELIST_H_ */
