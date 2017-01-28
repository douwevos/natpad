
/*
   File:    groiparseaction.c
   Project: gross
   Author:  Douwe Vos
   Date:    Jun 5, 2016
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

#include "groiparseaction.h"

G_DEFINE_INTERFACE(GroIParseAction, gro_iparse_action, G_TYPE_OBJECT);

static void gro_iparse_action_default_init(GroIParseActionInterface *iface) {
}

GroActionKind gro_iparse_action_kind(GroIParseAction *action) {
	return GRO_IPARSE_ACTION_GET_INTERFACE(action)->kind(action);
}
