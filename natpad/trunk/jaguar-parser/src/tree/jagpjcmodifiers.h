/*
   File:    jagpjcmodifiers.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 1, 2017
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

#ifndef TREE_JAGPJCMODIFIERS_H_
#define TREE_JAGPJCMODIFIERS_H_

#include "jagpjctree.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCMODIFIERS              (jagp_jcmodifiers_get_type())
#define JAGP_JCMODIFIERS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcmodifiers_get_type(), JagPJCModifiers))
#define JAGP_JCMODIFIERS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCMODIFIERS, JagPJCModifiersClass))
#define JAGP_IS_JCMODIFIERS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCMODIFIERS))
#define JAGP_IS_JCMODIFIERS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCMODIFIERS))
#define JAGP_JCMODIFIERS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCMODIFIERS, JagPJCModifiersClass))


typedef struct _JagPJCModifiers               JagPJCModifiers;
typedef struct _JagPJCModifiersClass          JagPJCModifiersClass;


struct _JagPJCModifiers {
	JagPJCTree parent;
	long long flags;
	CatArrayWo *annotations;
};

struct _JagPJCModifiersClass {
	JagPJCTreeClass parent_class;
};


GType jagp_jcmodifiers_get_type();

JagPJCModifiers *jagp_jcmodifiers_new(long long flags, CatArrayWo *annotations);

G_END_DECLS

#endif /* TREE_JAGPJCMODIFIERS_H_ */
