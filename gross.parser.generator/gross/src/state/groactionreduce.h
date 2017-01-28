
/*
   File:    groreduceaction.h
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

#ifndef PARSER_STATE_GROACTIONREDUCE_H_
#define PARSER_STATE_GROACTIONREDUCE_H_

#include "../model/gromproduction.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_ACTION_REDUCE              (gro_action_reduce_get_type())
#define GRO_ACTION_REDUCE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_action_reduce_get_type(), GroActionReduce))
#define GRO_ACTION_REDUCE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_ACTION_REDUCE, GroActionReduceClass))
#define GRO_IS_ACTION_REDUCE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_ACTION_REDUCE))
#define GRO_IS_ACTION_REDUCE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_ACTION_REDUCE))
#define GRO_ACTION_REDUCE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_ACTION_REDUCE, GroActionReduceClass))


typedef struct _GroActionReduce               GroActionReduce;
typedef struct _GroActionReducePrivate        GroActionReducePrivate;
typedef struct _GroActionReduceClass          GroActionReduceClass;


struct _GroActionReduce {
	GObject parent;
};

struct _GroActionReduceClass {
	GObjectClass parent_class;
};


GType gro_action_reduce_get_type();

GroActionReduce *gro_action_reduce_new(GroMProduction *production);

GroMProduction *gro_action_reduce_get_production(GroActionReduce *action_reduce);

G_END_DECLS

#endif /* PARSER_STATE_GROACTIONREDUCE_H_ */
