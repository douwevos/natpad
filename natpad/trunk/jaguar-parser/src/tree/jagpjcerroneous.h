/*
   File:    jagpjcerroneous.h
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

#ifndef TREE_JAGPJCERRONEOUS_H_
#define TREE_JAGPJCERRONEOUS_H_

#include "jagpjcexpression.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCERRONEOUS              (jagp_jcerroneous_get_type())
#define JAGP_JCERRONEOUS(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcerroneous_get_type(), JagPJCErroneous))
#define JAGP_JCERRONEOUS_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCERRONEOUS, JagPJCErroneousClass))
#define JAGP_IS_JCERRONEOUS(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCERRONEOUS))
#define JAGP_IS_JCERRONEOUS_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCERRONEOUS))
#define JAGP_JCERRONEOUS_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCERRONEOUS, JagPJCErroneousClass))


typedef struct _JagPJCErroneous               JagPJCErroneous;
typedef struct _JagPJCErroneousClass          JagPJCErroneousClass;


struct _JagPJCErroneous {
	JagPJCExpression parent;
	CatArrayWo *errs;
};

struct _JagPJCErroneousClass {
	JagPJCExpressionClass parent_class;
};


GType jagp_jcerroneous_get_type();

JagPJCErroneous *jagp_jcerroneous_new(CatArrayWo *errs);

G_END_DECLS

#endif /* TREE_JAGPJCERRONEOUS_H_ */
