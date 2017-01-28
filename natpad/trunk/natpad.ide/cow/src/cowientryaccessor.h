/*
   File:    cowientryaccessor.h
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

#ifndef COWIENTRYACCESSOR_H_
#define COWIENTRYACCESSOR_H_

#include <caterpillar.h>

#define COW_TYPE_IENTRY_ACCESSOR                 (cow_ientry_accessor_get_type())
#define COW_IENTRY_ACCESSOR(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), COW_TYPE_IENTRY_ACCESSOR, CowIEntryAccessor))
#define COW_IS_IENTRY_ACCESSOR(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), COW_TYPE_IENTRY_ACCESSOR))
#define COW_IENTRY_ACCESSOR_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), COW_TYPE_IENTRY_ACCESSOR, CowIEntryAccessorInterface))


typedef struct _CowIEntryAccessor               CowIEntryAccessor; /* dummy object */
typedef struct _CowIEntryAccessorInterface      CowIEntryAccessorInterface;

struct _CowIEntryAccessorInterface {
	GTypeInterface parent_iface;
	gpointer (*get)(CowIEntryAccessor *self, gpointer config);
	gboolean (*set)(CowIEntryAccessor *self, gpointer config, gpointer entry);
};

GType cow_ientry_accessor_get_type(void);

gpointer cow_ientry_accessor_get(CowIEntryAccessor *self, gpointer config);
gboolean cow_ientry_accessor_set(CowIEntryAccessor *self, gpointer config, gpointer entry);

#endif /* COWIENTRYACCESSOR_H_ */
