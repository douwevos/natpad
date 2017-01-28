/*
   File:    sholistvalue.h
   Project: caterpillar.sht
   Author:  Douwe Vos
   Date:    May 5, 2012
   Web:     http://www.natpad.net/
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2012 Douwe Vos.

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


#ifndef SHOLISTVALUE_H_
#define SHOLISTVALUE_H_

#include "shoivalue.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define SHO_TYPE_LIST_VALUE              (sho_list_value_get_type())
#define SHO_LIST_VALUE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_list_value_get_type(), ShoListValue))
#define SHO_LIST_VALUE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_LIST_VALUE, ShoListValueClass))
#define SHO_IS_LIST_VALUE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_LIST_VALUE))
#define SHO_IS_LIST_VALUE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_LIST_VALUE))
#define SHO_LIST_VALUE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_LIST_VALUE, ShoListValueClass))


typedef struct _ShoListValue               ShoListValue;
typedef struct _ShoListValuePrivate        ShoListValuePrivate;
typedef struct _ShoListValueClass          ShoListValueClass;


struct _ShoListValue {
	GObject parent;
};

struct _ShoListValueClass {
	GObjectClass parent_class;
};


GType sho_list_value_get_type();

ShoListValue *sho_list_value_new();

void sho_list_value_add_value(ShoListValue *list_value, ShoIValue *value);
void sho_list_value_add_string(ShoListValue *list_value, CatStringWo *text);

int sho_list_value_count(ShoListValue *list_value);
ShoIValue *sho_list_value_get_at(ShoListValue *list_value, int index);

CatIIterator *sho_list_value_iterator(ShoListValue *list_value);

G_END_DECLS

#endif /* SHOLISTVALUE_H_ */
