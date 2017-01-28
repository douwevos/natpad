/*
   File:    vipisequence.c
   Project: viper
   Author:  Douwe Vos
   Date:    May 9, 2013
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2013 Douwe Vos.

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


#include "vipisequence.h"

G_DEFINE_INTERFACE(VipISequence, vip_isequence, G_TYPE_OBJECT);

static void vip_isequence_default_init (VipISequenceInterface *klass) {
}


long long vip_isequence_next(VipISequence *self) {
	g_return_val_if_fail(VIP_IS_ISEQUENCE(self), 0);
	return VIP_ISEQUENCE_GET_INTERFACE(self)->next(self);
}
