/*
   File:    jagpjcsynchronized.h
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

#ifndef TREE_JAGPJCSYNCHRONIZED_H_
#define TREE_JAGPJCSYNCHRONIZED_H_

#include "jagpjcstatement.h"
#include "jagpjcexpression.h"
#include "jagpjcblock.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCSYNCHRONIZED              (jagp_jcsynchronized_get_type())
#define JAGP_JCSYNCHRONIZED(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcsynchronized_get_type(), JagPJCSynchronized))
#define JAGP_JCSYNCHRONIZED_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCSYNCHRONIZED, JagPJCSynchronizedClass))
#define JAGP_IS_JCSYNCHRONIZED(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCSYNCHRONIZED))
#define JAGP_IS_JCSYNCHRONIZED_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCSYNCHRONIZED))
#define JAGP_JCSYNCHRONIZED_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCSYNCHRONIZED, JagPJCSynchronizedClass))


typedef struct _JagPJCSynchronized               JagPJCSynchronized;
typedef struct _JagPJCSynchronizedClass          JagPJCSynchronizedClass;


struct _JagPJCSynchronized {
	JagPJCStatement parent;
    JagPJCExpression *lock;
    JagPJCBlock *body;
};

struct _JagPJCSynchronizedClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jcsynchronized_get_type();

JagPJCSynchronized *jagp_jcsynchronized_new(JagPJCExpression *lock, JagPJCBlock *body);

G_END_DECLS

#endif /* TREE_JAGPJCSYNCHRONIZED_H_ */
