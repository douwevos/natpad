//
///*
//   File:    grobnfrhsactioncode.h
//   Project: gross
//   Author:  Douwe Vos
//   Date:    Jul 12, 2016
//   e-mail:  dmvos2000(at)yahoo.com
//
//   Copyright (C) 2016 Douwe Vos.
//
//   This program is free software: you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 3 of the License, or
//   (at your option) any later version.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public License
//   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */
//
//#ifndef PARSER_BNF_GROBNFRHSACTIONCODE_H_
//#define PARSER_BNF_GROBNFRHSACTIONCODE_H_
//
//#include "grobnfstring.h"
//#include <caterpillar.h>
//
//G_BEGIN_DECLS
//
//#define GRO_TYPE_BNF_RHS_ACTION_CODE              (gro_bnf_rhs_action_code_get_type())
//#define GRO_BNF_RHS_ACTION_CODE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_rhs_action_code_get_type(), GroBnfRhsActionCode))
//#define GRO_BNF_RHS_ACTION_CODE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_RHS_ACTION_CODE, GroBnfRhsActionCodeClass))
//#define GRO_IS_BNF_RHS_ACTION_CODE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_RHS_ACTION_CODE))
//#define GRO_IS_BNF_RHS_ACTION_CODE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_RHS_ACTION_CODE))
//#define GRO_BNF_RHS_ACTION_CODE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_RHS_ACTION_CODE, GroBnfRhsActionCodeClass))
//
//
//typedef struct _GroBnfRhsActionCode               GroBnfRhsActionCode;
//typedef struct _GroBnfRhsActionCodePrivate        GroBnfRhsActionCodePrivate;
//typedef struct _GroBnfRhsActionCodeClass          GroBnfRhsActionCodeClass;
//
//
//struct _GroBnfRhsActionCode {
//	GObject parent;
//};
//
//struct _GroBnfRhsActionCodeClass {
//	GObjectClass parent_class;
//};
//
//
//GType gro_bnf_rhs_action_code_get_type();
//
//GroBnfRhsActionCode *gro_bnf_rhs_action_code_new(GroBnfString *code);
//
//GroBnfString *gro_bnf_rhs_action_code_get_code(GroBnfRhsActionCode *action_code);
//
//G_END_DECLS
//
//#endif /* PARSER_BNF_GROBNFRHSACTIONCODE_H_ */
