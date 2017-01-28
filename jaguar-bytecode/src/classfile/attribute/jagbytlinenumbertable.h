/*
   File:    jagbytlinenumbertable.h
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


#ifndef JAGBYTLINENUMBERTABLE_H_
#define JAGBYTLINENUMBERTABLE_H_

#include "jagbytlinenumbertableentry.h"
#include <caterpillar.h>

G_BEGIN_DECLS

#define JAG_BYT_TYPE_LINE_NUMBER_TABLE              (jag_byt_line_number_table_get_type())
#define JAG_BYT_LINE_NUMBER_TABLE(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), jag_byt_line_number_table_get_type(), JagBytLineNumberTable))
#define JAG_BYT_LINE_NUMBER_TABLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), JAG_BYT_TYPE_LINE_NUMBER_TABLE, JagBytLineNumberTableClass))
#define JAG_BYT_IS_LINE_NUMBER_TABLE(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), JAG_BYT_TYPE_LINE_NUMBER_TABLE))
#define JAG_BYT_IS_LINE_NUMBER_TABLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), JAG_BYT_TYPE_LINE_NUMBER_TABLE))
#define JAG_BYT_LINE_NUMBER_TABLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), JAG_BYT_TYPE_LINE_NUMBER_TABLE, JagBytLineNumberTableClass))
#define JAG_BYT_LINE_NUMBER_TABLE_GET_PRIVATE(obj)  (G_TYPE_INSTANCE_GET_PRIVATE ((obj), JAG_BYT_TYPE_LINE_NUMBER_TABLE, JagBytLineNumberTablePrivate))

typedef struct _JagBytLineNumberTable               JagBytLineNumberTable;
typedef struct _JagBytLineNumberTablePrivate        JagBytLineNumberTablePrivate;
typedef struct _JagBytLineNumberTableClass          JagBytLineNumberTableClass;


struct _JagBytLineNumberTable {
	GObject parent;
	JagBytLineNumberTablePrivate *priv;
};

struct _JagBytLineNumberTableClass {
	GObjectClass parent_class;
};


GType jag_byt_line_number_table_get_type();

void jag_byt_line_number_table_construct(JagBytLineNumberTable *line_number_table);

JagBytLineNumberTable *jag_byt_line_number_table_new();

void jag_byt_line_number_table_add(JagBytLineNumberTable *line_number_table, JagBytLineNumberTableEntry *entry);

CatIIterator *jag_byt_line_number_table_iterator(JagBytLineNumberTable *line_number_table);


G_END_DECLS


#endif /* JAGBYTLINENUMBERTABLE_H_ */
