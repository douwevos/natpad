
/*
   File:    grolalrstatemap.h
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 2, 2016
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

#ifndef PARSER_STATE_GROLALRSTATEMAP_H_
#define PARSER_STATE_GROLALRSTATEMAP_H_

#include "grolalrstate.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_LALR_STATE_MAP              (gro_lalr_state_map_get_type())
#define GRO_LALR_STATE_MAP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_lalr_state_map_get_type(), GroLalrStateMap))
#define GRO_LALR_STATE_MAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_LALR_STATE_MAP, GroLalrStateMapClass))
#define GRO_IS_LALR_STATE_MAP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_LALR_STATE_MAP))
#define GRO_IS_LALR_STATE_MAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_LALR_STATE_MAP))
#define GRO_LALR_STATE_MAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_LALR_STATE_MAP, GroLalrStateMapClass))


typedef struct _GroLalrStateMap               GroLalrStateMap;
typedef struct _GroLalrStateMapPrivate        GroLalrStateMapPrivate;
typedef struct _GroLalrStateMapClass          GroLalrStateMapClass;


struct _GroLalrStateMap {
	GObject parent;
};

struct _GroLalrStateMapClass {
	GObjectClass parent_class;
};


GType gro_lalr_state_map_get_type();

GroLalrStateMap *gro_lalr_state_map_new();

int gro_lalr_state_map_count(const GroLalrStateMap *state_map);

void gro_lalr_state_map_add(GroLalrStateMap *state_map, GroLalrState *lalr_state);

GroLalrStateIterator *gro_lalr_state_map_iterator(GroLalrStateMap *state_map);

G_END_DECLS

#endif /* PARSER_STATE_GROLALRSTATEMAP_H_ */
