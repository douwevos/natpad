/*
   File:    groastirhsenlistable.c
   Project: gross-parser
   Author:  Douwe Vos
   Date:    Nov 28, 2016
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

#include "groastirhsenlistable.h"

G_DEFINE_INTERFACE(GroAstIRhsEnlistable, groast_irhs_enlistable, G_TYPE_OBJECT);

static void groast_irhs_enlistable_default_init (GroAstIRhsEnlistableInterface *iface) {
	iface->enlist = NULL;
}

CatArrayWo *groast_irhs_enlistable_enlist(GroAstIRhsEnlistable *enlistable) {
	return GROAST_IRHS_ENLISTABLE_GET_INTERFACE(enlistable)->enlist(enlistable);
}
