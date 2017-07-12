/*
   File:    jagpsimplediagnosticposition.h
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

#ifndef SRC_JAGPSIMPLEDIAGNOSTICPOSITION_H_
#define SRC_JAGPSIMPLEDIAGNOSTICPOSITION_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_SIMPLE_DIAGNOSTIC_POSITION              (jagp_simple_diagnostic_position_get_type())
#define JAGP_SIMPLE_DIAGNOSTIC_POSITION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_simple_diagnostic_position_get_type(), JagPSimpleDiagnosticPosition))
#define JAGP_SIMPLE_DIAGNOSTIC_POSITION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_SIMPLE_DIAGNOSTIC_POSITION, JagPSimpleDiagnosticPositionClass))
#define JAGP_IS_SIMPLE_DIAGNOSTIC_POSITION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_SIMPLE_DIAGNOSTIC_POSITION))
#define JAGP_IS_SIMPLE_DIAGNOSTIC_POSITION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_SIMPLE_DIAGNOSTIC_POSITION))
#define JAGP_SIMPLE_DIAGNOSTIC_POSITION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_SIMPLE_DIAGNOSTIC_POSITION, JagPSimpleDiagnosticPositionClass))


typedef struct _JagPSimpleDiagnosticPosition               JagPSimpleDiagnosticPosition;
typedef struct _JagPSimpleDiagnosticPositionPrivate        JagPSimpleDiagnosticPositionPrivate;
typedef struct _JagPSimpleDiagnosticPositionClass          JagPSimpleDiagnosticPositionClass;


struct _JagPSimpleDiagnosticPosition {
	GObject parent;
};

struct _JagPSimpleDiagnosticPositionClass {
	GObjectClass parent_class;
};


GType jagp_simple_diagnostic_position_get_type();

JagPSimpleDiagnosticPosition *jagp_simple_diagnostic_position_new();

G_END_DECLS

#endif /* SRC_JAGPSIMPLEDIAGNOSTICPOSITION_H_ */
