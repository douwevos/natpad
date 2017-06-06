/*
   File:    jagpjccontinue.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 14, 2017
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

#ifndef TREE_JAGPJCCONTINUE_H_
#define TREE_JAGPJCCONTINUE_H_

#include "jagpjcstatement.h"
#include "../code/jagpname.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCCONTINUE              (jagp_jccontinue_get_type())
#define JAGP_JCCONTINUE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jccontinue_get_type(), JagPJCContinue))
#define JAGP_JCCONTINUE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCCONTINUE, JagPJCContinueClass))
#define JAGP_IS_JCCONTINUE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCCONTINUE))
#define JAGP_IS_JCCONTINUE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCCONTINUE))
#define JAGP_JCCONTINUE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCCONTINUE, JagPJCContinueClass))


typedef struct _JagPJCContinue               JagPJCContinue;
typedef struct _JagPJCContinueClass          JagPJCContinueClass;


struct _JagPJCContinue {
	JagPJCStatement parent;
	JagPName *label;
};

struct _JagPJCContinueClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jccontinue_get_type();

JagPJCContinue *jagp_jccontinue_new(JagPName *label);

G_END_DECLS

#endif /* TREE_JAGPJCCONTINUE_H_ */
