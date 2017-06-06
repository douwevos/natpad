/*
   File:    jagpjcmethodinvocation.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 12, 2017
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

#ifndef TREE_JAGPJCMETHODINVOCATION_H_
#define TREE_JAGPJCMETHODINVOCATION_H_

#include "jagpjcpolyexpression.h"
#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCMETHOD_INVOCATION              (jagp_jcmethod_invocation_get_type())
#define JAGP_JCMETHOD_INVOCATION(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcmethod_invocation_get_type(), JagPJCMethodInvocation))
#define JAGP_JCMETHOD_INVOCATION_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCMETHOD_INVOCATION, JagPJCMethodInvocationClass))
#define JAGP_IS_JCMETHOD_INVOCATION(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCMETHOD_INVOCATION))
#define JAGP_IS_JCMETHOD_INVOCATION_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCMETHOD_INVOCATION))
#define JAGP_JCMETHOD_INVOCATION_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCMETHOD_INVOCATION, JagPJCMethodInvocationClass))


typedef struct _JagPJCMethodInvocation               JagPJCMethodInvocation;
typedef struct _JagPJCMethodInvocationClass          JagPJCMethodInvocationClass;


struct _JagPJCMethodInvocation {
	JagPJCPolyExpression parent;
	CatArrayWo /*<JCExpression>*/ *typeargs;
	JagPJCExpression *meth;
	CatArrayWo /*<JCExpression>*/ *args;
};

struct _JagPJCMethodInvocationClass {
	JagPJCPolyExpressionClass parent_class;
};


GType jagp_jcmethod_invocation_get_type();

JagPJCMethodInvocation *jagp_jcmethod_invocation_new(CatArrayWo /*<JCExpression>*/ *typeargs, JagPJCExpression *meth, CatArrayWo /*<JCExpression>*/ *args);

G_END_DECLS

#endif /* TREE_JAGPJCMETHODINVOCATION_H_ */
