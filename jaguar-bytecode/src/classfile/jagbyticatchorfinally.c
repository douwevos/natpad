/*
   File:    jagbyticatchorfinally.c
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 4, 2012
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

#include "jagbyticatchorfinally.h"

G_DEFINE_INTERFACE(JagBytICatchOrFinally, jag_byt_icatch_or_finally, G_TYPE_OBJECT);

static void jag_byt_icatch_or_finally_default_init(JagBytICatchOrFinallyInterface *klass) {
}



int jag_byt_icatch_or_finally_get_handle_pc(JagBytICatchOrFinally *self) {
	g_return_val_if_fail(JAG_BYT_IS_ICATCH_OR_FINALLY(self), -1);
	return JAG_BYT_ICATCH_OR_FINALLY_GET_INTERFACE(self)->getHandlePc(self);
}
