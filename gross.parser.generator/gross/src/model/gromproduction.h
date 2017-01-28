
/*
   File:    gromproduction.h
   Project: gross
   Author:  Douwe Vos
   Date:    May 26, 2016
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

#ifndef PARSER_MODEL_GROMPRODUCTION_H_
#define PARSER_MODEL_GROMPRODUCTION_H_

#include "gromnonterminal.h"
#include "gromsymbol.h"
#include "groterminalset.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_M_PRODUCTION              (gro_m_production_get_type())
#define GRO_M_PRODUCTION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_m_production_get_type(), GroMProduction))
#define GRO_M_PRODUCTION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_M_PRODUCTION, GroMProductionClass))
#define GRO_IS_M_PRODUCTION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_M_PRODUCTION))
#define GRO_IS_M_PRODUCTION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_M_PRODUCTION))
#define GRO_M_PRODUCTION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_M_PRODUCTION, GroMProductionClass))

#define GroMProdPart GObject

typedef struct _GroMProduction               GroMProduction;
typedef struct _GroMProductionPrivate        GroMProductionPrivate;
typedef struct _GroMProductionClass          GroMProductionClass;


struct _GroMProduction {
	GObject parent;
};

struct _GroMProductionClass {
	GObjectClass parent_class;
};


GType gro_m_production_get_type();

GroMProduction *gro_m_production_new(int index, GroMNonTerminal *lhs_sym, CatArrayWo *rhs_parts, int prec_num, int prec_side);

int gro_m_production_get_index(const GroMProduction *prod);
GroMSymbol *gro_m_production_get_lhs_symbol(GroMProduction *production);
int gro_m_production_get_precedence_num(GroMProduction *production);
CatStringWo *gro_m_production_get_action(GroMProduction *production);

int gro_m_production_rhs_count(const GroMProduction *prod);
GroMProdPart *gro_m_production_rhs_at(GroMProduction *prod, int index);

gboolean gro_m_production_check_nullable(GroMProduction *prod);
GroTerminalSet *gro_m_production_check_first_set(GroMProduction *prod);

gboolean gro_m_production_equal(const GroMProduction *prod_a, const GroMProduction *prod_b);

int gro_m_production_hash(GroMProduction *prod);

CatStringWo *gro_m_production_to_simple_string(GroMProduction *prod);

G_END_DECLS

#endif /* PARSER_MODEL_GROMPRODUCTION_H_ */
