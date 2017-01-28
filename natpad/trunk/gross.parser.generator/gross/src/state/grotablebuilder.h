
/*
   File:    grotablebuilder.h
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 23, 2016
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

#ifndef STATE_GROTABLEBUILDER_H_
#define STATE_GROTABLEBUILDER_H_

#include "groparseactiontable.h"
#include "groparsereducetable.h"
#include "../model/gromodel.h"
#include <caterpillar.h>

G_BEGIN_DECLS


enum _GroTypeOfParser {
	/* Automatically select the type of the generated parser */
	GRO_PARSER_TYPE_AUTO,
	/* Force the generated parser to be of the type LRAR */
	GRO_PARSER_TYPE_LRAR,
	/* Force the generated parser to be of the type GLR */
	GRO_PARSER_TYPE_GLR
};

typedef enum _GroTypeOfParser GroTypeOfParser;

#define GRO_TYPE_TABLE_BUILDER              (gro_table_builder_get_type())
#define GRO_TABLE_BUILDER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_table_builder_get_type(), GroTableBuilder))
#define GRO_TABLE_BUILDER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_TABLE_BUILDER, GroTableBuilderClass))
#define GRO_IS_TABLE_BUILDER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_TABLE_BUILDER))
#define GRO_IS_TABLE_BUILDER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_TABLE_BUILDER))
#define GRO_TABLE_BUILDER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_TABLE_BUILDER, GroTableBuilderClass))


typedef struct _GroTableBuilder               GroTableBuilder;
typedef struct _GroTableBuilderPrivate        GroTableBuilderPrivate;
typedef struct _GroTableBuilderClass          GroTableBuilderClass;


struct _GroTableBuilder {
	GObject parent;
};

struct _GroTableBuilderClass {
	GObjectClass parent_class;
};


GType gro_table_builder_get_type();

GroTableBuilder *gro_table_builder_new(GroModel *model, GroTypeOfParser top);

void gro_table_builder_configure(GroTableBuilder *table_builder, int state_count);

void gro_table_builder_create_for_state(GroTableBuilder *table_builder, GroLalrState *state);

GroParseActionTable *gro_table_builder_get_action_table(GroTableBuilder *table_builder);
GroParseReduceTable *gro_table_builder_get_reduce_table(GroTableBuilder *table_builder);

G_END_DECLS

#endif /* STATE_GROTABLEBUILDER_H_ */
