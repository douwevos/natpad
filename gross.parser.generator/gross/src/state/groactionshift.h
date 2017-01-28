
/*
   File:    groactionshift.h
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 6, 2016
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

#ifndef PARSER_STATE_GROACTIONSHIFT_H_
#define PARSER_STATE_GROACTIONSHIFT_H_

#include "grolalrstate.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_ACTION_SHIFT              (gro_action_shift_get_type())
#define GRO_ACTION_SHIFT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_action_shift_get_type(), GroActionShift))
#define GRO_ACTION_SHIFT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_ACTION_SHIFT, GroActionShiftClass))
#define GRO_IS_ACTION_SHIFT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_ACTION_SHIFT))
#define GRO_IS_ACTION_SHIFT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_ACTION_SHIFT))
#define GRO_ACTION_SHIFT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_ACTION_SHIFT, GroActionShiftClass))


typedef struct _GroActionShift               GroActionShift;
typedef struct _GroActionShiftPrivate        GroActionShiftPrivate;
typedef struct _GroActionShiftClass          GroActionShiftClass;


struct _GroActionShift {
	GObject parent;
};

struct _GroActionShiftClass {
	GObjectClass parent_class;
};


GType gro_action_shift_get_type();

GroActionShift *gro_action_shift_new(GroLalrState *shift_to);

GroLalrState *gro_action_shift_get_shift_to(GroActionShift *action);

G_END_DECLS

#endif /* PARSER_STATE_GROACTIONSHIFT_H_ */
