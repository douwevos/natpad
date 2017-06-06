/*
   File:    jagpjcskip.h
   Project: jaguar-parser
   Author:  Douwe Vos
   Date:    May 3, 2017
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

#ifndef TREE_JAGPJCSKIP_H_
#define TREE_JAGPJCSKIP_H_

#include "jagpjcstatement.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCSKIP              (jagp_jcskip_get_type())
#define JAGP_JCSKIP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcskip_get_type(), JagPJCSkip))
#define JAGP_JCSKIP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCSKIP, JagPJCSkipClass))
#define JAGP_IS_JCSKIP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCSKIP))
#define JAGP_IS_JCSKIP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCSKIP))
#define JAGP_JCSKIP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCSKIP, JagPJCSkipClass))


typedef struct _JagPJCSkip               JagPJCSkip;
typedef struct _JagPJCSkipPrivate        JagPJCSkipPrivate;
typedef struct _JagPJCSkipClass          JagPJCSkipClass;


struct _JagPJCSkip {
	JagPJCStatement parent;
};

struct _JagPJCSkipClass {
	JagPJCStatementClass parent_class;
};


GType jagp_jcskip_get_type();

JagPJCSkip *jagp_jcskip_new();

G_END_DECLS

#endif /* TREE_JAGPJCSKIP_H_ */
