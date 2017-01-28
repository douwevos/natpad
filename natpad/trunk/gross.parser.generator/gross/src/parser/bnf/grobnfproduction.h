//
///*
//   File:    grobnfproduction.h
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
//#ifndef PARSER_BNF_GROBNFPRODUCTION_H_
//#define PARSER_BNF_GROBNFPRODUCTION_H_
//
//#include "grobnfrhs.h"
//#include "grobnfstring.h"
//#include <caterpillar.h>
//
//G_BEGIN_DECLS
//
//#define GRO_TYPE_BNF_PRODUCTION              (gro_bnf_production_get_type())
//#define GRO_BNF_PRODUCTION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_bnf_production_get_type(), GroBnfProduction))
//#define GRO_BNF_PRODUCTION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_BNF_PRODUCTION, GroBnfProductionClass))
//#define GRO_IS_BNF_PRODUCTION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_BNF_PRODUCTION))
//#define GRO_IS_BNF_PRODUCTION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_BNF_PRODUCTION))
//#define GRO_BNF_PRODUCTION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_BNF_PRODUCTION, GroBnfProductionClass))
//
//
//typedef struct _GroBnfProduction               GroBnfProduction;
//typedef struct _GroBnfProductionPrivate        GroBnfProductionPrivate;
//typedef struct _GroBnfProductionClass          GroBnfProductionClass;
//
//
//struct _GroBnfProduction {
//	GObject parent;
//};
//
//struct _GroBnfProductionClass {
//	GObjectClass parent_class;
//};
//
//
//GType gro_bnf_production_get_type();
//
//GroBnfProduction *gro_bnf_production_new(GroBnfString *lhs_id, GroBnfRhs *rhs);
//
//
//GroBnfString *gro_bnf_production_get_lhs_string(GroBnfProduction *bnf_prod);
//GroBnfRhs *gro_bnf_production_get_rhs(GroBnfProduction *bnf_prod);
//
//G_END_DECLS
//
//#endif /* PARSER_BNF_GROBNFPRODUCTION_H_ */
