/*
   File:    jagbytlinenumbertableentry.h
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

#ifndef JAGBYTLINENUMBERTABLEENTRY_H_
#define JAGBYTLINENUMBERTABLEENTRY_H_

#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_LINE_NUMBER_TABLE_ENTRY              (jag_byt_line_number_table_entry_get_type())
#define JAG_BYT_LINE_NUMBER_TABLE_ENTRY(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_line_number_table_entry_get_type(), JagBytLineNumberTableEntry))
#define JAG_BYT_LINE_NUMBER_TABLE_ENTRY_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_LINE_NUMBER_TABLE_ENTRY, JagBytLineNumberTableEntryClass))
#define JAG_BYT_IS_LINE_NUMBER_TABLE_ENTRY(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_LINE_NUMBER_TABLE_ENTRY))
#define JAG_BYT_IS_LINE_NUMBER_TABLE_ENTRY_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_LINE_NUMBER_TABLE_ENTRY))
#define JAG_BYT_LINE_NUMBER_TABLE_ENTRY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_LINE_NUMBER_TABLE_ENTRY, JagBytLineNumberTableEntryClass))

typedef struct _JagBytLineNumberTableEntry               JagBytLineNumberTableEntry;
typedef struct _JagBytLineNumberTableEntryPrivate        JagBytLineNumberTableEntryPrivate;
typedef struct _JagBytLineNumberTableEntryClass          JagBytLineNumberTableEntryClass;


struct _JagBytLineNumberTableEntry {
	GObject parent;
};

struct _JagBytLineNumberTableEntryClass {
	GObjectClass parent_class;
};


GType jag_byt_line_number_table_entry_get_type();

JagBytLineNumberTableEntry *jag_byt_line_number_table_entry_new(int startPc, int lineNumber);

int jag_byt_line_number_table_entry_get_start_pc(JagBytLineNumberTableEntry *entry);

int jag_byt_line_number_table_entry_get_line_number(JagBytLineNumberTableEntry *entry);

G_END_DECLS

#endif /* JAGBYTLINENUMBERTABLEENTRY_H_ */
