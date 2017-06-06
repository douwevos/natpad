/*
   File:    jagpjcimport.h
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

#ifndef TREE_JAGPJCIMPORT_H_
#define TREE_JAGPJCIMPORT_H_

#include "jagpjctree.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAGP_TYPE_JCIMPORT              (jagp_jcimport_get_type())
#define JAGP_JCIMPORT(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jagp_jcimport_get_type(), JagPJCImport))
#define JAGP_JCIMPORT_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAGP_TYPE_JCIMPORT, JagPJCImportClass))
#define JAGP_IS_JCIMPORT(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAGP_TYPE_JCIMPORT))
#define JAGP_IS_JCIMPORT_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAGP_TYPE_JCIMPORT))
#define JAGP_JCIMPORT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAGP_TYPE_JCIMPORT, JagPJCImportClass))


typedef struct _JagPJCImport               JagPJCImport;
typedef struct _JagPJCImportClass          JagPJCImportClass;


struct _JagPJCImport {
	JagPJCTree parent;
	JagPJCTree *qualid;
	gboolean import_static;
};

struct _JagPJCImportClass {
	JagPJCTreeClass parent_class;
};


GType jagp_jcimport_get_type();

JagPJCImport *jagp_jcimport_new(JagPJCTree *qualid, gboolean import_static);

G_END_DECLS

#endif /* TREE_JAGPJCIMPORT_H_ */
