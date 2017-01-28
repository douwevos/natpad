/*
   File:    groibnfrhsenlistable.c
   Project: gross
   Author:  Douwe Vos
   Date:    Nov 17, 2016
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

#include "groibnfrhsenlistable.h"

G_DEFINE_INTERFACE(GroIBnfRhsEnlistable, gro_ibnf_rhs_enlistable, G_TYPE_OBJECT);

static void gro_ibnf_rhs_enlistable_default_init (GroIBnfRhsEnlistableInterface *iface) {
	iface->enlist = NULL;
}

CatArrayWo *gro_ibnf_rhs_enlistable_enlist(GroIBnfRhsEnlistable *enlistable) {
	return GRO_IBNF_RHS_ENLISTABLE_GET_INTERFACE(enlistable)->enlist(enlistable);
}
