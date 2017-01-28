/*
   File:    moonamepath.h
   Project: moose
   Author:  Douwe Vos
   Date:    Jun 29, 2013
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


#ifndef MOONAMEPATH_H_
#define MOONAMEPATH_H_

#include "../moonodewo.h"
#include "mooidpath.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_NAME_PATH              (moo_name_path_get_type())
#define MOO_NAME_PATH(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_name_path_get_type(), MooNamePath))
#define MOO_NAME_PATH_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_NAME_PATH, MooNamePathClass))
#define MOO_IS_NAME_PATH(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_NAME_PATH))
#define MOO_IS_NAME_PATH_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_NAME_PATH))
#define MOO_NAME_PATH_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_NAME_PATH, MooNamePathClass))
#define MOO_NAME_PATH_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), MOO_TYPE_NAME_PATH, MooNamePathPrivate))


typedef struct _MooNamePath               MooNamePath;
typedef struct _MooNamePathPrivate        MooNamePathPrivate;
typedef struct _MooNamePathClass          MooNamePathClass;


struct _MooNamePath {
	GObject parent;
	MooNamePathPrivate *priv;
};

struct _MooNamePathClass {
	GObjectClass parent_class;
};


GType moo_name_path_get_type();

MooNamePath *moo_name_path_new(CatArrayWo *entries, MooINodeMatcher *matcher);

MooNodeWo *moo_name_path_get_tail(MooNamePath *path, MooNodeWo *root_node);

MooIdPath *moo_name_path_to_id_path(MooNamePath *path, MooNodeWo *base_node, MooIdPath *base_path);


CatStringWo *moo_name_path_create_flatten(MooNamePath *path);

gboolean moo_name_path_equal(const MooNamePath *path_a, const MooNamePath *path_b);

G_END_DECLS

#endif /* MOONAMEPATH_H_ */
