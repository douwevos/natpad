/*
   File:    jagpqualident.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    Aug 4, 2017
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

#ifndef NTREE_JAGPQUALIDENT_H_
#define NTREE_JAGPQUALIDENT_H_

#include "../tree/jagpjcexpression.h"
#include "../jagptoken.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_QUALIDENT              (jagp_qualident_get_type())
#define JAGP_QUALIDENT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_qualident_get_type(), JagPQualident))
#define JAGP_QUALIDENT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_QUALIDENT, JagPQualidentClass))
#define JAGP_IS_QUALIDENT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_QUALIDENT))
#define JAGP_IS_QUALIDENT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_QUALIDENT))
#define JAGP_QUALIDENT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_QUALIDENT, JagPQualidentClass))


typedef struct _JagPQualident               JagPQualident;
typedef struct _JagPQualidentPrivate        JagPQualidentPrivate;
typedef struct _JagPQualidentClass          JagPQualidentClass;


struct _JagPQualident {
	JagPJCExpression parent;
};

struct _JagPQualidentClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_qualident_get_type();

JagPQualident *jagp_qualident_new(JagPToken *ident);

void jagp_qualident_add_dot_ident(JagPQualident *qualident, JagPToken *dot, CatArrayWo *tyannos, JagPToken *ident);

gboolean jagp_qualident_is_plain_but_not_this(JagPQualident *qualident, JagPName *this_name);

G_END_DECLS

#endif /* NTREE_JAGPQUALIDENT_H_ */
