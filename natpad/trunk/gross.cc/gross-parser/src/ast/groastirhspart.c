/*
   File:    groastirhspart.c
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

#include "groastirhspart.h"

G_DEFINE_INTERFACE(GroAstIRhsPart, groast_irhs_part, G_TYPE_OBJECT);

static void groast_irhs_part_default_init (GroAstIRhsPartInterface *iface) {
}
