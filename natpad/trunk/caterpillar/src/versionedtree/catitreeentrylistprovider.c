/*
   File:    catitreeentrylistprovider.c
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Feb 6, 2014
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


#include "catitreeentrylistprovider.h"

G_DEFINE_INTERFACE(CatITreeEntryListProvider, cat_itree_entry_list_provider, G_TYPE_OBJECT);

static void cat_itree_entry_list_provider_default_init (CatITreeEntryListProviderInterface *klass) {
}

CatTreeEntryList *cat_itree_entry_list_provider_get_entry_list(CatITreeEntryListProvider *self) {
	g_return_val_if_fail(CAT_IS_ITREE_ENTRY_LIST_PROVIDER(self), NULL);
	return CAT_ITREE_ENTRY_LIST_PROVIDER_GET_INTERFACE(self)->getEntryList(self);
}

CatTreeEntryList *cat_itree_entry_list_provider_get_writable_entry_list(CatITreeEntryListProvider *self) {
	g_return_val_if_fail(CAT_IS_ITREE_ENTRY_LIST_PROVIDER(self), NULL);
	return CAT_ITREE_ENTRY_LIST_PROVIDER_GET_INTERFACE(self)->getWritableEntryList(self);
}

