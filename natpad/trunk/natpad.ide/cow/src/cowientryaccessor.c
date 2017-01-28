/*
   File:    cowientryaccessor.c
   Project: cow
   Author:  Douwe Vos
   Date:    Jun 25, 2015
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2015 Douwe Vos.

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

#include "cowientryaccessor.h"

G_DEFINE_INTERFACE(CowIEntryAccessor, cow_ientry_accessor, G_TYPE_OBJECT);

static void cow_ientry_accessor_default_init (CowIEntryAccessorInterface *iface) {
	iface->get = NULL;
	iface->set = NULL;
}

gpointer cow_ientry_accessor_get(CowIEntryAccessor *self, gpointer config) {
	g_return_val_if_fail(COW_IS_IENTRY_ACCESSOR(self), NULL);
	return COW_IENTRY_ACCESSOR_GET_INTERFACE(self)->get(self, config);
}

gboolean cow_ientry_accessor_set(CowIEntryAccessor *self, gpointer config, gpointer entry) {
	g_return_val_if_fail(COW_IS_IENTRY_ACCESSOR(self), FALSE);
	return COW_IENTRY_ACCESSOR_GET_INTERFACE(self)->set(self, config, entry);
}
