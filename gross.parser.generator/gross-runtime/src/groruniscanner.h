/*
   File:    groruniscanner.h
   Project: gross-runtime
   Author:  Douwe Vos
   Date:    Oct 11, 2016
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2016 Douwe Vos.

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

#ifndef GRORUNISCANNER_H_
#define GRORUNISCANNER_H_

#include "grorunitoken.h"
#include <caterpillar.h>

#define GRORUN_TYPE_ISCANNER                 (grorun_iscanner_get_type())
#define GRORUN_ISCANNER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), GRORUN_TYPE_ISCANNER, GroRunIScanner))
#define GRORUN_IS_ISCANNER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), GRORUN_TYPE_ISCANNER))
#define GRORUN_ISCANNER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), GRORUN_TYPE_ISCANNER, GroRunIScannerInterface))


typedef struct _GroRunIScanner               GroRunIScanner; /* dummy object */
typedef struct _GroRunIScannerInterface      GroRunIScannerInterface;


struct _GroRunIScannerInterface {
	GTypeInterface parent_iface;
	GroRunIToken *(*next)(GroRunIScanner *self);
};

GType grorun_iscanner_get_type(void);

GroRunIToken *grorun_iscanner_next(GroRunIScanner *self);

#endif /* GRORUNISCANNER_H_ */
