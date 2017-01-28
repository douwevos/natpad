/*
   File:    leaoutputactionreset.h
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Dec 1, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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

#ifndef LEAOUTPUTACTIONRESET_H_
#define LEAOUTPUTACTIONRESET_H_

#include "../../src/leafhopper.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define LEA_TYPE_OUTPUT_ACTION_RESET              (lea_output_action_reset_get_type())
#define LEA_OUTPUT_ACTION_RESET(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), lea_output_action_reset_get_type(), LeaOutputActionReset))
#define LEA_OUTPUT_ACTION_RESET_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), LEA_TYPE_OUTPUT_ACTION_RESET, LeaOutputActionResetClass))
#define LEA_IS_OUTPUT_ACTION_RESET(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), LEA_TYPE_OUTPUT_ACTION_RESET))
#define LEA_IS_OUTPUT_ACTION_RESET_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), LEA_TYPE_OUTPUT_ACTION_RESET))
#define LEA_OUTPUT_ACTION_RESET_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), LEA_TYPE_OUTPUT_ACTION_RESET, LeaOutputActionResetClass))
#define LEA_OUTPUT_ACTION_RESET_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), LEA_TYPE_OUTPUT_ACTION_RESET, LeaOutputActionResetPrivate))


typedef struct _LeaOutputActionReset               LeaOutputActionReset;
typedef struct _LeaOutputActionResetPrivate        LeaOutputActionResetPrivate;
typedef struct _LeaOutputActionResetClass          LeaOutputActionResetClass;


struct _LeaOutputActionReset {
	LeaAction parent;
	LeaOutputActionResetPrivate *priv;
};

struct _LeaOutputActionResetClass {
	LeaActionClass parent_class;
};


GType lea_output_action_reset_get_type();

LeaOutputActionReset *lea_output_action_reset_new(LeaIPanelOwner *owner);

G_END_DECLS

#endif /* LEAOUTPUTACTIONRESET_H_ */
