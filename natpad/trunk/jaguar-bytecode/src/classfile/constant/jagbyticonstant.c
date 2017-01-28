/*
   File:    jagbyticonstant.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 30, 2012
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

#include "jagbyticonstant.h"

G_DEFINE_INTERFACE(JagBytIConstant, jag_byt_iconstant, G_TYPE_OBJECT);

static void jag_byt_iconstant_default_init(JagBytIConstantInterface *klass) {
}


gboolean jag_byt_iconstant_is_resolved(JagBytIConstant *self) {
	g_return_val_if_fail(JAG_BYT_IS_ICONSTANT(self), FALSE);
	return JAG_BYT_ICONSTANT_GET_INTERFACE(self)->isResolved(self);
}

gboolean jag_byt_iconstant_try_resolve(JagBytIConstant *self, struct _JagBytConstantPool *constantPool, CatArrayWo *e_resolveStack) {
	g_return_val_if_fail(JAG_BYT_IS_ICONSTANT(self), FALSE);
	return JAG_BYT_ICONSTANT_GET_INTERFACE(self)->tryResolve(self, constantPool, e_resolveStack);
}

