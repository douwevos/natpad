/*
   File:    jagpjcbreak.h
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

#ifndef TREE_JAGPJCBREAK_H_
#define TREE_JAGPJCBREAK_H_

#include "jagpjcstatement.h"
#include "../code/jagpname.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCBREAK              (jagp_jcbreak_get_type())
#define JAGP_JCBREAK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcbreak_get_type(), JagPJCBreak))
#define JAGP_JCBREAK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCBREAK, JagPJCBreakClass))
#define JAGP_IS_JCBREAK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCBREAK))
#define JAGP_IS_JCBREAK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCBREAK))
#define JAGP_JCBREAK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCBREAK, JagPJCBreakClass))


typedef struct _JagPJCBreak               JagPJCBreak;
typedef struct _JagPJCBreakPrivate        JagPJCBreakPrivate;
typedef struct _JagPJCBreakClass          JagPJCBreakClass;


struct _JagPJCBreak {
	JagPJCStatement parent;
	JagPName *label;
};

struct _JagPJCBreakClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jcbreak_get_type();

JagPJCBreak *jagp_jcbreak_new(JagPName *label);

G_END_DECLS

#endif /* TREE_JAGPJCBREAK_H_ */
