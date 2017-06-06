/*
   File:    jagpjcenhancedforloop.h
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

#ifndef TREE_JAGPJCENHANCEDFORLOOP_H_
#define TREE_JAGPJCENHANCEDFORLOOP_H_

#include "jagpjcstatement.h"
#include "jagpjcvariabledecl.h"
#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCENHANCED_FOR_LOOP              (jagp_jcenhanced_for_loop_get_type())
#define JAGP_JCENHANCED_FOR_LOOP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcenhanced_for_loop_get_type(), JagPJCEnhancedForLoop))
#define JAGP_JCENHANCED_FOR_LOOP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCENHANCED_FOR_LOOP, JagPJCEnhancedForLoopClass))
#define JAGP_IS_JCENHANCED_FOR_LOOP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCENHANCED_FOR_LOOP))
#define JAGP_IS_JCENHANCED_FOR_LOOP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCENHANCED_FOR_LOOP))
#define JAGP_JCENHANCED_FOR_LOOP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCENHANCED_FOR_LOOP, JagPJCEnhancedForLoopClass))


typedef struct _JagPJCEnhancedForLoop               JagPJCEnhancedForLoop;
typedef struct _JagPJCEnhancedForLoopClass          JagPJCEnhancedForLoopClass;


struct _JagPJCEnhancedForLoop {
	JagPJCStatement parent;
	JagPJCVariableDecl *var;
	JagPJCExpression *expr;
	JagPJCStatement *body;
};

struct _JagPJCEnhancedForLoopClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jcenhanced_for_loop_get_type();

JagPJCEnhancedForLoop *jagp_jcenhanced_for_loop_new(JagPJCVariableDecl *var, JagPJCExpression *expr, JagPJCStatement *body);

G_END_DECLS

#endif /* TREE_JAGPJCENHANCEDFORLOOP_H_ */
