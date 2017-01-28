/*
   File:    leaikeybinding.h
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


#ifndef LEAIKEYBINDING_H_
#define LEAIKEYBINDING_H_

#include <glib-object.h>
#include "leakeysequence.h"
#include "leakeycontext.h"


#define LEA_TYPE_IKEY_BINDING                 (lea_ikey_binding_get_type())
#define LEA_IKEY_BINDING(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), LEA_TYPE_IKEY_BINDING, LeaIKeyBinding))
#define LEA_IS_IKEY_BINDING(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), LEA_TYPE_IKEY_BINDING))
#define LEA_IKEY_BINDING_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), LEA_TYPE_IKEY_BINDING, LeaIKeyBindingInterface))


typedef struct _LeaIKeyBinding               LeaIKeyBinding; /* dummy object */
typedef struct _LeaIKeyBindingInterface      LeaIKeyBindingInterface;

struct _LeaIKeyBindingInterface {
	GTypeInterface parent_iface;

	LeaKeySequence *(*getDefaultKeySequence)(LeaIKeyBinding *self);
	LeaKeySequence *(*getKeySequence)(LeaIKeyBinding *self);
	LeaKeyContext *(*getKeyContext)(LeaIKeyBinding *self);

	void (*setKeySequence)(LeaIKeyBinding *self, LeaKeySequence *new_key_sequence);


	void (*activate)(LeaIKeyBinding *self);
};

GType lea_ikey_binding_get_type(void);


LeaKeySequence *lea_ikey_binding_get_default_key_sequence(LeaIKeyBinding *self);

LeaKeySequence *lea_ikey_binding_get_key_sequence(LeaIKeyBinding *self);
void lea_ikey_binding_set_key_sequence(LeaIKeyBinding *self, LeaKeySequence *key_sequence);

void lea_ikey_binding_activate(LeaIKeyBinding *self);

LeaKeyContext *lea_ikey_binding_get_key_context(LeaIKeyBinding *self);


#endif /* LEAIKEYBINDING_H_ */
