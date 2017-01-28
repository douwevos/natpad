/*
   File:    mooidpath.h
   Project: moose
   Author:  Douwe Vos
   Date:    Nov 6, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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


#ifndef MOOIDPATH_H_
#define MOOIDPATH_H_

#include "../moonodewo.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_ID_PATH              (moo_id_path_get_type())
#define MOO_ID_PATH(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_id_path_get_type(), MooIdPath))
#define MOO_ID_PATH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_ID_PATH, MooIdPathClass))
#define MOO_IS_ID_PATH(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_ID_PATH))
#define MOO_IS_ID_PATH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_ID_PATH))
#define MOO_ID_PATH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_ID_PATH, MooIdPathClass))
#define MOO_ID_PATH_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_ID_PATH, MooIdPathPrivate))


typedef struct _MooIdPath               MooIdPath;
typedef struct _MooIdPathPrivate        MooIdPathPrivate;
typedef struct _MooIdPathClass          MooIdPathClass;


struct _MooIdPath {
	GInitiallyUnowned parent;
	MooIdPathPrivate *priv;
};

struct _MooIdPathClass {
	GInitiallyUnownedClass parent_class;
};


GType moo_id_path_get_type();

MooIdPath *moo_id_path_new();

MooIdPath *moo_id_path_append_id(MooIdPath *base_path, long long unique_id, int index);

MooIdPath *moo_id_path_append_multiple_ids(MooIdPath *base_path, long long *unique_ids, int *indexes, int count2add);

MooNodeWo *moo_id_path_create_editable_node(MooIdPath *id_path, MooNodeWo *root_node);

int moo_id_path_count(MooIdPath *id_path);
MooNodeWo *moo_id_path_get_tail(MooIdPath *id_path, MooNodeWo *root_node);
MooNodeWo *moo_id_path_get_at(MooIdPath *id_path, MooNodeWo *root_node, int index);

G_END_DECLS


#endif /* MOOIDPATH_H_ */
