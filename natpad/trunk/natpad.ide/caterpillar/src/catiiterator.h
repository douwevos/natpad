/*
   File:    catiiterator.h
   Project: Caterpillar
   Author:  Douwe Vos
   Date:    May 15, 2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2008 Douwe Vos.

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

#ifndef CATIITERATOR_H_
#define CATIITERATOR_H_

#include <glib-object.h>

#define CAT_TYPE_IITERATOR                 (cat_iiterator_get_type ())
#define CAT_IITERATOR(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), CAT_TYPE_IITERATOR, CatIIterator))
#define CAT_IS_IITERATOR(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_IITERATOR))
#define CAT_IITERATOR_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), CAT_TYPE_IITERATOR, CatIIteratorInterface))


typedef struct _CatIIterator               CatIIterator; /* dummy object */
typedef struct _CatIIteratorInterface      CatIIteratorInterface;

struct _CatIIteratorInterface {
  GTypeInterface parent_iface;

  gpointer (*next) (CatIIterator *self);
  gboolean (*has_next) (CatIIterator *self);
  gboolean (*is_last) (CatIIterator *self);
};

GType cat_iiterator_get_type(void);

gpointer cat_iiterator_next(CatIIterator *self);
gboolean cat_iiterator_has_next(CatIIterator *self);
gboolean cat_iiterator_is_last(CatIIterator *self);

#endif /* CATIITERATOR_H_ */
