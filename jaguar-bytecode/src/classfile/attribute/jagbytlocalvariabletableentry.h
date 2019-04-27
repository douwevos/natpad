/*
   File:    jagbytlocalvariabletableentry.h
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

#ifndef JAGBYTLOCALVARIABLETABLEENTRY_H_
#define JAGBYTLOCALVARIABLETABLEENTRY_H_

#include "../../ast/jagastdeclarationtype.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_LOCAL_VARIABLE_TABLE_ENTRY              (jag_byt_local_variable_table_entry_get_type())
#define JAG_BYT_LOCAL_VARIABLE_TABLE_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_local_variable_table_entry_get_type(), JagBytLocalVariableTableEntry))
#define JAG_BYT_LOCAL_VARIABLE_TABLE_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_LOCAL_VARIABLE_TABLE_ENTRY, JagBytLocalVariableTableEntryClass))
#define JAG_BYT_IS_LOCAL_VARIABLE_TABLE_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_LOCAL_VARIABLE_TABLE_ENTRY))
#define JAG_BYT_IS_LOCAL_VARIABLE_TABLE_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_LOCAL_VARIABLE_TABLE_ENTRY))
#define JAG_BYT_LOCAL_VARIABLE_TABLE_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_LOCAL_VARIABLE_TABLE_ENTRY, JagBytLocalVariableTableEntryClass))

typedef struct _JagBytLocalVariableTableEntry               JagBytLocalVariableTableEntry;
typedef struct _JagBytLocalVariableTableEntryPrivate        JagBytLocalVariableTableEntryPrivate;
typedef struct _JagBytLocalVariableTableEntryClass          JagBytLocalVariableTableEntryClass;


struct _JagBytLocalVariableTableEntry {
	GObject parent;
};

struct _JagBytLocalVariableTableEntryClass {
	GObjectClass parent_class;
};


GType jag_byt_local_variable_table_entry_get_type();

JagBytLocalVariableTableEntry *jag_byt_local_variable_table_entry_new(int startPc, int length, int name_index, CatStringWo *a_name_text, int descriptorIndex, JagAstDeclarationType *declaration_type, int index);

int jag_byt_local_variable_table_entry_get_start_pc(JagBytLocalVariableTableEntry *entry);
int jag_byt_local_variable_table_entry_get_length(JagBytLocalVariableTableEntry *entry);

int jag_byt_local_variable_table_entry_get_name_index(JagBytLocalVariableTableEntry *entry);
CatStringWo *jag_byt_local_variable_table_entry_get_name(JagBytLocalVariableTableEntry *entry);

int jag_byt_local_variable_table_entry_get_descriptor_index(JagBytLocalVariableTableEntry *entry);
JagAstDeclarationType *jag_byt_local_variable_table_entry_get_declaration_type(JagBytLocalVariableTableEntry *entry);

int jag_byt_local_variable_table_entry_get_index(JagBytLocalVariableTableEntry *entry);

G_END_DECLS

#endif /* JAGBYTLOCALVARIABLETABLEENTRY_H_ */
