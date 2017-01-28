/*
   File:    jagbytlocalvariabletable.h
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


#ifndef JAGBYTLOCALVARIABLETABLE_H_
#define JAGBYTLOCALVARIABLETABLE_H_

#include "jagbytlocalvariabletableentry.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_LOCAL_VARIABLE_TABLE              (jag_byt_local_variable_table_get_type())
#define JAG_BYT_LOCAL_VARIABLE_TABLE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_local_variable_table_get_type(), JagBytLocalVariableTable))
#define JAG_BYT_LOCAL_VARIABLE_TABLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_LOCAL_VARIABLE_TABLE, JagBytLocalVariableTableClass))
#define JAG_BYT_IS_LOCAL_VARIABLE_TABLE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_LOCAL_VARIABLE_TABLE))
#define JAG_BYT_IS_LOCAL_VARIABLE_TABLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_LOCAL_VARIABLE_TABLE))
#define JAG_BYT_LOCAL_VARIABLE_TABLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_LOCAL_VARIABLE_TABLE, JagBytLocalVariableTableClass))
#define JAG_BYT_LOCAL_VARIABLE_TABLE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_LOCAL_VARIABLE_TABLE, JagBytLocalVariableTablePrivate))

typedef struct _JagBytLocalVariableTable               JagBytLocalVariableTable;
typedef struct _JagBytLocalVariableTablePrivate        JagBytLocalVariableTablePrivate;
typedef struct _JagBytLocalVariableTableClass          JagBytLocalVariableTableClass;


struct _JagBytLocalVariableTable {
	GObject parent;
	JagBytLocalVariableTablePrivate *priv;
};

struct _JagBytLocalVariableTableClass {
	GObjectClass parent_class;
};


GType jag_byt_local_variable_table_get_type();

JagBytLocalVariableTable *jag_byt_local_variable_table_new();

CatStringWo *jag_byt_local_variable_table_find_name(JagBytLocalVariableTable *local_variable_table, int frameIndex, int index);

JagBytLocalVariableTableEntry *jag_byt_local_variable_table_find_entry(JagBytLocalVariableTable *local_variable_table, int frameIndex, int index);

void jag_byt_local_variable_table_add(JagBytLocalVariableTable *local_variable_table, JagBytLocalVariableTableEntry *entry);
void jag_byt_local_variable_table_remove(JagBytLocalVariableTable *local_variable_table, JagBytLocalVariableTableEntry *entry);

gboolean jag_byt_local_variable_table_isEmpty(JagBytLocalVariableTable *local_variable_table);
int jag_byt_local_variable_table_count(JagBytLocalVariableTable *local_variable_table);


G_END_DECLS


#endif /* JAGBYTLOCALVARIABLETABLE_H_ */
