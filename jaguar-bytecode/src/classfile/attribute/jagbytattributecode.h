/*
   File:    jagbytattributecode.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Mar 6, 2012
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

#ifndef JAGBYTATTRIBUTECODE_H_
#define JAGBYTATTRIBUTECODE_H_

#include "jagbytattribute.h"
#include "jagbytattributelocalvariabletable.h"
#include "jagbytattributelinenumbertable.h"
#include "../constant/jagbyticonstantprovider.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_ATTRIBUTE_CODE              (jag_byt_attribute_code_get_type())
#define JAG_BYT_ATTRIBUTE_CODE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_attribute_code_get_type(), JagBytAttributeCode))
#define JAG_BYT_ATTRIBUTE_CODE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_ATTRIBUTE_CODE, JagBytAttributeCodeClass))
#define JAG_BYT_IS_ATTRIBUTE_CODE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_ATTRIBUTE_CODE))
#define JAG_BYT_IS_ATTRIBUTE_CODE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_ATTRIBUTE_CODE))
#define JAG_BYT_ATTRIBUTE_CODE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_ATTRIBUTE_CODE, JagBytAttributeCodeClass))

typedef struct _JagBytAttributeCode               JagBytAttributeCode;
typedef struct _JagBytAttributeCodePrivate        JagBytAttributeCodePrivate;
typedef struct _JagBytAttributeCodeClass          JagBytAttributeCodeClass;


struct _JagBytAttributeCode {
	JagBytAttribute parent;
};

struct _JagBytAttributeCodeClass {
	JagBytAttributeClass parent_class;
};

extern CatS jag_txt_attribute_code;


GType jag_byt_attribute_code_get_type();

JagBytAttributeCode *jag_byt_attribute_code_new(CatStringWo *e_attribute_data, JagBytIConstantProvider *constant_provider);

CatStringWo *jag_byt_attribute_code_get_bytecode(JagBytAttributeCode *attribute_code);

CatArrayWo *jag_byt_attribute_code_get_exceptions(JagBytAttributeCode *attribute_code);

JagBytAttributeLocalVariableTable *jag_byt_attribute_code_get_local_variable_table(JagBytAttributeCode *attribute_code);

JagBytAttributeLineNumberTable *jag_byt_attribute_code_get_line_number_table(JagBytAttributeCode *attribute_code);


void jag_byt_attribute_code_as_source(JagBytAttributeCode *code_attribute, CatStringWo *e_source, CatStringWo *a_indent);

G_END_DECLS

#endif /* JAGBYTATTRIBUTECODE_H_ */
