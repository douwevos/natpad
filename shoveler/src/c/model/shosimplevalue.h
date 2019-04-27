/*
   File:    shosimplevalue.h
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

#ifndef SHOSIMPLEVALUE_H_
#define SHOSIMPLEVALUE_H_

#include "shoivalue.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define SHO_TYPE_SIMPLE_VALUE              (sho_simple_value_get_type())
#define SHO_SIMPLE_VALUE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), sho_simple_value_get_type(), ShoSimpleValue))
#define SHO_SIMPLE_VALUE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), SHO_TYPE_SIMPLE_VALUE, ShoSimpleValueClass))
#define SHO_IS_SIMPLE_VALUE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), SHO_TYPE_SIMPLE_VALUE))
#define SHO_IS_SIMPLE_VALUE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), SHO_TYPE_SIMPLE_VALUE))
#define SHO_SIMPLE_VALUE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), SHO_TYPE_SIMPLE_VALUE, ShoSimpleValueClass))


typedef struct _ShoSimpleValue               ShoSimpleValue;
typedef struct _ShoSimpleValuePrivate        ShoSimpleValuePrivate;
typedef struct _ShoSimpleValueClass          ShoSimpleValueClass;


struct _ShoSimpleValue {
	GObject parent;
};

struct _ShoSimpleValueClass {
	GObjectClass parent_class;
};


GType sho_simple_value_get_type();

ShoSimpleValue *sho_simple_value_new_string(CatStringWo *value);
ShoSimpleValue *sho_simple_value_new_boolean(gboolean val);
ShoSimpleValue *sho_simple_value_new_integer(int val);
ShoSimpleValue *sho_simple_value_new_double(double val);

CatStringWo *sho_simple_value_get_string(ShoSimpleValue *value);
double sho_simple_value_get_double(ShoSimpleValue *value, int default_value);

gboolean sho_simple_value_get_boolean(ShoSimpleValue *value, gboolean default_value);
int sho_simple_value_get_integer(ShoSimpleValue *value, int default_value);

G_END_DECLS

#endif /* SHOSIMPLEVALUE_H_ */
