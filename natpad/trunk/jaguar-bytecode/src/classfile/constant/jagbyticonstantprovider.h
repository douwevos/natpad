/*
   File:    jagbyticonstantprovider.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Oct 31, 2012
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


#ifndef JAGBYTICONSTANTPROVIDER_H_
#define JAGBYTICONSTANTPROVIDER_H_

#include "jagbyticonstant.h"
#include <caterpillar.h>

#define JAG_BYT_TYPE_ICONSTANT_PROVIDER                 (jag_byt_iconstant_provider_get_type())
#define JAG_BYT_ICONSTANT_PROVIDER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST((obj), JAG_BYT_TYPE_ICONSTANT_PROVIDER, JagBytIConstantProvider))
#define JAG_BYT_IS_ICONSTANT_PROVIDER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE((obj), JAG_BYT_TYPE_ICONSTANT_PROVIDER))
#define JAG_BYT_ICONSTANT_PROVIDER_GET_INTERFACE(inst)  (G_TYPE_INSTANCE_GET_INTERFACE((inst), JAG_BYT_TYPE_ICONSTANT_PROVIDER, JagBytIConstantProviderInterface))


typedef struct _JagBytIConstantProvider               JagBytIConstantProvider; /* dummy object */
typedef struct _JagBytIConstantProviderInterface      JagBytIConstantProviderInterface;

struct _JagBytIConstantProviderInterface {
	GTypeInterface parent_iface;

	JagBytIConstant *(*get)(JagBytIConstantProvider *self, int index);
	CatStringWo *(*getConstantUtf8Text)(JagBytIConstantProvider *self, int index);

};

GType jag_byt_iconstant_provider_get_type(void);

JagBytIConstant *jag_byt_iconstant_provider_get(JagBytIConstantProvider *self, int index);
CatStringWo *jag_byt_iconstant_provider_get_constant_utf8_text(JagBytIConstantProvider *self, int index);

#endif /* JAGBYTICONSTANTPROVIDER_H_ */
