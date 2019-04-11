/*
   File:    jagbytfield.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Feb 1, 2012
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

#ifndef JAGBYTFIELD_H_
#define JAGBYTFIELD_H_

#include "../ast/jagastdeclarationtype.h"
#include "../ast/jagastmodifiers.h"
#include "attribute/jagbytattributemap.h"
#include "constant/jagbyticonstantprovider.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_FIELD              (jag_byt_field_get_type())
#define JAG_BYT_FIELD(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_field_get_type(), JagBytField))
#define JAG_BYT_FIELD_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_FIELD, JagBytFieldClass))
#define JAG_BYT_IS_FIELD(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_FIELD))
#define JAG_BYT_IS_FIELD_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_FIELD))
#define JAG_BYT_FIELD_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_FIELD, JagBytFieldClass))
#define JAG_BYT_FIELD_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_FIELD, JagBytFieldPrivate))

typedef struct _JagBytField               JagBytField;
typedef struct _JagBytFieldPrivate        JagBytFieldPrivate;
typedef struct _JagBytFieldClass          JagBytFieldClass;


struct _JagBytField {
	GObject parent;
	JagBytFieldPrivate *priv;
};

struct _JagBytFieldClass {
	GObjectClass parent_class;
};


GType jag_byt_field_get_type();

JagBytField *jag_byt_field_new(JagBytIConstantProvider *constant_provider, uint16_t accessFlags, uint16_t nameIndex, uint16_t descriptorIndex, JagBytAttributeMap *attribute_map);

uint16_t jag_byt_field_get_access_flags(const JagBytField *field);
uint16_t jag_byt_field_get_name_index(const JagBytField *field);
uint16_t jag_byt_field_get_descriptor_index(const JagBytField *field);

JagAstDeclarationType *jag_byt_field_get_declaration_type(const JagBytField *field);

CatStringWo *jag_byt_field_get_name(const JagBytField *field);

JagBytIConstant *jag_byt_field_get_constant_value(const JagBytField *field);

JagAstDeclarationType *jag_byt_field_parse_descriptor(CatStringWo *a_description);

G_END_DECLS

#endif /* JAGBYTFIELD_H_ */
