/*
   File:    jagpidiagnosticposition.c
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Jul 3, 2017
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2017 Douwe Vos.

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

#include "jagpidiagnosticposition.h"

G_DEFINE_INTERFACE(JagPIDiagnosticPosition, jagp_idiagnostic_position, G_TYPE_OBJECT);

static void jagp_idiagnostic_position_default_init (JagPIDiagnosticPositionInterface *iface) {
}

JagPCursor *jagp_idiagnostic_position_get_preferred(JagPIDiagnosticPosition *position) {
	return JAGP_IDIAGNOSTIC_POSITION_GET_INTERFACE(position)->getPreferredCursor(position);
}

JagPCursor *jagp_idiagnostic_position_get_start(JagPIDiagnosticPosition *position) {
	return JAGP_IDIAGNOSTIC_POSITION_GET_INTERFACE(position)->getStartCursor(position);
}

JagPCursor *jagp_idiagnostic_position_get_end(JagPIDiagnosticPosition *position) {
	return JAGP_IDIAGNOSTIC_POSITION_GET_INTERFACE(position)->getEndCursor(position);
}
