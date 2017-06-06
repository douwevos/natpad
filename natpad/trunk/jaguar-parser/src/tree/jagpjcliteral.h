/*
   File:    jagpjcliteral.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Jun 5, 2017
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

#ifndef TREE_JAGPJCLITERAL_H_
#define TREE_JAGPJCLITERAL_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

typedef enum _JagPTpTag JagPTpTag;

enum _JagPTpTag {
	JAGP_TP_TAG_BYTE,
	JAGP_TP_TAG_INT
};


#define JAGP_TYPE_JCLITERAL              (jagp_jcliteral_get_type())
#define JAGP_JCLITERAL(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcliteral_get_type(), JagPJCLiteral))
#define JAGP_JCLITERAL_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCLITERAL, JagPJCLiteralClass))
#define JAGP_IS_JCLITERAL(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCLITERAL))
#define JAGP_IS_JCLITERAL_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCLITERAL))
#define JAGP_JCLITERAL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCLITERAL, JagPJCLiteralClass))


typedef struct _JagPJCLiteral               JagPJCLiteral;
typedef struct _JagPJCLiteralClass          JagPJCLiteralClass;


struct _JagPJCLiteral {
	JagPJCExpression parent;
	JagPTpTag type_tag;
	GObject *value;
};

struct _JagPJCLiteralClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jcliteral_get_type();

JagPJCLiteral *jagp_jcliteral_new(JagPTpTag type_tag, GObject *value);

G_END_DECLS

#endif /* TREE_JAGPJCLITERAL_H_ */
