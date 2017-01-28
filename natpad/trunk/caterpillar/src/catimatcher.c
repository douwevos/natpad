/*
   File:    catimatcher.c
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

#include "catimatcher.h"

G_DEFINE_INTERFACE(CatIMatcher, cat_imatcher, G_TYPE_OBJECT);

static void cat_imatcher_default_init (CatIMatcherInterface *klass) {
}

gboolean cat_imatcher_matches(CatIMatcher *self, GObject *to_match) {
	g_return_val_if_fail(CAT_IS_IMATCHER(self), FALSE);
	return CAT_IMATCHER_GET_INTERFACE(self)->matches(self, to_match);
}
