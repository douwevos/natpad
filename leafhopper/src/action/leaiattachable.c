/*
   File:    leaiattachable.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    Jul 31, 2009
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2009 Douwe Vos.

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


#include "leaiattachable.h"

G_DEFINE_INTERFACE(LeaIAttachable, lea_iattachable, G_TYPE_OBJECT);

static void lea_iattachable_default_init(LeaIAttachableInterface *iface) {
}

void lea_iattachable_sensitivity_set(LeaIAttachable *self, gboolean sensitivity) {
	g_return_if_fail (LEA_IS_IATTACHABLE(self));
	LeaIAttachableInterface *iface = LEA_IATTACHABLE_GET_INTERFACE(self);
	if (iface->sensitivitySet) {
		iface->sensitivitySet(self, sensitivity);
	}
}


void lea_iattachable_visibility_set(LeaIAttachable *self, gboolean visibility) {
	g_return_if_fail (LEA_IS_IATTACHABLE(self));
	LeaIAttachableInterface *iface = LEA_IATTACHABLE_GET_INTERFACE(self);
	if (iface->visibilitySet) {
		iface->visibilitySet(self, visibility);
	}
}

void lea_iattachable_toggability_set(LeaIAttachable *self, gboolean toggable) {
	g_return_if_fail (LEA_IS_IATTACHABLE(self));
	LeaIAttachableInterface *iface = LEA_IATTACHABLE_GET_INTERFACE(self);
	if (iface->toggabilitySet) {
		iface->toggabilitySet(self, toggable);
	}
}


void lea_iattachable_toggled(LeaIAttachable *self, gboolean toggled) {
	g_return_if_fail (LEA_IS_IATTACHABLE(self));
	LeaIAttachableInterface *iface = LEA_IATTACHABLE_GET_INTERFACE(self);
	if (iface->toggled) {
		iface->toggled(self, toggled);
	}
}
