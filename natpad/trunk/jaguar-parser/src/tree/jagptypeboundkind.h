/*
   File:    jagptypeboundkind.h
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

#ifndef TREE_JAGPTYPEBOUNDKIND_H_
#define TREE_JAGPTYPEBOUNDKIND_H_

#include "jagpjctree.h"
#include <caterpillar.h>

G_BEGIN_DECLS

typedef enum _JagPBoundKind JagPBoundKind;

enum _JagPBoundKind {
	JAGP_BOUND_KIND_EXTENDS,
	JAGP_BOUND_KIND_SUPER,
	JAGP_BOUND_KIND_UNBOUND
};

#define JAGP_TYPE_TYPE_BOUND_KIND              (jagp_type_bound_kind_get_type())
#define JAGP_TYPE_BOUND_KIND(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_type_bound_kind_get_type(), JagPTypeBoundKind))
#define JAGP_TYPE_BOUND_KIND_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_TYPE_BOUND_KIND, JagPTypeBoundKindClass))
#define JAGP_IS_TYPE_BOUND_KIND(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_TYPE_BOUND_KIND))
#define JAGP_IS_TYPE_BOUND_KIND_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_TYPE_BOUND_KIND))
#define JAGP_TYPE_BOUND_KIND_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_TYPE_BOUND_KIND, JagPTypeBoundKindClass))


typedef struct _JagPTypeBoundKind               JagPTypeBoundKind;
typedef struct _JagPTypeBoundKindClass          JagPTypeBoundKindClass;


struct _JagPTypeBoundKind {
	JagPJCTree parent;
	JagPBoundKind boundkind;
};

struct _JagPTypeBoundKindClass {
	JagPJCTreeClass parent_class;
};


GType jagp_type_bound_kind_get_type();

JagPTypeBoundKind *jagp_type_bound_kind_new(JagPBoundKind boundkind);

G_END_DECLS

#endif /* TREE_JAGPTYPEBOUNDKIND_H_ */
