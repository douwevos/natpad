/*
   File:    catimapiterator.c
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

#include "catimapiterator.h"

G_DEFINE_INTERFACE(CatIMapIterator, cat_imap_iterator, G_TYPE_OBJECT);

static void cat_imap_iterator_default_init (CatIMapIteratorInterface *iface) {
	iface->next = NULL;
}

gboolean cat_imap_iterator_next(CatIMapIterator *self, gpointer *key, gpointer *value) {
	return CAT_IMAP_ITERATOR_GET_INTERFACE(self)->next(self, key, value);
}
