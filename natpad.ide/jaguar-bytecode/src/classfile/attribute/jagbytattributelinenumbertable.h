/*
   File:    jagbytattributelinenumbertable.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 8, 2012
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


#ifndef JAGBYTATTRIBUTELINENUMBERTABLE_H_
#define JAGBYTATTRIBUTELINENUMBERTABLE_H_

#include "jagbytattribute.h"
#include "jagbytlinenumbertable.h"
#include "../constant/jagbyticonstantprovider.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_ATTRIBUTE_LINE_NUMBER_TABLE              (jag_byt_attribute_line_number_table_get_type())
#define JAG_BYT_ATTRIBUTE_LINE_NUMBER_TABLE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_attribute_line_number_table_get_type(), JagBytAttributeLineNumberTable))
#define JAG_BYT_ATTRIBUTE_LINE_NUMBER_TABLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_ATTRIBUTE_LINE_NUMBER_TABLE, JagBytAttributeLineNumberTableClass))
#define JAG_BYT_IS_ATTRIBUTE_LINE_NUMBER_TABLE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_ATTRIBUTE_LINE_NUMBER_TABLE))
#define JAG_BYT_IS_ATTRIBUTE_LINE_NUMBER_TABLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_ATTRIBUTE_LINE_NUMBER_TABLE))
#define JAG_BYT_ATTRIBUTE_LINE_NUMBER_TABLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_ATTRIBUTE_LINE_NUMBER_TABLE, JagBytAttributeLineNumberTableClass))
#define JAG_BYT_ATTRIBUTE_LINE_NUMBER_TABLE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_ATTRIBUTE_LINE_NUMBER_TABLE, JagBytAttributeLineNumberTablePrivate))

typedef struct _JagBytAttributeLineNumberTable               JagBytAttributeLineNumberTable;
typedef struct _JagBytAttributeLineNumberTablePrivate        JagBytAttributeLineNumberTablePrivate;
typedef struct _JagBytAttributeLineNumberTableClass          JagBytAttributeLineNumberTableClass;


struct _JagBytAttributeLineNumberTable {
	JagBytAttribute parent;
	JagBytAttributeLineNumberTablePrivate *priv;
};

struct _JagBytAttributeLineNumberTableClass {
	JagBytAttributeClass parent_class;
};

CatS jag_txt_attribute_line_number_table;


GType jag_byt_attribute_line_number_table_get_type();

JagBytAttributeLineNumberTable *jag_byt_attribute_line_number_table_new(CatStringWo *e_attribute_data, JagBytIConstantProvider *constant_provider);


JagBytLineNumberTable *jag_byt_attribute_line_number_table_get_table(JagBytAttributeLineNumberTable *attr_line_number_table);

G_END_DECLS


#endif /* JAGBYTATTRIBUTELINENUMBERTABLE_H_ */
