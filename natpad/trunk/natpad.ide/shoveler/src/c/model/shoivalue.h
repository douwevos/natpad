/*
   File:    shoivalue.h
   Project: shoveler
   Author:  Douwe Vos
   Date:    Oct 5, 2014
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

#ifndef SHOIVALUE_H_
#define SHOIVALUE_H_

#include <caterpillar.h>

#define SHO_TYPE_IVALUE                 (sho_ivalue_get_type())
#define SHO_IVALUE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), SHO_TYPE_IVALUE, ShoIValue))
#define SHO_IS_IVALUE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), SHO_TYPE_IVALUE))
#define SHO_IVALUE_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), SHO_TYPE_IVALUE, ShoIValueInterface))


typedef struct _ShoIValue               ShoIValue; /* dummy object */
typedef struct _ShoIValueInterface      ShoIValueInterface;

struct _ShoIValueInterface {
	GTypeInterface parent_iface;
	ShoIValue *(*deepCopy)(ShoIValue *self);
};

GType sho_ivalue_get_type(void);

ShoIValue *sho_ivalue_deep_copy(ShoIValue *self);

#endif /* SHOIVALUE_H_ */
