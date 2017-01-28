
/*
   File:    groactionnonassoc.h
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 23, 2016
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

#ifndef STATE_GROACTIONNONASSOC_H_
#define STATE_GROACTIONNONASSOC_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define GRO_TYPE_ACTION_NON_ASSOC              (gro_action_non_assoc_get_type())
#define GRO_ACTION_NON_ASSOC(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), gro_action_non_assoc_get_type(), GroActionNonAssoc))
#define GRO_ACTION_NON_ASSOC_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GRO_TYPE_ACTION_NON_ASSOC, GroActionNonAssocClass))
#define GRO_IS_ACTION_NON_ASSOC(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GRO_TYPE_ACTION_NON_ASSOC))
#define GRO_IS_ACTION_NON_ASSOC_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GRO_TYPE_ACTION_NON_ASSOC))
#define GRO_ACTION_NON_ASSOC_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GRO_TYPE_ACTION_NON_ASSOC, GroActionNonAssocClass))


typedef struct _GroActionNonAssoc               GroActionNonAssoc;
typedef struct _GroActionNonAssocPrivate        GroActionNonAssocPrivate;
typedef struct _GroActionNonAssocClass          GroActionNonAssocClass;


struct _GroActionNonAssoc {
	GObject parent;
};

struct _GroActionNonAssocClass {
	GObjectClass parent_class;
};


GType gro_action_non_assoc_get_type();

GroActionNonAssoc *gro_action_non_assoc_new();

G_END_DECLS

#endif /* STATE_GROACTIONNONASSOC_H_ */
