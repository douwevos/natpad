/*
   File:    catiiterator.c
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


#include "catiiterator.h"

static void cat_iiterator_interface_init (gpointer klass) {
//	CatIIteratorInterface *iface = (CatIIteratorInterface*) klass;
}

GType cat_iiterator_get_type (void) {
  static volatile gsize g_define_type_id__volatile = 0;
  if (g_once_init_enter (&g_define_type_id__volatile)) {


	  static const GTypeInfo info = {
	          sizeof (CatIIteratorInterface),
	          cat_iiterator_interface_init,   /* base_init */
	          NULL,   /* base_finalize */
	        };


      GType g_define_type_id = g_type_register_static(G_TYPE_INTERFACE,
                                       g_intern_static_string ("CatIIterator"), &info,0);
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id);
    }
  return g_define_type_id__volatile;
}

gpointer cat_iiterator_next(CatIIterator *self) {
  g_return_val_if_fail(CAT_IS_IITERATOR(self), NULL);
  return CAT_IITERATOR_GET_INTERFACE(self)->next(self);
}

gboolean cat_iiterator_has_next(CatIIterator *self) {
  g_return_val_if_fail(CAT_IS_IITERATOR(self), FALSE);
  return CAT_IITERATOR_GET_INTERFACE(self)->has_next(self);
}

gboolean cat_iiterator_is_last(CatIIterator *self) {
  g_return_val_if_fail(CAT_IS_IITERATOR(self), FALSE);
  return CAT_IITERATOR_GET_INTERFACE(self)->is_last(self);
}
