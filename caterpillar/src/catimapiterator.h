/*
   File:    catimapiterator.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Mar 29, 2017
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

#ifndef SRC_CATIMAPITERATOR_H_
#define SRC_CATIMAPITERATOR_H_

#include <glib-object.h>

G_BEGIN_DECLS

#define CAT_TYPE_IMAP_ITERATOR                 (cat_imap_iterator_get_type())
#define CAT_IMAP_ITERATOR(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), CAT_TYPE_IMAP_ITERATOR, CatIMapIterator))
#define CAT_IS_IMAP_ITERATOR(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), CAT_TYPE_IMAP_ITERATOR))
#define CAT_IMAP_ITERATOR_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), CAT_TYPE_IMAP_ITERATOR, CatIMapIteratorInterface))

typedef struct _CatIMapIterator               CatIMapIterator; /* dummy object */
typedef struct _CatIMapIteratorInterface      CatIMapIteratorInterface;

struct _CatIMapIteratorInterface {
	GTypeInterface parent_iface;

	gboolean (*next) (CatIMapIterator *self, gpointer *key, gpointer *value);
};

GType cat_imap_iterator_get_type(void);

gboolean cat_imap_iterator_next(CatIMapIterator *self, gpointer *key, gpointer *value);

G_END_DECLS

#endif /* SRC_CATIMAPITERATOR_H_ */
