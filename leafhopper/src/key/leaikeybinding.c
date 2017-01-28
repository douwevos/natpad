/*
   File:    leaikeybinding.c
   Project: leafhopper
   Author:  Douwe Vos
   Date:    May 13, 2010
   e-mail:  dmvos2000(at)yahoo.com

   Copyright (C) 2010 Douwe Vos.

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


#include "leaikeybinding.h"

G_DEFINE_INTERFACE(LeaIKeyBinding, lea_ikey_binding, G_TYPE_OBJECT)

static void lea_ikey_binding_default_init(LeaIKeyBindingInterface *klass) {
}


LeaKeySequence *lea_ikey_binding_get_default_key_sequence(LeaIKeyBinding *self) {
	g_return_val_if_fail(LEA_IS_IKEY_BINDING(self), NULL);
	return LEA_IKEY_BINDING_GET_INTERFACE(self)->getDefaultKeySequence(self);
}


LeaKeySequence *lea_ikey_binding_get_key_sequence(LeaIKeyBinding *self) {
	g_return_val_if_fail(LEA_IS_IKEY_BINDING(self), NULL);
	return LEA_IKEY_BINDING_GET_INTERFACE(self)->getKeySequence(self);
}

void lea_ikey_binding_set_key_sequence(LeaIKeyBinding *self, LeaKeySequence *key_sequence) {
	g_return_if_fail(LEA_IS_IKEY_BINDING(self));
	LEA_IKEY_BINDING_GET_INTERFACE(self)->setKeySequence(self, key_sequence);
}

void lea_ikey_binding_activate(LeaIKeyBinding *self) {
	g_return_if_fail(LEA_IS_IKEY_BINDING(self));
	LEA_IKEY_BINDING_GET_INTERFACE(self)->activate(self);
}

LeaKeyContext *lea_ikey_binding_get_key_context(LeaIKeyBinding *self) {
	g_return_val_if_fail(LEA_IS_IKEY_BINDING(self), NULL);
	return LEA_IKEY_BINDING_GET_INTERFACE(self)->getKeyContext(self);
}
