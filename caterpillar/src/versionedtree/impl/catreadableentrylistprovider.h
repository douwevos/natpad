/*
   File:    catreadableentrylistprovider.h
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

#ifndef CATREADABLEENTRYLISTPROVIDER_H_
#define CATREADABLEENTRYLISTPROVIDER_H_

#include "cattreeentrylist.h"

G_BEGIN_DECLS

#define CAT_TYPE_READABLE_ENTRY_LIST_PROVIDER              (cat_readable_entry_list_provider_get_type())
#define CAT_READABLE_ENTRY_LIST_PROVIDER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), cat_readable_entry_list_provider_get_type(), CatReadableEntryListProvider))
#define CAT_READABLE_ENTRY_LIST_PROVIDER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), CAT_TYPE_READABLE_ENTRY_LIST_PROVIDER, CatReadableEntryListProviderClass))
#define CAT_IS_READABLE_ENTRY_LIST_PROVIDER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), CAT_TYPE_READABLE_ENTRY_LIST_PROVIDER))
#define CAT_IS_READABLE_ENTRY_LIST_PROVIDER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), CAT_TYPE_READABLE_ENTRY_LIST_PROVIDER))
#define CAT_READABLE_ENTRY_LIST_PROVIDER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), CAT_TYPE_READABLE_ENTRY_LIST_PROVIDER, CatReadableEntryListProviderClass))


typedef struct _CatReadableEntryListProvider               CatReadableEntryListProvider;
typedef struct _CatReadableEntryListProviderPrivate        CatReadableEntryListProviderPrivate;
typedef struct _CatReadableEntryListProviderClass          CatReadableEntryListProviderClass;


struct _CatReadableEntryListProvider {
	GObject parent;
};

struct _CatReadableEntryListProviderClass {
	GObjectClass parent_class;
};


GType cat_readable_entry_list_provider_get_type();

CatReadableEntryListProvider *cat_readable_entry_list_provider_new(CatTreeEntryList *entry_list);

G_END_DECLS

#endif /* CATREADABLEENTRYLISTPROVIDER_H_ */
