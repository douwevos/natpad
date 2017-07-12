/*
   File:    jagpjcblock.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 9, 2017
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

#ifndef TREE_JAGPJCBLOCK_H_
#define TREE_JAGPJCBLOCK_H_

#include "jagpjcstatement.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCBLOCK              (jagp_jcblock_get_type())
#define JAGP_JCBLOCK(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcblock_get_type(), JagPJCBlock))
#define JAGP_JCBLOCK_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCBLOCK, JagPJCBlockClass))
#define JAGP_IS_JCBLOCK(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCBLOCK))
#define JAGP_IS_JCBLOCK_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCBLOCK))
#define JAGP_JCBLOCK_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCBLOCK, JagPJCBlockClass))


typedef struct _JagPJCBlock               JagPJCBlock;
typedef struct _JagPJCBlockClass          JagPJCBlockClass;


struct _JagPJCBlock {
	JagPJCStatement parent;
	long long flags;
	CatArrayWo *stats;
	JagPCursor *endpos;
};

struct _JagPJCBlockClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jcblock_get_type();

JagPJCBlock *jagp_jcblock_new(long long flags, CatArrayWo *stats);

G_END_DECLS

#endif /* TREE_JAGPJCBLOCK_H_ */
