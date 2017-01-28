//
///*
//   File:    grobnfrhs.h
//   Project: gross
//   Author:  Douwe Vos
//   Date:    May 22, 2016
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
//#ifndef PARSER_BNF_GROBNFRHS_H_
//#define PARSER_BNF_GROBNFRHS_H_
//
//#include "grobnfstring.h"
//#include <caterpillar.h>
//
//G_BEGIN_DECLS
//
//#define GRO_TYPE_BNF_RHS              (gro_bnf_rhs_get_type())
//#define GRO_BNF_RHS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_rhs_get_type(), GroBnfRhs))
//#define GRO_BNF_RHS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_RHS, GroBnfRhsClass))
//#define GRO_IS_BNF_RHS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_RHS))
//#define GRO_IS_BNF_RHS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_RHS))
//#define GRO_BNF_RHS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_RHS, GroBnfRhsClass))
//
//
//typedef struct _GroBnfRhs               GroBnfRhs;
//typedef struct _GroBnfRhsPrivate        GroBnfRhsPrivate;
//typedef struct _GroBnfRhsClass          GroBnfRhsClass;
//
//
//struct _GroBnfRhs {
//	GObject parent;
//};
//
//struct _GroBnfRhsClass {
//	GObjectClass parent_class;
//};
//
//
//GType gro_bnf_rhs_get_type();
//
//GroBnfRhs *gro_bnf_rhs_new(CatArrayWo *production_parts);
//
//CatIIterator *gro_bnf_rhs_production_part_iterator(GroBnfRhs *rhs);
//
//GroBnfString *gro_bnf_rhs_get_prec_term_name(GroBnfRhs *rhs);
//
//void gro_bnf_rhs_set_prec_term_name(GroBnfRhs *rhs, GroBnfString *prec_term_name);
//
//G_END_DECLS
//
//#endif /* PARSER_BNF_GROBNFRHS_H_ */
