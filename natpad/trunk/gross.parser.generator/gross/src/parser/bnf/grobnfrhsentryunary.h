//
///*
//   File:    grobnfrhsentryunary.h
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
//#ifndef PARSER_BNF_GROBNFRHSENTRYUNARY_H_
//#define PARSER_BNF_GROBNFRHSENTRYUNARY_H_
//
//#include <caterpillar.h>
//
//G_BEGIN_DECLS
//
//
//enum _GroBnfRhsEntryUnaryOpp {
//	GRO_ENTRY_UNARY_STAR,
//	GRO_ENTRY_UNARY_PLUS,
//	GRO_ENTRY_UNARY_EXCL
//};
//
//
//typedef enum _GroBnfRhsEntryUnaryOpp GroBnfRhsEntryUnaryOpp;
//
//#define GRO_TYPE_BNF_RHS_ENTRY_UNARY              (gro_bnf_rhs_entry_unary_get_type())
//#define GRO_BNF_RHS_ENTRY_UNARY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_rhs_entry_unary_get_type(), GroBnfRhsEntryUnary))
//#define GRO_BNF_RHS_ENTRY_UNARY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_RHS_ENTRY_UNARY, GroBnfRhsEntryUnaryClass))
//#define GRO_IS_BNF_RHS_ENTRY_UNARY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_RHS_ENTRY_UNARY))
//#define GRO_IS_BNF_RHS_ENTRY_UNARY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_RHS_ENTRY_UNARY))
//#define GRO_BNF_RHS_ENTRY_UNARY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_RHS_ENTRY_UNARY, GroBnfRhsEntryUnaryClass))
//
//
//typedef struct _GroBnfRhsEntryUnary               GroBnfRhsEntryUnary;
//typedef struct _GroBnfRhsEntryUnaryPrivate        GroBnfRhsEntryUnaryPrivate;
//typedef struct _GroBnfRhsEntryUnaryClass          GroBnfRhsEntryUnaryClass;
//
//
//struct _GroBnfRhsEntryUnary {
//	GObject parent;
//};
//
//struct _GroBnfRhsEntryUnaryClass {
//	GObjectClass parent_class;
//};
//
//
//GType gro_bnf_rhs_entry_unary_get_type();
//
//GroBnfRhsEntryUnary *gro_bnf_rhs_entry_unary_new(GObject *entry, GroBnfRhsEntryUnaryOpp opp);
//
//GroBnfRhsEntryUnaryOpp gro_bnf_rhs_entry_unary_get_operation(GroBnfRhsEntryUnary *entry);
//
//GObject *gro_bnf_rhs_entry_unary_get_entry(GroBnfRhsEntryUnary *entry);
//
//G_END_DECLS
//
//#endif /* PARSER_BNF_GROBNFRHSENTRYUNARY_H_ */
