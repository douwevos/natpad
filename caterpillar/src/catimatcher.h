/*
   File:    catimatcher.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 4, 2014
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2014 Douwe Vos.

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

#ifndef CATIMATCHER_H_
#define CATIMATCHER_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_IMATCHER                 (cat_imatcher_get_type())
#define CAT_IMATCHER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), CAT_TYPE_IMATCHER, CatIMatcher))
#define CAT_IS_IMATCHER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), CAT_TYPE_IMATCHER))
#define CAT_IMATCHER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), CAT_TYPE_IMATCHER, CatIMatcherInterface))


typedef struct _CatIMatcher               CatIMatcher; /* dummy object */
typedef struct _CatIMatcherInterface      CatIMatcherInterface;

struct _CatIMatcherInterface {
	GTypeInterface parent_iface;

	gboolean (*matches)(CatIMatcher *self, GObject *to_match);
};

GType cat_imatcher_get_type(void);

gboolean cat_imatcher_matches(CatIMatcher *self, GObject *to_match);

G_END_DECLS

#endif /* CATIMATCHER_H_ */
