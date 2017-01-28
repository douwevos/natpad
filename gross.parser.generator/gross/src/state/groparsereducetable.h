
/*
   File:    groparsereducetable.h
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 5, 2016
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

#ifndef PARSER_STATE_GROPARSEREDUCETABLE_H_
#define PARSER_STATE_GROPARSEREDUCETABLE_H_

#include "groparsereducerow.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_PARSE_REDUCE_TABLE              (gro_parse_reduce_table_get_type())
#define GRO_PARSE_REDUCE_TABLE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_parse_reduce_table_get_type(), GroParseReduceTable))
#define GRO_PARSE_REDUCE_TABLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_PARSE_REDUCE_TABLE, GroParseReduceTableClass))
#define GRO_IS_PARSE_REDUCE_TABLE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_PARSE_REDUCE_TABLE))
#define GRO_IS_PARSE_REDUCE_TABLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_PARSE_REDUCE_TABLE))
#define GRO_PARSE_REDUCE_TABLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_PARSE_REDUCE_TABLE, GroParseReduceTableClass))


typedef struct _GroParseReduceTable               GroParseReduceTable;
typedef struct _GroParseReduceTablePrivate        GroParseReduceTablePrivate;
typedef struct _GroParseReduceTableClass          GroParseReduceTableClass;


struct _GroParseReduceTable {
	GObject parent;
};

struct _GroParseReduceTableClass {
	GObjectClass parent_class;
};


GType gro_parse_reduce_table_get_type();

GroParseReduceTable *gro_parse_reduce_table_new(GroModel *model, int state_count);

GroParseReduceRow *gro_parse_reduce_table_get_row(GroParseReduceTable *reduce_table, int index);

int gro_parse_reduce_table_row_count(GroParseReduceTable *reduce_table);

G_END_DECLS

#endif /* PARSER_STATE_GROPARSEREDUCETABLE_H_ */
