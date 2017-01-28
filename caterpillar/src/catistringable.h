/*
   File:    catistringable.h
   Project: caterpillar
   Author:  Douwe Vos
   Date:    Nov 12, 2012
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


#ifndef CATISTRINGABLE_H_
#define CATISTRINGABLE_H_

#include <glib-object.h>

#define CAT_TYPE_ISTRINGABLE                 (cat_istringable_get_type())
#define CAT_ISTRINGABLE(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), CAT_TYPE_ISTRINGABLE, CatIStringable))
#define CAT_IS_ISTRINGABLE(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), CAT_TYPE_ISTRINGABLE))
#define CAT_ISTRINGABLE_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), CAT_TYPE_ISTRINGABLE, CatIStringableInterface))


typedef struct _CatIStringable               CatIStringable; /* dummy object */
typedef struct _CatIStringableInterface      CatIStringableInterface;

struct _CatStringWo;

struct _CatIStringableInterface {
	GTypeInterface parent_iface;

	void (*print)(CatIStringable *self, struct _CatStringWo *append_to);
};

GType cat_istringable_get_type(void);

void cat_istringable_print(CatIStringable *self, struct _CatStringWo *append_to);


#endif /* CATISTRINGABLE_H_ */
