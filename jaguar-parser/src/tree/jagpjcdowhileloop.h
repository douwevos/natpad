/*
   File:    jagpjcdowhileloop.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 11, 2017
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

#ifndef TREE_JAGPJCDOWHILELOOP_H_
#define TREE_JAGPJCDOWHILELOOP_H_

#include "jagpjcstatement.h"
#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCDO_WHILE_LOOP              (jagp_jcdo_while_loop_get_type())
#define JAGP_JCDO_WHILE_LOOP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcdo_while_loop_get_type(), JagPJCDoWhileLoop))
#define JAGP_JCDO_WHILE_LOOP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCDO_WHILE_LOOP, JagPJCDoWhileLoopClass))
#define JAGP_IS_JCDO_WHILE_LOOP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCDO_WHILE_LOOP))
#define JAGP_IS_JCDO_WHILE_LOOP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCDO_WHILE_LOOP))
#define JAGP_JCDO_WHILE_LOOP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCDO_WHILE_LOOP, JagPJCDoWhileLoopClass))


typedef struct _JagPJCDoWhileLoop               JagPJCDoWhileLoop;
typedef struct _JagPJCDoWhileLoopClass          JagPJCDoWhileLoopClass;


struct _JagPJCDoWhileLoop {
	JagPJCStatement parent;
	JagPJCStatement *body;
	JagPJCExpression *cond;
};

struct _JagPJCDoWhileLoopClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jcdo_while_loop_get_type();

JagPJCDoWhileLoop *jagp_jcdo_while_loop_new(JagPJCStatement *body, JagPJCExpression *cond);

G_END_DECLS

#endif /* TREE_JAGPJCDOWHILELOOP_H_ */
