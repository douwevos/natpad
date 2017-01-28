
/*
   File:    gromodel.h
   Project: gross
   Author:  Douwe Vos
   Date:    May 25, 2016
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

#ifndef PARSER_MODEL_GROMODEL_H_
#define PARSER_MODEL_GROMODEL_H_

#include "gromterminal.h"
#include "gromnonterminal.h"
#include "gromproduction.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_MODEL              (gro_model_get_type())
#define GRO_MODEL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_model_get_type(), GroModel))
#define GRO_MODEL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_MODEL, GroModelClass))
#define GRO_IS_MODEL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_MODEL))
#define GRO_IS_MODEL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_MODEL))
#define GRO_MODEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_MODEL, GroModelClass))


typedef struct _GroModel               GroModel;
typedef struct _GroModelPrivate        GroModelPrivate;
typedef struct _GroModelClass          GroModelClass;


struct _GroModel {
	GObject parent;
};

struct _GroModelClass {
	GObjectClass parent_class;
};


GType gro_model_get_type();

GroModel *gro_model_new();

void gro_model_compute_nullability(GroModel *model);
void gro_model_compute_first_sets(GroModel *model);


GroMTerminal *gro_model_add_terminal(GroModel *model, GroBnfString *name, CatStringWo *stack_type, gboolean *created_new);


GroMSymbol *gro_model_get_symbol_by_name(GroModel *model, CatStringWo *name);

GroMTerminal *gro_model_get_terminal_eof(GroModel *model);
GroMTerminal *gro_model_get_terminal_error(GroModel *model);
GroMTerminal *gro_model_get_terminal_by_index(GroModel *model, int index);
GroMTerminal *gro_model_get_terminal_by_name(GroModel *model, CatStringWo *name);
int gro_model_terminal_count(GroModel *model);
GroMTerminalIterator *gro_model_terminal_iterator(GroModel *model);
CatArrayWo *gro_model_enlist_terminals(GroModel *model);
void gro_model_dump_terminals(GroModel *model);


GroMNonTerminal *gro_model_get_non_terminal_dollar_start(GroModel *model);
GroMNonTerminal *gro_model_get_non_terminal_by_name(GroModel *model, CatStringWo *name);
int gro_model_non_terminal_count(GroModel *model);
CatArrayWo *gro_model_enlist_non_terminals(GroModel *model);
GroMNonTerminal *gro_model_add_non_terminal(GroModel *model, GroBnfString *name, CatStringWo *stack_type);
void gro_model_dump_non_terminals(GroModel *model);



GroMProduction *gro_model_add_production(GroModel *model, GroMNonTerminal *lhs_non_term, CatArrayWo *production_parts, int prec_num, int prec_side);
GroMNonTerminal *gro_model_create_sub_non_terminal(GroModel *model);
GroMNonTerminal *gro_model_add_sub_productions(GroModel *model, CatArrayWo *sub_prod_parts_list);


GroMProduction *gro_model_get_start_production(GroModel *model);
CatIIterator *gro_model_production_iterator(GroModel *model);
int gro_model_production_count(GroModel *model);

G_END_DECLS

#endif /* PARSER_MODEL_GROMODEL_H_ */
