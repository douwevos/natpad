/*
   File:    jagpjcbinary.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 5, 2017
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

#ifndef TREE_JAGPJCBINARY_H_
#define TREE_JAGPJCBINARY_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCBINARY              (jagp_jcbinary_get_type())
#define JAGP_JCBINARY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcbinary_get_type(), JagPJCBinary))
#define JAGP_JCBINARY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCBINARY, JagPJCBinaryClass))
#define JAGP_IS_JCBINARY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCBINARY))
#define JAGP_IS_JCBINARY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCBINARY))
#define JAGP_JCBINARY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCBINARY, JagPJCBinaryClass))


typedef struct _JagPJCBinary               JagPJCBinary;
typedef struct _JagPJCBinaryClass          JagPJCBinaryClass;


struct _JagPJCBinary {
	JagPJCExpression parent;
	JagPTag opcode;
	JagPJCExpression *lhs;
	JagPJCExpression *rhs;
};

struct _JagPJCBinaryClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jcbinary_get_type();

JagPJCBinary *jagp_jcbinary_new(JagPTag opcode, JagPJCExpression *lhs, JagPJCExpression *rhs);

G_END_DECLS

#endif /* TREE_JAGPJCBINARY_H_ */
