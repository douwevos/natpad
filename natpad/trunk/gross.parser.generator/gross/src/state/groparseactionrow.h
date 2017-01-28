
/*
   File:    groparseactionrow.h
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

#ifndef PARSER_STATE_GROPARSEACTIONROW_H_
#define PARSER_STATE_GROPARSEACTIONROW_H_

#include "groiparseaction.h"
#include "../model/gromodel.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_PARSE_ACTION_ROW              (gro_parse_action_row_get_type())
#define GRO_PARSE_ACTION_ROW(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_parse_action_row_get_type(), GroParseActionRow))
#define GRO_PARSE_ACTION_ROW_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_PARSE_ACTION_ROW, GroParseActionRowClass))
#define GRO_IS_PARSE_ACTION_ROW(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_PARSE_ACTION_ROW))
#define GRO_IS_PARSE_ACTION_ROW_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_PARSE_ACTION_ROW))
#define GRO_PARSE_ACTION_ROW_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_PARSE_ACTION_ROW, GroParseActionRowClass))


typedef struct _GroParseActionRow               GroParseActionRow;
typedef struct _GroParseActionRowPrivate        GroParseActionRowPrivate;
typedef struct _GroParseActionRowClass          GroParseActionRowClass;


struct _GroParseActionRow {
	GObject parent;
};

struct _GroParseActionRowClass {
	GObjectClass parent_class;
};


GType gro_parse_action_row_get_type();

GroParseActionRow *gro_parse_action_row_new(GroModel *model);

GroIParseAction *gro_parse_action_row_get_action(GroParseActionRow *action_row, int index);
void gro_parse_action_row_set_action(GroParseActionRow *action_row, int index, GroIParseAction *action);

G_END_DECLS

#endif /* PARSER_STATE_GROPARSEACTIONROW_H_ */
