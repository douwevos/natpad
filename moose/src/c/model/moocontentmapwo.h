/*
   File:    MooContentMapWo.h
   Project: moose
   Author:  Douwe Vos
   Date:    Oct 31, 2013
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

#ifndef MooContentMapWo_H_
#define MooContentMapWo_H_

#include "mooicontent.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define MOO_TYPE_CONTENT_MAP_WO              (moo_content_map_wo_get_type())
#define MOO_CONTENT_MAP_WO(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), moo_content_map_wo_get_type(), MooContentMapWo))
#define MOO_CONTENT_MAP_WO_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), MOO_TYPE_CONTENT_MAP_WO, MooContentMapWoClass))
#define MOO_IS_CONTENT_MAP_WO(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), MOO_TYPE_CONTENT_MAP_WO))
#define MOO_IS_CONTENT_MAP_WO_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), MOO_TYPE_CONTENT_MAP_WO))
#define MOO_CONTENT_MAP_WO_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), MOO_TYPE_CONTENT_MAP_WO, MooContentMapWoClass))

typedef struct _MooContentMapWo               MooContentMapWo;
typedef struct _MooContentMapWoPrivate        MooContentMapWoPrivate;
typedef struct _MooContentMapWoClass          MooContentMapWoClass;

struct _MooContentMapWo {
	GObject parent;
};

struct _MooContentMapWoClass {
	GObjectClass parent_class;
};


GType moo_content_map_wo_get_type();

MooContentMapWo *moo_content_map_wo_new();

MooContentMapWo *moo_content_map_wo_ensure_editable(MooContentMapWo *content_map);
gboolean moo_content_map_wo_is_fixed(MooContentMapWo *content_map);
MooContentMapWo *moo_content_map_wo_anchor(MooContentMapWo *content_map, int version);
gboolean moo_content_map_wo_equal(MooContentMapWo *content_map_a, MooContentMapWo *content_map_b);

MooIContent *moo_content_map_wo_get(MooContentMapWo *self, CatStringWo *key);
GObject *moo_content_map_wo_get_raw(MooContentMapWo *self, CatStringWo *key);

void moo_content_map_wo_set(MooContentMapWo *e_content_map, MooIContent *content);
void moo_content_map_wo_set_with_key(MooContentMapWo *e_content_map, MooIContent *content, CatStringWo *key);
void moo_content_map_wo_create_link(MooContentMapWo *e_content_map, CatStringWo *key, CatStringWo *link_to);

G_END_DECLS

#endif /* MooContentMapWo_H_ */
