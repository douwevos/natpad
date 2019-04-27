/*
   File:    jagbytattributelocalvariabletable.h
   Project: jaguar-bytecode
   Author:  Douwe Vos
   Date:    Nov 5, 2012
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

#ifndef JAGBYTATTRIBUTELOCALVARIABLETABLE_H_
#define JAGBYTATTRIBUTELOCALVARIABLETABLE_H_

#include "jagbytattribute.h"
#include "jagbytlocalvariabletable.h"
#include "../constant/jagbyticonstantprovider.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_ATTRIBUTE_LOCAL_VARIABLE_TABLE              (jag_byt_attribute_local_variable_table_get_type())
#define JAG_BYT_ATTRIBUTE_LOCAL_VARIABLE_TABLE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_attribute_local_variable_table_get_type(), JagBytAttributeLocalVariableTable))
#define JAG_BYT_ATTRIBUTE_LOCAL_VARIABLE_TABLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_ATTRIBUTE_LOCAL_VARIABLE_TABLE, JagBytAttributeLocalVariableTableClass))
#define JAG_BYT_IS_ATTRIBUTE_LOCAL_VARIABLE_TABLE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_ATTRIBUTE_LOCAL_VARIABLE_TABLE))
#define JAG_BYT_IS_ATTRIBUTE_LOCAL_VARIABLE_TABLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_ATTRIBUTE_LOCAL_VARIABLE_TABLE))
#define JAG_BYT_ATTRIBUTE_LOCAL_VARIABLE_TABLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_ATTRIBUTE_LOCAL_VARIABLE_TABLE, JagBytAttributeLocalVariableTableClass))

typedef struct _JagBytAttributeLocalVariableTable               JagBytAttributeLocalVariableTable;
typedef struct _JagBytAttributeLocalVariableTablePrivate        JagBytAttributeLocalVariableTablePrivate;
typedef struct _JagBytAttributeLocalVariableTableClass          JagBytAttributeLocalVariableTableClass;


struct _JagBytAttributeLocalVariableTable {
	JagBytAttribute parent;
};

struct _JagBytAttributeLocalVariableTableClass {
	JagBytAttributeClass parent_class;
};

extern CatS jag_txt_attribute_local_variable_table;


GType jag_byt_attribute_local_variable_table_get_type();

JagBytAttributeLocalVariableTable *jag_byt_attribute_local_variable_table_new(CatStringWo *e_attribute_data, JagBytIConstantProvider *constant_provider);


JagBytLocalVariableTable *jag_byt_attribute_local_variable_table_get_table(JagBytAttributeLocalVariableTable *attr_local_variable_table);

G_END_DECLS

#endif /* JAGBYTATTRIBUTELOCALVARIABLETABLE_H_ */
