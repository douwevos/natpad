/*
   File:    jagbyticatchorfinally.h
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


#ifndef JAGBYTICATCHORFINALLY_H_
#define JAGBYTICATCHORFINALLY_H_

#include <glib-object.h>

#define JAG_BYT_TYPE_ICATCH_OR_FINALLY                 (jag_byt_icatch_or_finally_get_type())
#define JAG_BYT_ICATCH_OR_FINALLY(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_BYT_TYPE_ICATCH_OR_FINALLY, JagBytICatchOrFinally))
#define JAG_BYT_IS_ICATCH_OR_FINALLY(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_BYT_TYPE_ICATCH_OR_FINALLY))
#define JAG_BYT_ICATCH_OR_FINALLY_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_BYT_TYPE_ICATCH_OR_FINALLY, JagBytICatchOrFinallyInterface))


typedef struct _JagBytICatchOrFinally               JagBytICatchOrFinally; /* dummy object */
typedef struct _JagBytICatchOrFinallyInterface      JagBytICatchOrFinallyInterface;

struct _JagBytICatchOrFinallyInterface {
	GTypeInterface parent_iface;

	int (*getHandlePc)(JagBytICatchOrFinally *self);
};

GType jag_byt_icatch_or_finally_get_type(void);

int jag_byt_icatch_or_finally_get_handle_pc(JagBytICatchOrFinally *self);


#endif /* JAGBYTICATCHORFINALLY_H_ */
