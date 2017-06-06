/*
   File:    jagpjcswitch.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 13, 2017
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

#ifndef TREE_JAGPJCSWITCH_H_
#define TREE_JAGPJCSWITCH_H_

#include "jagpjcexpression.h"
#include "jagpjcstatement.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCSWITCH              (jagp_jcswitch_get_type())
#define JAGP_JCSWITCH(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcswitch_get_type(), JagPJCSwitch))
#define JAGP_JCSWITCH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCSWITCH, JagPJCSwitchClass))
#define JAGP_IS_JCSWITCH(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCSWITCH))
#define JAGP_IS_JCSWITCH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCSWITCH))
#define JAGP_JCSWITCH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCSWITCH, JagPJCSwitchClass))


typedef struct _JagPJCSwitch               JagPJCSwitch;
typedef struct _JagPJCSwitchClass          JagPJCSwitchClass;


struct _JagPJCSwitch {
	JagPJCStatement parent;
    JagPJCExpression *selector;
    CatArrayWo /*<JagPJCCase>*/ *cases;
};

struct _JagPJCSwitchClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jcswitch_get_type();

JagPJCSwitch *jagp_jcswitch_new(JagPJCExpression *selector, CatArrayWo /*<JagPJCCase>*/ *cases);

G_END_DECLS

#endif /* TREE_JAGPJCSWITCH_H_ */
