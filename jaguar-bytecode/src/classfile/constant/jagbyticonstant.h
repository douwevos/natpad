/*
   File:    jagbyticonstant.h
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


#ifndef JAGBYTICONSTANT_H_
#define JAGBYTICONSTANT_H_

#include <caterpillar.h>

#define JAG_BYT_TYPE_ICONSTANT                 (jag_byt_iconstant_get_type())
#define JAG_BYT_ICONSTANT(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_BYT_TYPE_ICONSTANT, JagBytIConstant))
#define JAG_BYT_IS_ICONSTANT(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_BYT_TYPE_ICONSTANT))
#define JAG_BYT_ICONSTANT_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_BYT_TYPE_ICONSTANT, JagBytIConstantInterface))


typedef struct _JagBytIConstant               JagBytIConstant; /* dummy object */
typedef struct _JagBytIConstantInterface      JagBytIConstantInterface;

struct _JagBytConstantPool;

struct _JagBytIConstantInterface {
	GTypeInterface parent_iface;


	gboolean (*isResolved)(JagBytIConstant *self);

	gboolean (*tryResolve)(JagBytIConstant *self, struct _JagBytConstantPool *constantPool, CatArrayWo *e_resolveStack);
};

GType jag_byt_iconstant_get_type(void);

gboolean jag_byt_iconstant_is_resolved(JagBytIConstant *self);

gboolean jag_byt_iconstant_try_resolve(JagBytIConstant *self, struct _JagBytConstantPool *constantPool, CatArrayWo *e_resolveStack);



#endif /* JAGBYTICONSTANT_H_ */
