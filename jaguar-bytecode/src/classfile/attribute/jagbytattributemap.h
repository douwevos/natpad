/*
   File:    jagbytattributemap.h
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

#ifndef JAGBYTATTRIBUTEMAP_H_
#define JAGBYTATTRIBUTEMAP_H_

#include "jagbytattribute.h"
#include "jagbytattributecode.h"
#include "jagbytattributeexceptions.h"
#include "jagbytattributeconstantvalue.h"
#include "jagbytattributelinenumbertable.h"
#include "../../jagjobjectinputstream.h"
#include "../constant/jagbyticonstantprovider.h"

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_ATTRIBUTE_MAP              (jag_byt_attribute_map_get_type())
#define JAG_BYT_ATTRIBUTE_MAP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_attribute_map_get_type(), JagBytAttributeMap))
#define JAG_BYT_ATTRIBUTE_MAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_ATTRIBUTE_MAP, JagBytAttributeMapClass))
#define JAG_BYT_IS_ATTRIBUTE_MAP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_ATTRIBUTE_MAP))
#define JAG_BYT_IS_ATTRIBUTE_MAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_ATTRIBUTE_MAP))
#define JAG_BYT_ATTRIBUTE_MAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_ATTRIBUTE_MAP, JagBytAttributeMapClass))

typedef struct _JagBytAttributeMap               JagBytAttributeMap;
typedef struct _JagBytAttributeMapPrivate        JagBytAttributeMapPrivate;
typedef struct _JagBytAttributeMapClass          JagBytAttributeMapClass;


struct _JagBytAttributeMap {
	GObject parent;
};

struct _JagBytAttributeMapClass {
	GObjectClass parent_class;
};


GType jag_byt_attribute_map_get_type();

JagBytAttributeMap *jag_byt_attribute_map_new(JagJObjectInputStream *dis, JagBytIConstantProvider *constant_provider, int cnt);

JagBytAttribute *jag_byt_attribute_map_get(JagBytAttributeMap *attribute_map, CatStringWo *a_key);

JagBytAttributeCode *jag_byt_attribute_map_get_code(JagBytAttributeMap *attribute_map);
JagBytAttributeConstantValue *jag_byt_attribute_map_get_constant_value(JagBytAttributeMap *attribute_map);
JagBytAttributeExceptions *jag_byt_attribute_map_get_exceptions(JagBytAttributeMap *attribute_map);

G_END_DECLS

#endif /* JAGBYTATTRIBUTEMAP_H_ */
