/*
   File:    jagpidiagnosticposition.h
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

#ifndef SRC_JAGPIDIAGNOSTICPOSITION_H_
#define SRC_JAGPIDIAGNOSTICPOSITION_H_

#include "jagpcursor.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_IDIAGNOSTIC_POSITION                 (jagp_idiagnostic_position_get_type())
#define JAGP_IDIAGNOSTIC_POSITION(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAGP_TYPE_IDIAGNOSTIC_POSITION, JagPIDiagnosticPosition))
#define JAGP_IS_IDIAGNOSTIC_POSITION(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAGP_TYPE_IDIAGNOSTIC_POSITION))
#define JAGP_IDIAGNOSTIC_POSITION_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAGP_TYPE_IDIAGNOSTIC_POSITION, JagPIDiagnosticPositionInterface))

typedef struct _JagPIDiagnosticPosition               JagPIDiagnosticPosition; /* dummy object */
typedef struct _JagPIDiagnosticPositionInterface      JagPIDiagnosticPositionInterface;

struct _JagPJCTree;

struct _JagPIDiagnosticPositionInterface {
	GTypeInterface parent_iface;

    /** Gets the tree node, if any, to which the diagnostic applies. */
    struct _JagPJCTree *(*getTree)(JagPIDiagnosticPosition *position);
    /** If there is a tree node, get the start position of the tree node.
     *  Otherwise, just returns the same as getPreferredCursor(). */
	JagPCursor *(*getStartCursor)(JagPIDiagnosticPosition *position);
    /** Get the position within the file that most accurately defines the
     *  location for the diagnostic. */
	JagPCursor *(*getPreferredCursor)(JagPIDiagnosticPosition *position);
    /** If there is a tree node, and if endPositions are available, get
     *  the end position of the tree node. Otherwise, just returns the
     *  same as getPreferredCursor(). */
	JagPCursor *(*getEndCursor)(JagPIDiagnosticPosition *position);

};

GType jagp_idiagnostic_position_get_type(void);

JagPCursor *jagp_idiagnostic_position_get_preferred(JagPIDiagnosticPosition *position);
JagPCursor *jagp_idiagnostic_position_get_start(JagPIDiagnosticPosition *position);
JagPCursor *jagp_idiagnostic_position_get_end(JagPIDiagnosticPosition *position);

G_END_DECLS

#endif /* SRC_JAGPIDIAGNOSTICPOSITION_H_ */
