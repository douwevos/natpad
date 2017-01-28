/*
   File:    leaoutputactionclear.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Nov 15, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef LEAOUTPUTACTIONCLEAR_H_
#define LEAOUTPUTACTIONCLEAR_H_

#include "../../src/leafhopper.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_OUTPUT_ACTION_CLEAR              (lea_output_action_clear_get_type())
#define LEA_OUTPUT_ACTION_CLEAR(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_output_action_clear_get_type(), LeaOutputActionClear))
#define LEA_OUTPUT_ACTION_CLEAR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_OUTPUT_ACTION_CLEAR, LeaOutputActionClearClass))
#define LEA_IS_OUTPUT_ACTION_CLEAR(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_OUTPUT_ACTION_CLEAR))
#define LEA_IS_OUTPUT_ACTION_CLEAR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_OUTPUT_ACTION_CLEAR))
#define LEA_OUTPUT_ACTION_CLEAR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_OUTPUT_ACTION_CLEAR, LeaOutputActionClearClass))


typedef struct _LeaOutputActionClear               LeaOutputActionClear;
typedef struct _LeaOutputActionClearPrivate        LeaOutputActionClearPrivate;
typedef struct _LeaOutputActionClearClass          LeaOutputActionClearClass;


struct _LeaOutputActionClear {
	LeaAction parent;
};

struct _LeaOutputActionClearClass {
	LeaActionClass parent_class;
};


GType lea_output_action_clear_get_type();

LeaOutputActionClear *lea_output_action_clear_new();

G_END_DECLS


#endif /* LEAOUTPUTACTIONCLEAR_H_ */
