/*
   File:    jagbytmethod.h
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

#ifndef JAGBYTMETHOD_H_
#define JAGBYTMETHOD_H_

#include "jagbyttrycatchlist.h"
#include "constant/jagbytconstantpool.h"
#include "constant/jagbyticonstantprovider.h"
#include "attribute/jagbytattributemap.h"
#include "attribute/jagbytattributelocalvariabletable.h"
#include "attribute/jagbytattributelinenumbertable.h"
#include "../ast/jagastmodifiers.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_METHOD              (jag_byt_method_get_type())
#define JAG_BYT_METHOD(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_method_get_type(), JagBytMethod))
#define JAG_BYT_METHOD_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_METHOD, JagBytMethodClass))
#define JAG_BYT_IS_METHOD(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_METHOD))
#define JAG_BYT_IS_METHOD_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_METHOD))
#define JAG_BYT_METHOD_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_METHOD, JagBytMethodClass))
#define JAG_BYT_METHOD_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_METHOD, JagBytMethodPrivate))

typedef struct _JagBytMethod               JagBytMethod;
typedef struct _JagBytMethodPrivate        JagBytMethodPrivate;
typedef struct _JagBytMethodClass          JagBytMethodClass;


struct _JagBytMethod {
	GObject parent;
	JagBytMethodPrivate *priv;
};

struct _JagBytMethodClass {
	GObjectClass parent_class;
};


GType jag_byt_method_get_type();

JagBytMethod *jag_byt_method_new(JagBytName *mainTypeName, JagBytIConstantProvider *constant_provider, uint16_t accessFlags, uint16_t nameIndex, uint16_t descriptorIndex, JagBytAttributeMap *attribute_map);

uint16_t jag_byt_method_get_access_flags(const JagBytMethod *method);
JagAstModifiers *jag_byt_method_get_modifiers(const JagBytMethod *method);

uint16_t jag_byt_method_get_name_index(const JagBytMethod *method);
uint16_t jag_byt_method_get_descriptor_index(const JagBytMethod *method);
JagBytAttributeMap *jag_byt_method_get_attribute_map(const JagBytMethod *method);


CatStringWo *jag_byt_method_get_name(const JagBytMethod *method);

JagBytMethodHeader *jag_byt_method_get_method_header(const JagBytMethod *method);

JagBytAttributeLocalVariableTable *jag_byt_method_get_local_variable_table(JagBytMethod *method);

JagBytAttributeLineNumberTable *jag_byt_method_get_line_number_table(JagBytMethod *method);

CatStringWo *jag_byt_method_get_bytecode(const JagBytMethod *method);

JagBytTryCatchList *jag_byt_method_get_try_catch_list(JagBytMethod *method);

G_END_DECLS

#endif /* JAGBYTMETHOD_H_ */
